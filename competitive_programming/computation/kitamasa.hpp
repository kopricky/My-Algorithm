#include "../header.hpp"
// きたまさ法を用いてk項間漸化式の第n項を計算する。O(k^2logn)
// aが係数, xが初期値
// 第1引数 : A[n] = a[0] * A[n-k] + a[1] * A[n-k+1] + ... + a[k-1] * A[n-1]
// 第2引数 : A[0] = x[0], A[1] = x[1], ... , A[k-1] = x[k-1]

#define MOD 1000000007

void rec(int k, int n, vector<int>& a, vector<int>& t, vector<int>& c)
{
	if(n < k){
		c[n] = 1;
	}else{
		vector<int> b(k);
		rec(k,n/2,a,t,b);
		fill(t.begin(), t.end(), 0);
		for (int i = 0; i < k; ++i) {
			for (int j = 0; j < k; ++j) {
				(t[i + j + (n & 1)] += (ll)b[i] * b[j] % MOD) %= MOD;
			}
		}
		for (int i = 2 * k - 1; i >= k; --i) {
			for (int j = 0; j < k; j++) {
				(t[i - k + j] += (ll)a[j] * t[i] % MOD) %= MOD;
			}
		}
		for (int i = 0; i < k; ++i) c[i] = t[i];
	}
}

int kitamasa(vector<int>& a, vector<int>& x, int n)
{
	int k = (int)a.size();
	vector<int> t(2*k+1);
	vector<int> res(k);
	rec(k,n,a,t,res);
	int ans = 0;
	for (int i = 0; i < (int)x.size(); ++i) {
		(ans += (ll)res[i] * x[i] % MOD) %= MOD;
	}
	return ans;
}
