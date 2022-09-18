//
// Created by sguggi on 24.08.22.
//

#ifndef SCALE_MAMBA_SHAMIRSS_H
#define SCALE_MAMBA_SHAMIRSS_H
#include "Math/bigint.h"

class ShamirSS
{
  bigint prime_;
  PRNG PRNG_;
  bigint bigint0_;
public:
  ShamirSS(bigint prime, PRNG PRNG);
  map<int, bigint> split(int n, int k, bigint secret, vector<int> &pointToEvaluate);
  bigint recoverSecret(map<int, bigint> parts);

private:
    bool check_bigint_zero(bigint toCheck);
    bigint divmod(int num, int den);
};

#endif // SCALE_MAMBA_SHAMIRSS_H
