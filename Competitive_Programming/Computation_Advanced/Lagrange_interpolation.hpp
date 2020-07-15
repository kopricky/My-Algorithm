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

inline int mod_pow(int a, int b)
{
	int res = 1;
	while(b){
		if(b & 1){
			res = (long long)res * a % MOD;
		}
		a = (long long)a * a % MOD;
		b >>= 1;
	}
	return res;
}

inline int add(int x,int y)
{
	return (x + y)%MOD;
}

inline int sub(int x,int y)
{
	return (x+MOD-y)%MOD;
}

inline int mul(int x,int y)
{
	return (long long)x*y%MOD;
}

void extgcd(int a,int b, int& x,int& y)
{
	if(b != 0){
		extgcd(b,a%b,y,x);
		y -= (a/b)*x;
	}else{
		x = 1;
		y = 0;
	}
}

int mod_inverse(int a,int m)
{
	int x,y;
	extgcd(a,m,x,y);
	return (m + x % m) % m;
}

// deg + 1 個の f(i) = val[i] (i = 0,..., deg) の情報から関数 f を復元し, f(num) を返す
// すべて MOD を法として考えていることに注意
int solve(int deg, long long num, vector<int>& val)
{
	if(num <= deg) return val[num];
	int ue = 1;
	for(int i = 0; i < deg + 1; i++){
		ue = mul(ue, sub(num % MOD, i));
	}
	int ans = 0;
	for(int i = 0; i < deg + 1; i++){
		int r1 = mul(ue, mul(mod_inverse(sub(num % MOD, i), MOD), val[i]));
		int r2 = mul(finv[deg-i], finv[i]);
		if((deg - i) % 2){
			r2 = MOD - r2;
		}
		ans = add(ans, mul(r1, r2));
	}
	return ans;
}
