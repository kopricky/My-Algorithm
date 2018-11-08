#include "../header.hpp"

// 特殊なmodの場合
// 剰余と原始根
// (924844033, 5)
// (998244353, 3)
// (1012924417, 5)
// (167772161, 3)
// (469762049, 3)
// (1224736769, 3)

#define MOD 924844033
#define root 5

int add(const int x, const int y) { return (x + y < MOD) ? x + y : x + y - MOD; }
int mul(const int x, const int y) { return (long long)x * y % MOD; }
int power(int x, int n)
{
    int res = 1;
    while(n > 0) {
        if (n & 1) { res = mul(res, x); }
        x = mul(x, x);
        n >>= 1;
    }
    return res;
}
int inverse(const int x) { return power(x, MOD - 2); }

void ntt(vector<int>& a, const bool rev = false)
{
    int i,j,k,s,t,v,w,wn;
    const int size = (int)a.size();
    const int height = (int)log2(2 * size - 1);
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

vector<int> convolute(const vector<int>& a, const vector<int>& b)
{
    const int size = (int)a.size() + (int)b.size() - 1;
    int t = 1;
    while (t < size) { t <<= 1; }
    vector<int> A(t, 0), B(t, 0);
    for (int i = 0; i < (int)a.size(); i++) { A[i] = a[i]; }
    for (int i = 0; i < (int)b.size(); i++) { B[i] = b[i]; }
    ntt(A), ntt(B);
    for (int i = 0; i < t; i++) { A[i] = mul(A[i], B[i]); }
    ntt(A, true);
    A.resize(size);
    return A;
}
