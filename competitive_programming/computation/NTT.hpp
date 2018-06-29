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

static ll add(const ll x, const ll y) { return (x + y < MOD) ? x + y : x + y - MOD; }
static ll mul(const ll x, const ll y) { return (x * y) % MOD; }
static ll power(ll x, ll n)
{
    ll res = 1;
    while (n > 0) {
        if (n & 1) { res = mul(res, x); }
        x = mul(x, x);
        n >>= 1;
    }
    return res;
}
static ll inverse(const ll x) { return power(x, MOD - 2); }

static void ntt(vector<ll>& a, const bool rev = false)
{
    const ll size = a.size();
    const ll height = (ll)log2(2 * size - 1);
    for (ll i = 0; i < size; i++) {
        ll j = 0;
        for (ll k = 0; k < height; k++) { j |= (i >> k & 1) << (height - 1 - k); }
        if (i < j) { swap(a[i], a[j]); }
    }
    for (ll i = 1; i < size; i *= 2) {
        ll w = power(root, (MOD - 1) / (i * 2));
        if (rev) { w = inverse(w); }
        for (ll j = 0; j < size; j += i * 2) {
            ll wn = 1;
            for (ll k = 0; k < i; k++) {
                const ll s = a[j + k + 0];
                const ll t = mul(a[j + k + i], wn);
                a[j + k + 0] = add(s, t);
                a[j + k + i] = add(s, MOD - t);
                wn = mul(wn, w);
            }
        }
    }
    if (rev) {
        const ll v = inverse(size);
        for (ll i = 0; i < size; i++) { a[i] = mul(a[i], v); }
    }
}

static vector<ll> convolute(const vector<ll>& a, const vector<ll>& b)
{
    const ll size = (int)a.size() + (int)b.size() - 1;
    ll t = 1;
    while (t < size) { t <<= 1; }
    vector<ll> A(t, 0), B(t, 0);
    for (ll i = 0; i < (int)a.size(); i++) { A[i] = a[i]; }
    for (ll i = 0; i < (int)b.size(); i++) { B[i] = b[i]; }
    ntt(A), ntt(B);
    for (ll i = 0; i < t; i++) { A[i] = mul(A[i], B[i]); }
    ntt(A, true);
    A.resize(size);
    return A;
}
