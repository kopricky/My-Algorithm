struct Miller{
    const vector<ll> v = {2,7,61}; // < 4,759,123,141 ≒ 4×10^9
    //const vector<ll> v = {2,3,5,7,11,13,17}; < 341,550,071,728,321 ≒　3*10^14
    //const vector<ll> v = {2,3,5,7,11,13,17,19,23,29,31,37}; 2^64までいける
    // x^k (mod m)
    ll modpow(ll x, ll k, ll m){
        ll res = 1;
        while(k){
          if(k & 1){
            res = res * x % m;
          }
          k /= 2;
          x = x * x % m;
        }
        return res;
    }
    // check if n is prime
    bool check(ll n){
        if(n < 2){
          return false;
        }
        ll d = n - 1;
        ll s = 0;
        while(d % 2 == 0){
            d /= 2;
            s++;
        }
        for(ll a : v){
            if(a == n){
              return true;
            }
            if(modpow(a,d,n) != 1){
              bool ok = true;
              for(ll r=0;r<s;r++){
                if( modpow(a,d*(1LL << r),n) == n-1){
                  ok = false;
                  break;
                }
              }
              if(ok){
                return false;
              }
            }
        }
        return true;
    }
};
