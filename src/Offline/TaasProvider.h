//
// Created by sguggi on 17.08.22.
//

#ifndef SCALE_MAMBA_TAASPROVIDER_H
#define SCALE_MAMBA_TAASPROVIDER_H
#include "System/Player.h"
#include "sio_client.h"
#include "sio_message.h"
#include "sio_socket.h"
#include <iostream>
#include <mutex>
#include <string>
#include "Math/gfp.h"
#include "Tools/ShamirSS.h"
#include <curl/curl.h>
#include <thread>
#include <cstddef>
#include <cstring>
#include "openssl/pem.h"
#include "Tools/base64.h"
#include "LSSS/MSP.h"
#include <fstream>
#include <cstddef>
#include <utility>
#include <mutex>
#include <condition_variable>
#include "Tools/json.h"
#include "ptp_and_brdcst_msg.pb.h"
#include "randVal_msg.pb.h"

using namespace std;
class TaasProvider
{
  const Player P_;
  gfp macShare_;
  string ledgerAddress_;
  PRNG PRG_;
  bigint prime_;
  ShamirSS shamirSs_;
  map<int, string> providersMap_;
  mutex providerMapMutex_;
  enum RequestChoice { TRIPLES, RND_VALUE };

public:
  TaasProvider(Player &P, vector<string> availableTripleProviders, string ledgerAddress, bigint prime);
  void getTriples(int nrOfTriples);

private:

  string createUUID();
  static string downloadCertificateEncryptAndEncode(string provider, const bigint& macShare);
  void downloadAllServiceProviderNr(vector<string> tripleProviders);
  map<int, bigint> splitMacShare();
  void createMacShare();
  void downloadSingleServiceProviderNr(string url);
  void chooseTripleProviders(vector<string> availableTripleProviders);
  static size_t write_cb(char *data, size_t n, size_t l, void *userp);
  static size_t write_cert(char *data, size_t n, size_t l, void *userp);
  void sendRequestToLedger(int count, const map<int, string> &encryptedShareParts, const string& uuid, RequestChoice choice);
  void communicateWithSP(int count, string uuid, RequestChoice choice);
};

#endif // SCALE_MAMBA_TAASPROVIDER_H
