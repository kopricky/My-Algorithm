//約数の個数をO(n^(1/3))で求めるテク(意味ある？)
#include "../header.hpp"

const vector<ll> numset = {2,3,5,7,11,13,17,19,23,29,31,37};

ll modpow(__int128 x, ll k, ll m){
    __int128 res = 1;
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
    for(ll a : numset){
        if(a == n){
          return true;
        }
        if(modpow(a,d,n) != 1){
            bool ok = true;
            for(ll r=0;r<s;r++){
                if(modpow(a,d*(1LL << r),n) == n-1){
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

ll count_devisor(ll N){
    ll res = 1;
	for(ll i=2;i*i*i<=N;i++){
		int cnt = 0;
		while(N%i == 0){
			cnt++;
			N /= i;
		}
        res *= (cnt+1);
	}
    if(N != 1){
        if(check(N)){
            res *= 2;
        }else{
            ll val = (ll)floor((long double)sqrt(N));
            if(val*val == N){
                res *= 3;
            }else{
                res *= 4;
            }
        }
    }
	return res;
}
