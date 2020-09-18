#include "../header.hpp"

const unsigned int numset[] = {2,7,61}; // < 4,759,123,141 ≒ 4×10^9 までは決定的
// const unsigned long long numset[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022ULL}; // 少なくとも 2^64 までは決定的

// int 型の素数判定の場合

unsigned int mod_pow(unsigned int x, unsigned int k, unsigned int mod){
    unsigned int res = 1;
    while(k){
        if(k & 1) res = (unsigned long long)res * x % mod;
        x = (unsigned long long)x * x % mod;
        k >>= 1;
    }
    return res;
}

bool check(unsigned int n){
    if(n < 2 || ((n % 6 != 1) && (n % 6 != 5))) return (n == 2) || (n == 3);
    unsigned int d = n - 1, s = 0;
    while(d % 2 == 0){
        d /= 2;
        s++;
    }
    for(unsigned int a : numset){
        if(a == n) return true;
        unsigned int res = mod_pow(a, d, n);
        if(res == 1) continue;
        bool ok = true;
        for(unsigned int r = 0; r < s; r++){
            if(res == n-1){
                ok = false;
                break;
            }
            res = (unsigned long long)res * res % n;
        }
        if(ok) return false;
    }
    return true;
}

// long long型の素数判定の場合

// unsigned long long mod_mul(unsigned long long a, unsigned long long b, unsigned long long mod) {
// 	long long ret = a * b - mod * (unsigned long long)((long double)(a) * (long double)(b) / (long double)(mod));
// 	return ret + mod * (ret < 0) - mod * (ret >= (ll)mod);
// }

// unsigned long long mod_pow(unsigned long long x, unsigned long long k, unsigned long long mod){
//     unsigned long long res = 1;
//     while(k){
//         if(k & 1) res = mod_mul(res, x, mod);
//         x = mod_mul(x, x, mod);
//         k >>= 1;
//     }
//     return res;
// }

// bool check(unsigned long long n){
//     if(n < 2 || ((n % 6 != 1) && (n % 6 != 5))) return (n == 2) || (n == 3);
//     unsigned long long d = n - 1, s = 0;
//     while(d % 2 == 0){
//         d /= 2;
//         s++;
//     }
//     for(unsigned long long a : numset){
//         if(a % n == 0) return true;
//         unsigned long long res = mod_pow(a, d, n);
//         if(res == 1) continue;
//         bool ok = true;
//         for(unsigned int r = 0; r < s; r++){
//             if(res == n-1){
//                 ok = false;
//                 break;
//             }
//             res = mod_mul(res, res, n);
//         }
//         if(ok) return false;
//     }
//     return true;
// }

