//
// Created by sguggi on 24.08.22.
//

#include <map>
#include "ShamirSS.h"

ShamirSS::ShamirSS(bigint prime, PRNG PRNG) : prime_(prime), PRNG_(PRNG) {
    int zero = 0;
    bigintFromBytes(bigint0_, static_cast<uint8_t *>(static_cast<void *>(&zero)), sizeof(uint8_t));
}


map<int, bigint> ShamirSS::split(int n, int k, bigint secret, vector<int> &pointToEvaluate) {

    for (auto x: pointToEvaluate) {
        bigint tmp;
        bigintFromBytes(tmp, static_cast<uint8_t *>(static_cast<void *>(&x)), sizeof(uint8_t));
        if (check_bigint_zero(tmp)) {
            cout << "Something in ShamirSS is 0" << endl;
            exit(1);
        }
    }

    bigint coeffs[k];
    coeffs[0] = secret;
    for (int i = 1; i < k; ++i) {
        bigint tmp;
        do {
            tmp = randomBnd(PRNG_, prime_);
        } while (check_bigint_zero(tmp));
        coeffs[i] = tmp;
    }
    map<int, bigint> shares;
    int zero = 0;
    for (auto x: pointToEvaluate) {
        bigint share;
        bigintFromBytes(share, static_cast<uint8_t *>(static_cast<void *>(&zero)), sizeof(uint8_t));
        for (int j = 0; j < k; ++j) {
            bigint tmp;
            bigint jBigint;
            bigintFromBytes(tmp, static_cast<uint8_t *>(static_cast<void *>(&x)), sizeof(uint8_t));
            bigintFromBytes(jBigint, static_cast<uint8_t *>(static_cast<void *>(&j)), sizeof(uint8_t));
            bigint evaluated = powerMod(tmp, jBigint, prime_);
            share = share + (coeffs[j] * evaluated);
        }
        shares[x] = share;
    }
    return shares;
}

bigint ShamirSS::recoverSecret(map<int, bigint> parts) {
    int zero = 0;
    int one = 1;
    bigint recovered;
    bigintFromBytes(recovered, static_cast<uint8_t *>(static_cast<void *>(&zero)), sizeof(uint8_t));
    vector<int> x_s;
    for (const auto &elem: parts) {
        x_s.push_back(elem.first);
    }
    for (int j = 0; j < x_s.size(); ++j) {
        bigint tmp;
        bigintFromBytes(tmp, static_cast<uint8_t *>(static_cast<void *>(&one)), sizeof(uint8_t));
        for (int m = 0; m < x_s.size(); ++m) {
            if (j == m) continue;
            bigint divided = divmod(x_s[m], x_s[m] - x_s[j]);
            tmp = tmp*divided;
            tmp = mod(tmp, prime_);
        }
        auto y = parts[x_s[j]];
        bigint multiplied = tmp*y;
        multiplied = mod(multiplied, prime_);
        recovered = recovered+multiplied;
        recovered = mod(recovered, prime_);
    }
    return recovered;
}

bigint ShamirSS::divmod(int num, int den) {
    bigint numBigInt;
    bigint denBigInt;
    numBigInt = num;
    denBigInt = den;
    bigint result;
    invMod(result,denBigInt,prime_);
    return numBigInt*result;

}

bool ShamirSS::check_bigint_zero(bigint toCheck) {

    return mod(toCheck, prime_) == bigint0_;

}
