#include "../header.hpp"

template<typename T>
T mod_add(const T a, const T b, const T mod){
    return (a + b) % mod;
}

template<typename T>
T mod_mul(const T a, const T b, const T mod){
    return a * b % mod;
}

template<typename T>
T mod_inv(T a, T mod){
    T u[] = {a, 1, 0}, v[] = {mod, 0, 1}, t;
    while(*v){
        t = *u / *v;
        swap(u[0] -= t * v[0], v[0]);
        swap(u[1] -= t * v[1], v[1]);
        swap(u[2] -= t * v[2], v[2]);
    }
    u[1] %= mod;
    return (u[1] < 0) ? (u[1] + mod) : u[1];
}

template<typename T>
T garner(const vector<T>& a, vector<T> p, const T mod){
    const unsigned int sz = a.size();
    vector<T> kp(sz + 1, 0), rmult(sz + 1, 1);
    p.push_back(mod);
    for(unsigned int i = 0; i < sz; ++i){
        T x = mod_mul(a[i] - kp[i], mod_inv(rmult[i], p[i]), p[i]);
        x = (x < 0) ? (x + p[i]) : x;
        for(unsigned int j = i + 1; j < sz + 1; ++j){
            kp[j] = mod_add(kp[j], rmult[j] * x, p[j]);
            rmult[j] = mod_mul(rmult[j], p[i], p[j]);
        }
    }
    return kp[sz];
}
