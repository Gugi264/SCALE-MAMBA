//
// Created by sguggi on 17.08.22.
//


#include "TaasProvider.h"

#define HIGHLIGHT(__O__) std::cout<<"\e[1;31m"<<__O__<<"\e[0m"<<std::endl
#define OUT(__O__) cout << __O__ << endl;


TaasProvider::TaasProvider(Player &P, vector<string> availableTripleProviders, string ledgerAddress, bigint prime)
        : P_(P), ledgerAddress_(std::move(ledgerAddress)), PRG_(P.G), prime_(std::move(prime)),
          shamirSs_(prime_, PRG_) {

    PRG_.ReSeed(10000 + P.whoami());
    //createMacShare();
    macShare_ = P.get_mac_key(0);
    cout << "prime in taas: " << prime_ << endl;
    chooseTripleProviders(std::move(availableTripleProviders));
}

map<int, bigint> TaasProvider::splitMacShare() {
    bigint macShareBigInt;
    to_bigint(macShareBigInt, macShare_);
    vector<int> pointToEval;
    for (std::map<int, string>::iterator it = providersMap_.begin(); it != providersMap_.end(); ++it) {
        pointToEval.push_back(it->first);
    }

    auto macShares = shamirSs_.split(pointToEval.size(), pointToEval.size() - 1, macShareBigInt, pointToEval);
//    cout << "printing macSahres" << endl;
//    for (const auto &elem: macShares) {
//        std::cout << elem.first << " " << elem.second << "\n";
//    }
    return macShares;
}

void TaasProvider::chooseTripleProviders(vector<string> availableTripleProviders) {
    int excluded;
    uint8_t tmp_excluded;
    unsigned int len = 1;
    vector<string> chosenProviders;
    // cout << "size of available: " << availableTripleProviders.size() << endl;
    if (P_.whoami() == 0) {
        tmp_excluded = PRG_.get_uint();
        excluded = tmp_excluded % (availableTripleProviders.size());
        P_.send_all(&tmp_excluded, len, 0);
    } else {
        const uint8_t *tmp = P_.receive_from_player(0, len, 0);
        excluded = *tmp;
        excluded = excluded % (availableTripleProviders.size());
    }
    //cout << "excludedProvider: " << excluded << endl;

    for (int i = 0; i < availableTripleProviders.size(); ++i) {
        if (i != excluded) {
            chosenProviders.push_back(availableTripleProviders[i]);
        }
    }
    cout << "Chosen triple providers" << endl;
    for (const auto &value: chosenProviders) {
        std::cout << value << "\n";
    }
    downloadAllServiceProviderNr(chosenProviders);
}

void TaasProvider::getTriples(int nrOfTriples, list<Share> &a, list<Share> &b, list<Share> &c) {
    auto uuid = createUUID();
    //cout << "before splitMachsare" << endl;
    auto partMap = splitMacShare();
    //cout << "after splitMachsare" << endl;
    map<int, string> encryptedMacShareParts;
    for (const auto &elem: partMap) {
        //cout << "in part map" << endl;
        //cout << "first: " << elem.first << endl;
        auto encryptedPart = downloadCertificateEncryptAndEncode(providersMap_[elem.first], partMap[elem.first]);
        encryptedMacShareParts[elem.first] = encryptedPart;
    }
    sendRequestToLedger(nrOfTriples, encryptedMacShareParts, uuid, TRIPLES);
   // cout << "get triples finished " << endl;
    communicateWithSP(nrOfTriples, uuid, TRIPLES, a, b, c);
    OUT("finished getTriples");

}

