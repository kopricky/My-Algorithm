#include "../header.hpp"

#define MOD 998244353
#define root 3

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

unsigned int mod_pow(unsigned int x, unsigned int n)
{
    unsigned int res = 1;
    while(n > 0){
        if(n & 1){ res = mul(res, x); }
        x = mul(x, x);
        n >>= 1;
    }
    return res;
}

unsigned int inverse(const unsigned int x)
{
	return mod_pow(x, MOD - 2);
}

void ntt(vector<int>& a, const bool rev = false)
{
    unsigned int i, j, k, l, p, q, r, s;
    const unsigned int size = a.size();
	if(size == 1) return;
	vector<int> b(size);
	r = rev ? (MOD - 1 - (MOD - 1) / size) : (MOD - 1) / size;
	s = mod_pow(root, r);
	vector<unsigned int> kp(size / 2 + 1, 1);
	for(i = 0; i < size / 2; ++i) kp[i + 1] = mul(kp[i], s);
	for(i = 1, l = size / 2; i < size; i <<= 1, l >>= 1){
		for(j = 0, r = 0; j < l; ++j, r += i){
			for(k = 0, s = kp[i * j]; k < i; ++k){
				p = a[k + r], q = a[k + r + size / 2];
				b[k + 2 * r] = add(p, q);
				b[k + 2 * r + i] = mul(sub(p, q), s);
			}
		}
		swap(a, b);
	}
    if(rev){
        s = inverse(size);
        for(i = 0; i < size; i++){ a[i] = mul(a[i], s); }
    }
}

vector<int> convolute(const vector<int>& a, const vector<int>& b, int asize, int bsize, int _size)
{
	if((long long)asize * min(bsize, _size) < 128LL){
		vector<int> A(_size, 0);
		for(int i = 0; i < asize; ++i){
			for(int j = 0; j < min(bsize, _size - i); ++j){
				A[i+j] = add(A[i+j], mul(a[i], b[j]));
			}
		}
		return A;
	}
    const int size = asize + bsize - 1;
    int t = 1;
    while(t < size){ t <<= 1; }
    vector<int> A(t, 0), B(t, 0);
	for(int i = 0; i < asize; i++){ A[i] = (a[i] < MOD) ? a[i] : (a[i] % MOD); }
	for(int i = 0; i < bsize; i++){ B[i] = (b[i] < MOD) ? b[i] : (b[i] % MOD); }
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

int func(const vector<int>& f, const int u){
    int res = 0, mult = 1;
    for(int i = 0; i < (int)f.size(); ++i){
        res = add(res, mul(f[i], mult));
        mult = mul(mult, u);
    }
    return res;
}

int pre_computation(const vector<int>& u, vector<vector<vector<int> > >& p)
{
    const int m = (int)u.size();
    int sz = 1, t = 1;
    while(t < m) ++sz, t <<= 1;
    const int res = t;
    p.resize(sz), p[sz - 1].resize(t);
    for(int j = 0; j < m; ++j){
        p[sz - 1][j] = {(int)sub(0, u[j]), 1};
    }
    for(int j = m; j < t; ++j){
        p[sz - 1][j] = {1};
    }
    t /= 2;
    for(int i = sz - 2; i >= 0; --i){
        p[i].resize(t);
        for(int j = 0; j < t; ++j){
            const int x = (int)p[i+1][2*j].size(), y = (int)p[i+1][2*j+1].size();
			if(y > 1) p[i][j] = convolute(p[i+1][2*j], p[i+1][2*j+1], x, y, x + y - 1);
			else p[i][j] = p[i+1][2*j];
        }
        t /= 2;
    }
    return res;
}

void recursive_multipoint_evaluation(const vector<int>& f, const vector<int>& u,
    const vector<vector<vector<int> > >& p, vector<int>& ans, const int usize, const int size,
        const int depth, const int num)
{
    if(usize <= 32){
		for(int i = 0; i < usize; ++i){
			const int ad = func(f, u[ans.size()]);
			ans.push_back(ad);
		}
        return;
    }
    const int lsize = min(usize, size / 2), rsize = max(usize - size / 2, 0);
    auto r0 = polynomial_division(f, p[depth + 1][2 * num]);
    recursive_multipoint_evaluation(r0.second, u, p, ans, lsize, size / 2, depth + 1, 2 * num);
    if(rsize == 0) return;
    auto r1 = polynomial_division(f, p[depth + 1][2 * num + 1]);
    recursive_multipoint_evaluation(r1.second, u, p, ans, rsize, size / 2, depth + 1, 2 * num + 1);
}

vector<int> multipoint_evaluation
	(const vector<int>& f, const vector<int>& u, const vector<vector<vector<int> > >& p, const int al)
{
    vector<int> ans;
    return recursive_multipoint_evaluation(f, u, p, ans, (int)u.size(), al, 0, 0), ans;
}

vector<int> differentiate(const vector<int>& a)
{
    const int n = (int)a.size();
    vector<int> res(n - 1);
    for(int i = 0; i < n - 1; ++i){
        res[i] = mul(a[i + 1], i + 1);
    }
    return res;
}

vector<int> _pre_computation
    (const vector<int>& u, const vector<int>& v, vector<vector<vector<int> > >& p)
{
    const int al = pre_computation(u, p);
    const vector<int> g = differentiate(p[0][0]);
    return multipoint_evaluation(g, u, p, al);
}

void recursive_polynomial_interpolation
    (const vector<int>& u, const vector<int>& v, vector<int>& res,
        const vector<vector<vector<int> > >& p, const int usize, const int size,
            const int depth, const int num, const int id)
{
    if(usize == 1){
		res = {v[id]};
		return;
	}
    const int lsize = min(usize, size / 2), rsize = max(usize - size / 2, 0);
	if(rsize == 0){
		vector<int> r0;
		recursive_polynomial_interpolation(u, v, r0, p, lsize, size / 2, depth + 1, 2 * num, id);
		res.resize(lsize);
		for(int i = 0; i < lsize; ++i) res[i] = r0[i];
	}else{
		vector<int> r0, r1;
		recursive_polynomial_interpolation(u, v, r0, p, lsize, size / 2, depth + 1, 2 * num, id);
		recursive_polynomial_interpolation(u, v, r1, p, rsize, size / 2, depth + 1, 2 * num + 1, id + size / 2);
		const vector<int> res1 = convolute(r0, p[depth + 1][2 * num + 1], lsize, rsize + 1, lsize + rsize);
		const vector<int> res2 = convolute(r1, p[depth + 1][2 * num], rsize, lsize + 1, lsize + rsize);
		res.resize(lsize + rsize);
		for(int i = 0; i < lsize + rsize; ++i) res[i] = add(res1[i], res2[i]);
	}
}

vector<int> polynomial_interpolation(const vector<int>& u, const vector<int>& v)
{
    const int n = (int)u.size();
    int t = 1;
    while(t < n) t <<= 1;
    vector<vector<vector<int> > > p;
    const vector<int> inv_s = _pre_computation(u, v, p);
    vector<int> vs(n);
    for(int i = 0; i < n; ++i) vs[i] = mul(v[i], inverse(inv_s[i]));
    vector<int> res;
	recursive_polynomial_interpolation(u, vs, res, p, n, t, 0, 0, 0);
    return res;
}