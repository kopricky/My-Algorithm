#include "../header.hpp"

// a↑↑b % mod を求めるプログラム

int euler_function(int n)
{
	int res = n;
	for(int i = 2; i * i <= n; ++i){
		if(n % i == 0){
			res = res / i * (i - 1);
			while(n % i == 0) n /= i;
		}
	}
	if(n != 1) res = res / n * (n - 1);
	return res;
}

int mod_pow(int a, int b, int mod)
{
    int res = 1;
    while(b){
        if(b & 1){
            res = (long long)res * a % mod;
        }
        a = (long long)a * a % mod;
        b >>= 1;
    }
    return res;
}

int rec(long long a, int b, int mod)
{
    if(a == 1 || b == 0) return 1;
    if(mod == 1) return 0;
    int ans = rec(a, b-1, euler_function(mod)), ad = mod;
    if(ans <= 30 && a < mod && pow((double)a, ans) < mod) ad = 0;
    return mod_pow(a % mod, ans, mod) + ad;
}

inline int tetration(long long a, int b, int mod)
{
    return rec(a, b, mod) % mod;
}