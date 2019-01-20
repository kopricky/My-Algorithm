#include "../header.hpp"

#define MOD 1000000007

// コドフォなどで __int128 型が使えないときに long double を用いて long long 同士の積の剰余を求める裏技
long long mul(long long a, long long b){
    long long res = (a * b - (long long)((long double)a / MOD * b) * MOD) % MOD;
    return res < 0 ? res + MOD : res;
}