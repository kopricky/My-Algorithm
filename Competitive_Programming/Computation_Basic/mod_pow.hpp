#include "../header.hpp"

#define MOD 1000000007

ll mod_pow(ll a,ll b)
{
    a %= MOD;
    ll res = 1;
    while(b){
        if(b & 1){
            res = res * a % MOD;
        }
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}
