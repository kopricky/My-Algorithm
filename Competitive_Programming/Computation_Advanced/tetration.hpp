#include "../header.hpp"

// a↑↑b % mod を求めるプログラム
// 何度も a↑↑b % mod を計算する場合は phi 関数の値を持っておいたほうがよい

// precondition: n >= 2
unsigned int phi(unsigned int n)
{
    unsigned int res = n;
    if(n % 2 == 0){ res = res / 2; do{ n /= 2; }while(n % 2 == 0); }
    if(n % 3 == 0){ res = res / 3 * 2; do{ n /= 3; }while(n % 3 == 0); }
    for(unsigned int i = 5; i * i <= n; i += 4){
        if(n % i == 0){
            res = res / i * (i - 1);
            do{ n /= i; }while(n % i == 0);
        }
        i += 2;
        if(n % i == 0){
            res = res / i * (i - 1);
            do{ n /= i; }while(n % i == 0);
        }
    }
    if(n != 1) res = res / n * (n - 1);
    return res;
}

// a ^ b % mod
// precondition: a >= 2, a < 2^32, b >= 0, mod >= 2, mod < 2^31
unsigned int mod_pow(unsigned long long a, unsigned int b, unsigned int mod, bool flag)
{
    unsigned long long res = 1;
    while(b){
        if(b & 1){
            // Note: condition is true if a >= mod and flag = false
            if((res *= a) >= mod) res %= mod, flag = true;
        }
        if((a *= a) >= mod) a = a % mod + mod;
        b >>= 1;
    }
    return res + (flag ? mod : 0);
}

// (a ↑↑ b) % mod + mod  if a ↑↑ b >= mod
// a ↑↑ b                otherwise
// precondition: a >= 2, b >= 2, mod >= 2
unsigned int rec(unsigned int a, unsigned int b, unsigned int mod)
{
    if(b == 2) return mod_pow(a, a, mod, false);
    const unsigned int phi_val = phi(mod);
    const unsigned int res = ((phi_val == 1) ? 1 : rec(a, b - 1, phi_val));
    return mod_pow(a, res, mod, (res >= phi_val));
}

// (a ↑↑ b) % mod
// precondition: a >= 0, b >= 0, mod >= 1, mod < 2^31
unsigned int tetration(unsigned int a, unsigned int b, unsigned int mod)
{
    if(mod == 1) return 0;
    if(a == 0) return !(b & 1);
    if(a == 1 || b == 0) return 1;
    if(b == 1) return (a >= mod) ? (a % mod) : a;
    const unsigned int res = rec(a, b, mod);
    return (res >= mod) ? (res - mod) : res;
}