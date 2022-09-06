//
// Created by sguggi on 17.08.22.
//


#include "TaasProvider.h"

#define HIGHLIGHT(__O__) std::cout<<"\e[1;31m"<<__O__<<"\e[0m"<<std::endl
#define OUT(__O__) cout << __O__ << endl;


TaasProvider::TaasProvider(Player &P, vector<string> availableTripleProviders, string ledgerAddress, bigint prime)
        : P_(P), ledgerAddress_(std::move(ledgerAddress)), PRG_(P.G), prime_(std::move(prime)), shamirSs_(prime_, PRG_) {
    PRG_.ReSeed(0);
    createMacShare();
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
    cout << "macShare: " << macShare_ << endl;
    cout << "printing macSahres" << endl;
    for (const auto &elem: macShares) {
        std::cout << elem.first << " " << elem.second << "\n";
    }
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

void TaasProvider::getTriples(int nrOfTriples) {
    auto uuid = createUUID();
    cout << "before splitMachsare" << endl;
    auto partMap = splitMacShare();
    cout << "after splitMachsare" << endl;
    map<int, string> encryptedMacShareParts;
    for (const auto &elem: partMap) {
        cout << "in part map" << endl;
        cout << "first: " << elem.first << endl;
        auto encryptedPart = downloadCertificateEncryptAndEncode(providersMap_[elem.first], partMap[elem.first]);
        encryptedMacShareParts[elem.first] = encryptedPart;
    }
    sendRequestToLedger(nrOfTriples, encryptedMacShareParts, uuid, TRIPLES);
    cout << "get triples finished " << endl;

}

void TaasProvider::sendRequestToLedger(int count, const map<int, string>& encryptedShareParts, const string& uuid, RequestChoice choice ) {
    sio::client h;
    bool finished = false;
    bool retVal = false;
    condition_variable_any cond;
    mutex mu;
    h.set_open_listener([&]() {
        cout << "in set open listener" << endl;
    });
    h.socket()->on("reserve_response", [&](sio::event& ev) {
        cout << "in resever_response" << endl;
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
    j["playerID"] = P_.whoami()+1;
    j["CP"] = P_.nplayers();
    j["sp_r"] = "";
    j["room"] = uuid;

    h.socket()->emit("reserve", to_string(j));

    unique_lock<mutex> lk(mu);
    cond.wait(mu, [&finished] {return finished;});
    lk.unlock();
    if (!retVal) {
        HIGHLIGHT("Error from ledger");
        exit(-1);
    }
    h.sync_close();
    mu.lock();
    mu.unlock();
}

void TaasProvider::communicateWithSP(int count, string uuid, RequestChoice choice) {
    map<int, sio::client*> socketMap;
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
        sio::client h;

        socketMap[elem.first] = &h;

        h.socket()->on("transformshare_point_to_point_rnd", [&](sio::event& ev) {
            cout << "In transformPointToPoint rnd" << endl;
            auto msg = ev.get_message()->get_string();
            TransformedShareMsgRndVal transformedShareMsg;
            bool error = transformedShareMsg.ParseFromString(msg);
            if (!error) {
                cout << "Failed parsing transformedshareMsg" << endl;
            }
            transformedShareRndMsgMap[transformedShareMsg.coeff()] = transformedShareMsg;

            unique_lock<mutex> lk(mutexes[elem.first]);
            socketConnectedMap[elem.first] = true;
            conditionsMap[elem.first].notify_all();
        });

        h.socket()->on("transformshare_point_to_point_rnd", [&](sio::event& ev) {
            cout << "In transformPointToPoint Triple" << endl;
            auto msg = ev.get_message()->get_string();
            TransformedShareMsg transformedShareMsg;
            bool error = transformedShareMsg.ParseFromString(msg);
            if (!error) {
                cout << "Failed parsing transformedshareMsg" << endl;
            }
            transformedShareMsgMap[transformedShareMsg.coeff()] = transformedShareMsg;

            unique_lock<mutex> lk(mutexes[elem.first]);
            socketConnectedMap[elem.first] = true;
            conditionsMap[elem.first].notify_all();
        });

        h.socket()->on("transformshare_broadcast_triple", [&](sio::event& ev) {
            cout << "In transform share broadcast triple" << endl;
            auto msg = ev.get_message()->get_string();
            BroadcastTransformShareMsg broadcastTransformShareMsg;
            bool error = broadcastTransformShareMsg.ParseFromString(msg);
            if (!error) {
                cout << "Failed parsing broadcastTransformShareMsg" << endl;
            }
            broadcastTransformShareMsgMap[broadcastTransformShareMsg.coeff()] = broadcastTransformShareMsg;

            unique_lock<mutex> lk(mutexes[elem.first]);
            broadcastMap[elem.first] = true;
            conditionsMap[elem.first].notify_all();
        });

        h.socket()->on("transformshare_broadcast_rnd", [&](sio::event& ev) {
            cout << "In transform share broadcast rnd" << endl;
            auto msg = ev.get_message()->get_string();
            BroadcastTransformShareRndValMsg broadcastTransformShareMsg;
            bool error = broadcastTransformShareMsg.ParseFromString(msg);
            if (!error) {
                cout << "Failed parsing broadcastTransformShareMsg" << endl;
            }
            broadcastTransformShareRndMsgMap[broadcastTransformShareMsg.coeff()] = broadcastTransformShareMsg;

            unique_lock<mutex> lk(mutexes[elem.first]);
            broadcastMap[elem.first] = true;
            conditionsMap[elem.first].notify_all();
        });

        h.set_open_listener([&]() {
            cout << "Connected to: " << elem.first << endl;
            unique_lock<mutex> lk(mutexes[elem.first]);
            socketConnectedMap[elem.first] = true;
            conditionsMap[elem.first].notify_all();
        });

        h.connect(elem.second);
    }
    cout << "after listen for loop" << endl;

    nlohmann::json jo;
    for (const auto &elem: providersMap_) {
        {
            unique_lock<mutex> lk(mutexes[elem.first]);
            while (!socketConnectedMap[elem.first]) {
                conditionsMap[elem.first].wait(lk);
            }
        }
        socketConnectedMap[elem.first] = false;
        jo["uuid"] = uuid;
        jo["clientID"] = P_.whoami()+1;
        jo["nrOfClients"] = P_.nplayers();
        cout << "before emitp" << endl;
        socketMap[elem.first]->socket()->emit("register_triple", to_string(jo));
        cout << "after emitp" << endl;
    }

}

string TaasProvider::downloadCertificateEncryptAndEncode(string provider, const bigint& macShare) {
    cout << "in cert function" << endl;
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

        cout << "before bio" << endl;

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

        cout << "here bevore evp" << endl;
        EVP_PKEY_CTX *ctx;
        string toEncrypt = macShare.get_str();
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

        out = (unsigned char*)OPENSSL_malloc(outlen);

        if (!out) {
            /* malloc failure */
            cout << "malloc error" << endl;
        }

        if (EVP_PKEY_encrypt(ctx, out, &outlen, in, inlen) <= 0) {
            cout << "encrypt error" << endl;
        }

        /* Encrypted data is outlen bytes written to buffer out */
        //cout << out << endl;
        EVP_PKEY_free(pubkey);
        X509_free(x);

        string unencoded(reinterpret_cast<char*>(out));
        auto encoded = base64_encode(unencoded);
        return encoded;
    }
    cout << "Error in downloadCertificateEncryptAndEncode" << endl;
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