void TaasProvider::sendRequestToLedger(int count, const map<int, string> &encryptedShareParts, const string &uuid,
                                       RequestChoice choice) {
    sio::client h;
    bool finished = false;
    bool retVal = false;
    condition_variable_any cond;
    mutex mu;
    h.set_open_listener([&]() {
        //cout << "in set open listener" << endl;
    });
    h.socket()->on("reserve_response", [&](sio::event &ev) {
        //cout << "in resever_response" << endl;
        lock_guard<mutex> lk(mu);
        retVal = ev.get_message()->get_bool();
        finished = true;
        cond.notify_all();
    });
    h.set_fail_listener([&]() {
        HIGHLIGHT("sio failed");
    });
    h.connect(ledgerAddress_);
    nlohmann::json j;
    nlohmann::json macs_json;
    for (const auto &elem: encryptedShareParts) {
        macs_json[to_string(elem.first)] = elem.second;
    }

    j["tripleCount"] = (choice == TRIPLES) ? count : 0;
    j["r_count"] = (choice == RND_VALUE) ? count : 0;
    gfp r_s;
    r_s.randomize(PRG_);
    bigint r_sBigint;
    to_bigint(r_sBigint, r_s);
    j["r_s"] = r_sBigint.get_str();
    j["macs"] = macs_json;
    j["playerID"] = P_.whoami() + 1;
    j["CP"] = P_.nplayers();
    j["sp_r"] = "";
    j["room"] = uuid;

    h.socket()->emit("reserve", to_string(j));

    unique_lock<mutex> lk(mu);
    cond.wait(mu, [&finished] { return finished; });
    lk.unlock();
    if (!retVal) {
        HIGHLIGHT("Error from ledger");
        exit(-1);
    }
    h.sync_close();
}

void TaasProvider::closeListen(sio::client::close_reason const& reason) {
    //HIGHLIGHT("one connection closed");
}

void TaasProvider::communicateWithSP(int count, string uuid, RequestChoice choice, list<Share> &a, list<Share> &b, list<Share> &c) {
    map<int, sio::client> socketMap;
    map<int, bool> socketConnectedMap;
    map<int, TransformedShareMsg> transformedShareMsgMap;
    map<int, BroadcastTransformShareMsg> broadcastTransformShareMsgMap;
    map<int, TransformedShareMsgRndVal> transformedShareRndMsgMap;
    map<int, BroadcastTransformShareRndValMsg> broadcastTransformShareRndMsgMap;
    map<int, mutex> mutexes;
    map<int, condition_variable> conditionsMap;
    map<int, bool> broadcastMap;

    for (const auto &elem: providersMap_) {
        socketConnectedMap[elem.first] = false;
        broadcastMap[elem.first] = false;
    }

    for (const auto &elem: providersMap_) {
        //cout << "in first loop for " << elem.second << endl << flush;
        auto h = &socketMap[elem.first];
        h->socket()->on("transformshare_point_to_point_rnd", [&](sio::event &ev) {
            //cout << "In transformPointToPoint rnd" << endl;
            string msg = *ev.get_message()->get_binary();
            //OUT(msg);
            TransformedShareMsgRndVal transformedShareMsg;
            bool error = transformedShareMsg.ParseFromString(msg);
            if (!error) {
                cout << "Failed parsing transformedshareMsg" << endl;
            }
            transformedShareRndMsgMap[transformedShareMsg.coeff()] = transformedShareMsg;

            lock_guard<mutex> lk(mutexes[elem.first]);
            socketConnectedMap[elem.first] = true;
            conditionsMap[elem.first].notify_all();
        });

        h->socket()->on("transformshare_point_to_point_triple", [&](sio::event &ev) {
            //cout << "In transformPointToPoint Triple" << endl << flush;
            string msg = *ev.get_message()->get_binary();
            TransformedShareMsg transformedShareMsg;
            bool error = transformedShareMsg.ParseFromString(msg);
            if (!error) {
                cout << "Failed parsing transformedshareMsg" << endl;
            }
            transformedShareMsgMap[transformedShareMsg.coeff()] = transformedShareMsg;

            lock_guard<mutex> lk(mutexes[elem.first]);
            socketConnectedMap[elem.first] = true;
            conditionsMap[elem.first].notify_all();
            //OUT("finished with trfs ptp triple");
        });

        h->socket()->on("transformshare_broadcast_triple", [&](sio::event &ev) {
            cout << "In transform share broadcast triple" << endl << flush;
            string msg = *ev.get_message()->get_binary();
            BroadcastTransformShareMsg broadcastTransformShareMsg;
            bool error = broadcastTransformShareMsg.ParseFromString(msg);
            if (!error) {
                cout << "Failed parsing broadcastTransformShareMsg" << endl;
            }
            broadcastTransformShareMsgMap[broadcastTransformShareMsg.coeff()] = broadcastTransformShareMsg;

            lock_guard<mutex> lk(mutexes[elem.first]);
            broadcastMap[elem.first] = true;
            conditionsMap[elem.first].notify_all();
        });

        h->socket()->on("transformshare_broadcast_rnd", [&](sio::event &ev) {
            //cout << "In transform share broadcast rnd" << endl << flush;
            string msg = *ev.get_message()->get_binary();
            BroadcastTransformShareRndValMsg broadcastTransformShareMsg;
            bool error = broadcastTransformShareMsg.ParseFromString(msg);
            if (!error) {
                cout << "Failed parsing broadcastTransformShareMsg" << endl;
            }
            broadcastTransformShareRndMsgMap[broadcastTransformShareMsg.coeff()] = broadcastTransformShareMsg;

            lock_guard<mutex> lk(mutexes[elem.first]);
            broadcastMap[elem.first] = true;
            conditionsMap[elem.first].notify_all();
        });

        h->set_open_listener([&]() {
            //HIGHLIGHT("in set_open");
            //cout << "elem.first" << elem.first << endl;
            //cout << "Connected to: " << elem.first << endl;
            lock_guard<mutex> lk(mutexes[elem.first]);
            socketConnectedMap[elem.first] = true;
            conditionsMap[elem.first].notify_all();
            //OUT("finished with open listener");
        });

        h->set_fail_listener([&]() {
            HIGHLIGHT("sio failed");
        });

        h->set_close_listener(std::bind(&TaasProvider::closeListen, this,std::placeholders::_1));

        h->connect(elem.second);
        //cout << "after connect " << elem.second << endl;
    }

    nlohmann::json jo;
    for (const auto &elem: providersMap_) {
        {
            unique_lock<mutex> lk(mutexes[elem.first]);
            conditionsMap[elem.first].wait(lk, [&]{return socketConnectedMap[elem.first];});
            while (!socketConnectedMap[elem.first]) {
                conditionsMap[elem.first].wait(lk);
            }
        }
        socketConnectedMap[elem.first] = false;
        jo["uuid"] = uuid;
        jo["clientID"] = P_.whoami() + 1;
        jo["nrOfClients"] = P_.nplayers();
        socketMap[elem.first].socket()->emit("register_triple", to_string(jo));
    }

    //waiting for point-to-point from transformshare
    for (const auto &elem: providersMap_) {
        //cout << "waiting for ptp: " << elem.first << endl;
        {
            unique_lock<mutex> lk(mutexes[elem.first]);
            while (!socketConnectedMap[elem.first]) {
                conditionsMap[elem.first].wait(lk);
            }
        }
    }
    //OUT("finished waiting ptp");

    // wait for broadcast from transformShare
    for (const auto &elem: providersMap_) {
        {
            unique_lock<mutex> lk(mutexes[elem.first]);
            while (!broadcastMap[elem.first]) {
                conditionsMap[elem.first].wait(lk);
            }
        }
        // OUT("before sync_close");
        //socketMap[elem.first].close(); //TODO: make parallel
        //socketMap[elem.first].clear_con_listeners();
        //OUT("after sync_close");
    }

    //OUT("end of communicatewithSP");
    checkAllOpeningsAndCreateTriples(transformedShareMsgMap, broadcastTransformShareMsgMap, count, a, b, c);
}

