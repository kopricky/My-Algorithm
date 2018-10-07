#include "../header.hpp"

#define MOD 1000000007

long long mod_pow(long long a,long long b)
{
    a %= MOD;
    long long res = 1;
    while(b){
        if(b & 1){
            res = res * a % MOD;
        }
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}
