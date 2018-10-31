#include "../header.hpp"

//離散対数問題に対するアルゴリズム
long long mod_pow(long long a, long long b, long long m)
{
    a %= m;
    long long res = 1;
    while(b){
        if(b & 1){
            res = res * a % m;
        }
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

void extgcd(long long a, long long b, long long& x, long long& y)
{
    if(b != 0){
        extgcd(b,a%b,y,x);
        y -= (a/b)*x;
    }else{
        x = 1;
        y = 0;
    }
}

long long mod_inverse(long long a, long long m)
{
    long long x,y;
    extgcd(a,m,x,y);
    return (m + x % m) % m;
}

//g^x ≡ y (mod p) の解xを求める (O(p^(1/2)logp))
long long BabyStep_GiantStep(long long g, long long y, long long p)
{
    long long m = floor(sqrt(p));
    map<long long,long long> mp;
    long long val = 1;
    for(int i = 0; i < m; i++){
        mp[val] = i;
        val = val * g % p;
    }
    long long invgm = mod_pow(mod_inverse(g,p), m, p);
    long long giant = y;
    for(int i = 0; i < m; i++){
        if(mp.find(giant) != mp.end()){
            return i*m + mp[giant];
        }else{
            giant = giant * invgm % p;
        }
    }
    //解なし
    return -1;
}