void TaasProvider::checkAllOpeningsAndCreateTriples(map<int, TransformedShareMsg> &transformedShareMsgMap,
                                    map<int, BroadcastTransformShareMsg> &broadcastTransformShareMsgMap,
                                    int nrOfTriples, list<Share> &a, list<Share> &b, list<Share> &c){

    map<int, bigint> aParts;
    map<int, bigint> bParts;
    map<int, bigint> cParts;

    map<int, bigint> c_aParts;
    map<int, bigint> c_bParts;
    map<int, bigint> c_cParts;

    map<int, bigint> a_aaParts;
    map<int, bigint> alpha_baParts;
    map<int, bigint> b_abParts;
    map<int, bigint> alpha_bbParts;
    map<int, bigint> c_acParts;
    map<int, bigint> alpha_bcParts;

    for (int i = 0; i < nrOfTriples; ++i) {
        for (const auto &elem: providersMap_) {
            auto key = elem.first;
            auto transformedShare = transformedShareMsgMap[key].transformedshare(i);
            auto broadcastShare = broadcastTransformShareMsgMap[key].broadcast(i);
            aParts[key] = transformedShare.share_a();
            bParts[key] = transformedShare.share_b();
            cParts[key] = transformedShare.share_c();
            c_aParts[key] = transformedShare.associated_c_a();
            c_bParts[key] = transformedShare.associated_c_b();
            c_cParts[key] = transformedShare.associated_c_c();
            a_aaParts[key] = broadcastShare.a_aa();
            alpha_baParts[key] = broadcastShare.alpha_ba();
            b_abParts[key] = broadcastShare.b_ab();
            alpha_bbParts[key] = broadcastShare.alpha_bb();
            c_acParts[key] = broadcastShare.c_ac();
            alpha_bcParts[key] = broadcastShare.alpha_bc();
        }


        auto aShare = shamirSs_.recoverSecret(aParts);
        auto bShare = shamirSs_.recoverSecret(bParts);
        auto cShare = shamirSs_.recoverSecret(cParts);

        auto c_a = shamirSs_.recoverSecret(c_aParts);
        auto c_b = shamirSs_.recoverSecret(c_bParts);
        auto c_c = shamirSs_.recoverSecret(c_cParts);

        auto a_aa = shamirSs_.recoverSecret(a_aaParts);
        auto alpha_ba = shamirSs_.recoverSecret(alpha_baParts);
        auto b_ab  = shamirSs_.recoverSecret(b_abParts);
        auto alpha_bb = shamirSs_.recoverSecret(alpha_bbParts);
        auto c_ac = shamirSs_.recoverSecret(c_acParts);
        auto alpha_bc = shamirSs_.recoverSecret(alpha_bcParts);

        aParts.clear();
        bParts.clear();
        cParts.clear();
        c_aParts.clear();
        c_bParts.clear();
        c_cParts.clear();

        a_aaParts.clear();
        alpha_baParts.clear();
        b_abParts.clear();
        alpha_bbParts.clear();
        c_acParts.clear();
        alpha_bcParts.clear();

        gfp macA = c_a;
        gfp macB = c_b;
        gfp macC = c_c;
        if (this->P_.whoami() == 0) {
            gfp a_aaElem = a_aa;
            gfp alpha_baElem = alpha_ba;
            a_aaElem.negate();
            a_aaElem.mul(alpha_baElem);
            macA.add(a_aaElem);

            gfp b_abElem = b_ab;
            gfp alpha_bbElem = alpha_bb;
            b_abElem.negate();
            b_abElem.mul(alpha_bbElem);
            macB.add(b_abElem);

            gfp c_acElem = c_ac;
            gfp alpha_bcElem = alpha_bc;
            c_acElem.negate();
            c_acElem.mul(alpha_bcElem);
            macC.add(c_acElem);
        }
        gfp fieldA_aa = a_aa;
        gfp fieldAlpha_ba = alpha_ba;
        gfp fieldAShare = aShare;

        gfp fieldB_ab = b_ab;
        gfp fieldAlpha_bb = alpha_bb;
        gfp fieldBShare = bShare;

        gfp fieldC_ac = c_ac;
        gfp fieldAlpha_bc = alpha_bc;
        gfp fieldCShare = cShare;

        macA += (fieldA_aa * macShare_) + (fieldAlpha_ba * fieldAShare);
        macB += (fieldB_ab * macShare_) + (fieldAlpha_bb * fieldBShare);
        macC += (fieldC_ac * macShare_) + (fieldAlpha_bc * fieldCShare);

        Share A;
        Share B;
        Share C;
        vector<gfp> s(1), mac(1);
        s[0] = aShare;
        mac[0] = macA;
        A.assign(P_.whoami(), s, mac);
        s[0] = bShare;
        mac[0] = macB;
        B.assign(P_.whoami(), s, mac);
        s[0] = cShare;
        mac[0] = macC;
        C.assign(P_.whoami(), s, mac);


        a.push_back(A);
        b.push_back(B);
        c.push_back(C);
    }

    //HIGHLIGHT("finished converting");

}

