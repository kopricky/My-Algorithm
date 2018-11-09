#include "../header.hpp"

//高速Walsh–Hadamard変換(高速Fourier変換の一般化)
//具体的には xor, or, and convolution の実装
//xor, or, and については Walsh-Hadamard 行列が具体的に知られているので実装は容易
//問題にするならMODで出そうなのでMODをとった実装になっています

#define MOD 1000000007

inline int add(int x, int y)
{
    return (x + y)%MOD;
}

inline int sub(int x, int y)
{
    return (x+MOD-y)%MOD;
}

inline int mul(int x, int y)
{
    return (long long)x*y%MOD;
}

inline int mod_pow(int a, int b)
{
    int res = 1;
    while(b){
        if(b & 1){
            res = mul(res, a);
        }
        a = mul(a, a);
        b >>= 1;
    }
    return res;
}

//xor convolution
void fwht(vector<int>& poly, bool rev=false){
    int n = (int)poly.size();
    for(int i = 1; i < n; i *= 2){
        for(int j = 0; j < i; j++){
            for(int k = 0; k < n; k += i*2){
                int u = poly[j+k], v = poly[j+k+i];
                poly[j+k] = add(u, v);
                poly[j+k+i] = sub(u, v);
            }
        }
    }
    if(rev){
        int inv = mod_pow(n, MOD-2);
        for(int i = 0; i < n; i++){
            poly[i] = mul(poly[i], inv);
        }
    }
}

//or convolution
// void fwht(vector<int>& poly, bool rev=false){
//     int n = (int)poly.size();
//     for(int i = 1; i < n; i *= 2){
//         for(int j = 0; j < i; j++){
//             for(int k = 0; k < n; k += i*2){
//                 if(rev){
//                     poly[j+k+i] = sub(poly[j+k+i], poly[j+k]);
//                 }else{
//                     poly[j+k+i] = add(poly[j+k+i], poly[j+k]);
//                 }
//             }
//         }
//     }
// }

//and convolution
// void fwht(vector<int>& poly, bool rev=false){
//     int n = (int)poly.size();
//     for(int i = 1; i < n; i *= 2){
//         for(int j = 0; j < i; j++){
//             for(int k = 0; k < n; k += i*2){
//                 int u = poly[j+k], v = poly[j+k+i];
//                 if(rev){
//                     poly[j+k] = sub(v, u);
//                     poly[j+k+i] = u;
//                 }else{
//                     poly[j+k] = v;
//                     poly[j+k+i] = add(u, v);
//                 }
//             }
//         }
//     }
// }

vector<int> mul(vector<int> a, vector<int> b){
    int sm = (int)a.size() + (int)b.size() - 1, size_ = 1;
    while(size_ < sm) size_ *= 2;
    a.resize(size_, 0), b.resize(size_, 0);
    fwht(a), fwht(b);
    for(int i = 0; i < size_; i++){
        a[i] = mul(a[i], b[i]);
    }
    fwht(a, true);
    a.resize(sm);
    return a;
}
