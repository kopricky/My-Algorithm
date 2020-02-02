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
    const int size = asize + bsize - 1;
    int t = 1;
    while (t < size){ t <<= 1; }
    vector<int> A(t, 0), B(t, 0);
    for(int i = 0; i < asize; i++){ A[i] = (a[i] < MOD) ? a[i] : (a[i] % MOD); }
    for(int i = 0; i < bsize; i++){ B[i] = (b[i] < MOD) ? b[i] : (b[i] % MOD); }
    ntt(A), ntt(B);
    for(int i = 0; i < t; i++){ A[i] = mul(A[i], B[i]); }
    ntt(A, true);
    A.resize(_size);
    return A;
}
 
vector<int> polynomial_inverse(const vector<int>& a, int r)
{
    assert(a[0] != 0);
    vector<int> h = {(int)inverse(a[0])};
    int t = 1;
    for(int i = 0; t < r; ++i){
        t <<= 1;
        vector<int> res = convolute(a, convolute(h, h, t / 2, t / 2, t), min((int)a.size(), t), t, t);
        for(int j = 0; j < t; ++j){
            res[j] = sub(0, res[j]);
            if(j < t / 2) res[j] = add(res[j], mul(2, h[j]));
        }
        swap(h, res);
    }
    h.resize(r);
    return h;
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

vector<int> integral(const vector<int>& a)
{
    const int n = (int)a.size();
    vector<int> res(n + 1, 0);
    for(int i = 0; i < n; ++i){
        res[i + 1] = mul(a[i], inverse(i + 1));
    }
    return res;
}

vector<int> polynomial_log(const vector<int>& a, int r)
{
    assert(a[0] == 1);
    if(r == 0) return {0};
    const int n = (int)a.size();
    const vector<int> num = differentiate(a);
    const vector<int> den = polynomial_inverse(a, r - 1);
    const vector<int> res = convolute(num, den, min(n, r) - 1, r - 1, r - 1);
    return integral(res);
}

vector<int> polynomial_exp(const vector<int>& a, int r)
{
    assert(a[0] == 0);
    const int n = (int)a.size();
    vector<int> ans = {1};
    for(int t = 1; t < r;){
        t <<= 1;
        vector<int> res = polynomial_log(ans, t);
        for(int i = 0; i < t; ++i) res[i] = sub((int)0, res[i]);
        res[0] = add(res[0], 1);
        for(int i = 0; i < min(n, t); ++i){
            res[i] = add(res[i], a[i]);
        }
        ans = convolute(ans, res, t / 2, t, t);
    }
    ans.resize(r);
    return ans;
}