string TaasProvider::downloadCertificateEncryptAndEncode(string provider, const bigint &macShare) {
    //cout << "in cert function" << endl;
    const string certRelativPath = "/crt/ServerCert.crt";
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        string buffer;
        auto tmpurl = provider + certRelativPath;
        char *char_arr = &tmpurl[0];
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
        curl_easy_setopt(curl, CURLOPT_URL, char_arr);
        curl_easy_setopt(curl, CURLOPT_PRIVATE, char_arr);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);


        std::string fLine;
        std::istringstream f(buffer);
        std::getline(f, fLine);

        BIO *bioPublic = BIO_new(BIO_s_mem());
        BIO_write(bioPublic, buffer.c_str(), static_cast<int>(buffer.length()));

        X509 *x;
        x = PEM_read_bio_X509(bioPublic, nullptr, 0, nullptr);
        if (x == nullptr) {
            cout << "could not read prem" << endl;
        }
        EVP_PKEY *pubkey;
        pubkey = X509_get_pubkey(x);
        if (pubkey == nullptr) {
            cout << "couldnt get pubkey from x509" << endl;
        }
        RSA *rsa;
        rsa = EVP_PKEY_get1_RSA(pubkey);
        if (rsa == NULL) {
            cout << "rsa is null" << endl;
        }

        EVP_PKEY_CTX *ctx;
        string toEncrypt = macShare.get_str();
        //OUT(macShare);
        unsigned char *in = (unsigned char *) toEncrypt.c_str();
        unsigned char *out;
        size_t outlen, inlen;
        inlen = toEncrypt.length();
        ctx = EVP_PKEY_CTX_new(pubkey, nullptr);
        if (!ctx) {
            cout << "not ctx" << endl;
        }
        if (EVP_PKEY_encrypt_init(ctx) <= 0) {
            cout << "encrypt init error" << endl;
        }
        if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
            cout << "padding error" << endl;
        }

        /* Determine buffer length */
        if (EVP_PKEY_encrypt(ctx, NULL, &outlen, in, inlen) <= 0) {
            cout << "encrypt error" << endl;
        }
        out = (unsigned char *) OPENSSL_malloc(outlen);
        if (!out) {
            /* malloc failure */
            cout << "malloc error" << endl;
        }

        if (EVP_PKEY_encrypt(ctx, out, &outlen, in, inlen) <= 0) {
            cout << "encrypt error" << endl;
        }

        /* Encrypted data is outlen bytes written to buffer out */
        EVP_PKEY_free(pubkey);
        X509_free(x);

        auto base64Char = base64(out, outlen);
        string encodedString(base64Char);

        OPENSSL_free(out);
        return encodedString;
    }
    HIGHLIGHT("Error in downloadCertificateEncryptAndEncode");
    return "";
}


