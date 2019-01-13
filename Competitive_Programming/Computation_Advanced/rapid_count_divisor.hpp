//約数の個数をO(n^(1/3))で求めるテク(実装はしてみたが意味あるかな？)
#include "../header.hpp"

const vector<long long> numset = {2,3,5,7,11,13,17,19,23,29,31,37};

long long modpow(__int128 x, long long k, long long m){
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
bool check(long long n){
    if(n < 2){
      return false;
    }
    long long d = n - 1;
    long long s = 0;
    while(d % 2 == 0){
        d /= 2;
        s++;
    }
    for(long long a : numset){
        if(a == n){
          return true;
        }
        if(modpow(a,d,n) != 1){
            bool ok = true;
            for(long long r=0;r<s;r++){
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

long long count_devisor(long long N){
    long long res = 1;
	for(long long i=2;i*i*i<=N;i++){
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
            long long val = (long long)floor((long double)sqrt(N));
            if(val*val == N){
                res *= 3;
            }else{
                res *= 4;
            }
        }
    }
	return res;
}
