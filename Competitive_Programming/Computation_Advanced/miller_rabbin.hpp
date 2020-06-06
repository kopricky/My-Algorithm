#include "../header.hpp"

const vector<unsigned int> numset = {2,7,61}; // < 4,759,123,141 ≒ 4×10^9
// const vector<unsigned int> numset = {2,3,5,7,11,13,17}; // < 341,550,071,728,321 ≒　3*10^14
// const vector<unsigned int> numset = {2,3,5,7,11,13,17,19,23,29,31,37}; // 2^64までいける

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

// check if n is prime
bool check(unsigned int n){
    if(n < 2) return false;
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

// inline unsigned long long mod_comp(unsigned __int128 a, unsigned long long b) {
//   unsigned long long q, r;
//   __asm__ (
//     "divq\t%4"
//     : "=a"(q), "=d"(r)
//     : "0"((unsigned long long)(a)), "1"((unsigned long long)(a >> 64)), "rm"(b)
//   );
//   return r;
// }

// unsigned long long mod_pow(unsigned long long x, unsigned long long k, unsigned long long mod){
//     unsigned long long res = 1;
//     while(k){
//         if(k & 1) res = mod_comp((unsigned __int128)res * x, mod);
//         x = mod_comp((unsigned __int128)x * x, mod);
//         k >>= 1;
//     }
//     return res;
// }

// bool check(unsigned long long n){
//     if(n < 2) return false;
//     unsigned long long d = n - 1, s = 0;
//     while(d % 2 == 0){
//         d /= 2;
//         s++;
//     }
//     for(unsigned int a : numset){
//         if(a == n) return true;
//         unsigned long long res = mod_pow(a, d, n);
//         if(res == 1) continue;
//         bool ok = true;
//         for(unsigned int r = 0; r < s; r++){
//             if(res == n-1){
//                 ok = false;
//                 break;
//             }
//             res = mod_comp((unsigned __int128)res * res, n);
//         }
//         if(ok) return false;
//     }
//     return true;
// }