string TaasProvider::createUUID() {
    string line;
    if (P_.whoami() == 0) {
        auto ifs = std::ifstream("/proc/sys/kernel/random/uuid");
        getline(ifs, line);
        P_.send_all(line, 0, false);
    } else {
        P_.receive_from_player(0, line, 0, false);
    }
    return line;
}

void TaasProvider::downloadSingleServiceProviderNr(string url) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        string endpoint = "/ServiceProviderNr";
        string buffer;
        auto tmpurl = url + endpoint;
        char *char_arr = &tmpurl[0];
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
        curl_easy_setopt(curl, CURLOPT_URL, char_arr);
        curl_easy_setopt(curl, CURLOPT_PRIVATE, char_arr);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
        int number = stoi(buffer);
        std::lock_guard<mutex> guard(providerMapMutex_);
        providersMap_[number] = url;
    }
}


void TaasProvider::createMacShare() {
    macShare_.randomize(PRG_);
}

void TaasProvider::downloadAllServiceProviderNr(vector<string> tripleProviders) {
    std::thread threads[tripleProviders.size()];
    for (int i = 0; i < tripleProviders.size(); ++i) {
        threads[i] = thread(&TaasProvider::downloadSingleServiceProviderNr, this, tripleProviders[i]);
    }
    for (auto &t: threads) t.join();
}

size_t TaasProvider::write_cb(char *data, size_t n, size_t l, void *userp) {
    /* take care of the data here, ignored in this example */
    (void) data;
    (void) userp;
    ((std::string *) userp)->append((char *) data, n * l);
    return n * l;
}

char* TaasProvider::base64(const unsigned char *input, int length) {
    const auto pl = 4*((length+2)/3);
    auto output = reinterpret_cast<char *>(calloc(pl+1, 1)); //+1 for the terminating null that EVP_EncodeBlock adds on
    const auto ol = EVP_EncodeBlock(reinterpret_cast<unsigned char *>(output), input, length);
    if (pl != ol) { std::cerr << "Whoops, encode predicted " << pl << " but we got " << ol << "\n"; }
    return output;
}




