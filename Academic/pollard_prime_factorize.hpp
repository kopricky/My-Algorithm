
// (注) バグってるかも
// pollardのρ法のBrentによる高速化
// Brent 1980を参照
// 自然数の素因数分解を高速に行う(一般のポラードのρ法よりも24％速い)

#include "header.hpp"
#define ll long long

ll gcd(ll a,ll b)
{
    if(a == 0 || b == 0){
        return max(a,b);
    }
    if(a % b == 0){
        return b;
    }else{
        return gcd(b,a%b);
    }
}

unsigned RV(){
    static unsigned x = 123456789, y = 362436069, z = 521288629, w = 86675123;
    unsigned t = (x^(x<<11));
    x = y,y = z,z = w;
    return (w = (w^(w>>19))^(t^(t>>8)));
}

ll func(ll y, ll n, ll c){
    return ((__int128)y * y + c) % n;
}

ll pollard(ll n){
    ll c = RV() % (n-1) + 1, y = RV() % (n-1) + 1, m = RV() % (n-1) + 1;
    ll g = 1, r = 1, q = 1;
    ll x, ys;
    do {
        x = y;
        rep(i, r){
            y = func(y, n, c);
        }
        ll k = 0;
        do {
            ys = y;
            rep(i, min(m, r-k)){
                y = func(y, n, c);
                q = (__int128)q * abs(x-y) % n;
            }
            g = gcd(q, n);
            k += m;
        }while(k < r && g <= 1);
        r *= 2;
    }while(g <= 1);
    if(g == n){
        do {
            ys = func(ys, n, c);
            g = gcd(abs(x-ys), n);
        }while(g <= 1);
    }
    if(g == n){
        return -1;
    }else{
        return g;
    }
}

vector<pair<ll,int> > prime_factorize(ll n)
{
    vector<pair<ll, int> > res;
    if(n % 2 == 0){
        int cnt = 0;
        do {
            cnt++;
            n /= 2;
        }while(n % 2 == 0);
        res.emplace_back(2, cnt);
    }
    const int LOOP = 10000;
    while(n > 1){
        ll d;
        rep(itr, LOOP){
            d = pollard(n);
            if(d >= 0) break;
        }
        if(d < 0){
            res.emplace_back(n, 1);
            break;
        }else{
            int cnt = 0;
            do {
                cnt++;
                n /= d;
            }while(n % d == 0);
            res.emplace_back(d, cnt);
        }
    }
    sort(res.begin(),res.end());
    return res;
}
