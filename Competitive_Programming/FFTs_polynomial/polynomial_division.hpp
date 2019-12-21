#include "../header.hpp"

#define MOD 924844033
#define root 5

unsigned int add(const unsigned int x, const unsigned int y)
{
	return (x + y < MOD) ? x + y : x + y - MOD;
}

unsigned int sub(const unsigned int x, const unsigned int y)
{
	return (x >= y) ? (x - y) : (MOD - y + x);
}

unsigned int mul(const unsigned int x, const unsigned int y)
{
	return (unsigned long long)x * y % MOD;
}

unsigned int power(unsigned int x, unsigned int n)
{
    unsigned int res = 1;
    while(n > 0) {
        if(n & 1) { res = mul(res, x); }
        x = mul(x, x);
        n >>= 1;
    }
    return res;
}

unsigned int inverse(const unsigned int x)
{
	return power(x, MOD - 2);
}

void ntt(vector<int>& a, const bool rev = false)
{
    unsigned int i,j,k,s,t,v,w,wn;
    const unsigned int size = a.size();
    const unsigned int height = (unsigned int)log2(2 * size - 1);
    for(i = 0; i < size; i++) {
        j = 0;
        for(k = 0; k < height; k++) { j |= (i >> k & 1) << (height - 1 - k); }
        if(i < j) swap(a[i], a[j]);
    }
    for(i = 1; i < size; i *= 2) {
        w = power(root, (MOD - 1) / (i * 2));
        if(rev) w = inverse(w);
        for(j = 0; j < size; j += i * 2) {
            wn = 1;
            for(k = 0; k < i; k++) {
                s = a[j + k + 0], t = mul(a[j + k + i], wn);
                a[j + k + 0] = add(s, t);
                a[j + k + i] = add(s, MOD - t);
                wn = mul(wn, w);
            }
        }
    }
    if(rev){
        v = inverse(size);
        for (i = 0; i < size; i++) { a[i] = mul(a[i], v); }
    }
}

vector<int> convolute(const vector<int>& a, const vector<int>& b, int asize, int bsize, int _size)
{
    const int size = asize + bsize - 1;
    int t = 1;
    while (t < size) { t <<= 1; }
    vector<int> A(t, 0), B(t, 0);
    for(int i = 0; i < asize; i++) { A[i] = a[i]; }
    for(int i = 0; i < bsize; i++) { B[i] = b[i]; }
    ntt(A), ntt(B);
    for(int i = 0; i < t; i++) { A[i] = mul(A[i], B[i]); }
    ntt(A, true);
    A.resize(_size);
    return A;
}

vector<int> polynomial_inverse(const vector<int>& a, int r){
	vector<int> h = {(int)inverse(a[0])};
	int t = 1;
	for(int i = 0; t < r; ++i){
		t <<= 1;
		vector<int> res = convolute(a, convolute(h, h, t / 2, t / 2, t), min((int)a.size(), t), t, t);
		for(int j = 0; j < t; ++j){
			res[j] = MOD - res[j];
			if(j < t / 2) res[j] = add(res[j], mul(2, h[j]));
		}
		swap(h, res);
	}
	h.resize(r);
	return h;
}

pair<vector<int>, vector<int> > polynomial_division(const vector<int>& a, const vector<int>& b)
{
	const int n = a.size() - 1, m = b.size() - 1;
	assert(b[m] != 0);
	if(n < m) return {vector<int>(n - m + 1, 0), a};
	vector<int> reva(n + 1), revb(m + 1);
	for(int i = 0; i <= n; ++i) reva[n - i] = a[i];
	for(int i = 0; i <= m; ++i) revb[m - i] = b[i];
    vector<int> inv_revb = polynomial_inverse(revb, n - m + 1);
	vector<int> res = convolute(reva, inv_revb, n - m + 1, n - m + 1, n - m + 1);
	vector<int> q(n - m + 1), r;
	for(int i = 0; i < n - m + 1; ++i) q[i] = res[n - m - i];
	vector<int> qb = convolute(q, b, n - m + 1, m + 1, n + 1);
	bool first = false;
	for(int i = n; i >= 0; --i){
		const int val = sub(a[i], qb[i]);
		if(!first && val > 0){
			first = true, r.resize(i + 1);
		}
		if(first) r[i] = val;
	}
	return {q, r};
}