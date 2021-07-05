#include "../header.hpp"

#define MAX_N 100005
#define MOD 1000000007

unsigned inv[MAX_N], fac[MAX_N], finv[MAX_N];

unsigned int add(unsigned int x, unsigned int y){
	return (x + y >= MOD) ? (x + y - MOD) : (x + y);
}

unsigned int sub(unsigned int x, unsigned int y){
	return (x < y) ? (x + MOD - y) : (x - y);
}

unsigned int mul(unsigned int x, unsigned int y){
	return (unsigned long long)x * y % MOD;
}

void make()
{
	fac[0] = fac[1] = 1;
	finv[0] = finv[1] = 1;
	inv[1] = 1;
	for(int i = 2; i < MAX_N; ++i){
		inv[i] = MOD - mul(inv[MOD % i], (MOD / i));
		fac[i] = mul(fac[i - 1], i);
		finv[i] = mul(finv[i - 1], inv[i]);
	}
}

// solve() を呼び出す前に必ず make() を行っておく！！
// deg + 1 個の f(i) = val[i] (i = 0,..., deg) の情報から関数 f を復元し, f(num) を返す
// すべて MOD を法として考えていることに注意
unsigned int solve(int deg, long long num, vector<int>& val)
{
	if((num %= MOD) < 0) num += MOD;
	if(num <= deg) return val[num];
	unsigned int ue = 1;
	vector<unsigned int> lf(deg + 2), rg(deg + 2);
	lf[0] = rg[deg + 1] = 1;
	for(int i = deg; i >= 0; --i){
		rg[i] = mul(rg[i + 1], num - i);
	}
	unsigned int ans = 0;
	for(int i = 0; i <= deg; ++i){
		unsigned int r = mul(finv[deg-i], finv[i]);
		if((deg - i) % 2) r = MOD - r;
		ans = add(ans, mul(mul(lf[i], rg[i + 1]), mul(r, val[i])));
		lf[i + 1] = mul(lf[i], num - i);
	}
	return ans;
}
