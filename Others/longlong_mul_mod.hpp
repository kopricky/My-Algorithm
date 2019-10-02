#include "./header.hpp"

#define MOD 1000000007

// コドフォなどで __int128 型が使えないときに long double を用いて long long 同士の積の剰余を求める裏技
long long mul(long long a, long long b){
    long long res = (a * b - (long long)((long double)a / MOD * b) * MOD) % MOD;
    return res < 0 ? res + MOD : res;
}

// Returns (a * b) % mod
long long moduloMultiplication(long long a,
                            long long b,
                            long long mod)
{
    long long res = 0; // Initialize result

    // Update a if it is more than
    // or equal to mod
    a %= mod;

    while (b)
    {
        // If b is odd, add a with result
        if (b & 1)
            res = (res + a) % mod;

        // Here we assume that doing 2*a
        // doesn't cause overflow
        a = (2 * a) % mod;

        b >>= 1; // b = b / 2
    }

    return res;
}
