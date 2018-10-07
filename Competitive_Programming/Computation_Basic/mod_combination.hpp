#include "../header.hpp"

#define MAX_N 100005
#define MOD 1000000007

int inv[MAX_N],fac[MAX_N],finv[MAX_N];

void make()
{
	fac[0] = fac[1] = 1;
	finv[0] = finv[1] = 1;
	inv[1] = 1;
	for(int i=2;i<MAX_N;i++){
		inv[i] = MOD - (long long)inv[MOD%i] * (MOD/i) % MOD;
		fac[i] = (long long)fac[i-1] * i % MOD;
		finv[i] = (long long)finv[i-1] * inv[i] % MOD;
	}
}

int comb(int a,int b)
{
	if(a < b){
		return 0;
	}
	return fac[a] * ((long long)finv[b] * finv[a-b] % MOD) % MOD;
}
