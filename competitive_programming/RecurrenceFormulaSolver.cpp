// typedef ll long long

// きたまさ法を用いてk項間漸化式の第n項を計算する。O(k^2logn)
// aが係数, xが初期値
// 第1引数 : A[n] = a[0] * A[n-k] + a[1] * A[n-k+1] + ... + a[k-1] * A[n-1]
// 第2引数 : A[0] = x[0], A[1] = x[1], ... , A[k-1] = x[k-1]

ll kitamasa(vector<ll> a, vector<ll> x, ll n, ll mod) {
	int k = (int)a.size();
	vector<ll> t(2*k+1);
	function<vector<ll>(ll)> rec = [&](ll n) {
		std::vector<ll> c(k);
		if(n < k){
            c[n] = 1;
        }else{
			vector<ll> b = rec(n/2);
			fill(t.begin(), t.end(), 0LL);
			for (int i = 0; i < k; ++i) {
				for (int j = 0; j < k; ++j) {
					(t[i + j + (n & 1)] += b[i] * b[j] % mod) %= mod;
				}
			}
			for (int i = 2 * k - 1; i >= k; --i) {
				for (int j = 0; j < k; j++) {
					(t[i - k + j] += a[j] * t[i] % mod) %= mod;
				}
			}
			for (int i = 0; i < k; ++i) c[i] = t[i];
		}
		return c;
	};
	vector<ll> c = rec(n);
	ll ans = 0;
	for (int i = 0; i < (int)x.size(); ++i) {
		(ans += c[i] * x[i] % mod) %= mod;
	}
	return ans;
}
