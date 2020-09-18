#include "../header.hpp"

const unsigned long long numset[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022ULL};

unsigned long long mod_mul(unsigned long long a, unsigned long long b, unsigned long long mod) {
	long long ret = a * b - mod * (unsigned long long)((long double)(a) * (long double)(b) / (long double)(mod));
	return ret + mod * (ret < 0) - mod * (ret >= (ll)mod);
}

unsigned long long mod_pow(unsigned long long x, unsigned long long k, unsigned long long mod){
    unsigned long long res = 1;
    while(k){
        if(k & 1) res = mod_mul(res, x, mod);
        x = mod_mul(x, x, mod);
        k >>= 1;
    }
    return res;
}

bool miller_rabbin(unsigned long long n){
    if(n < 2 || ((n % 6 != 1) && (n % 6 != 5))) return (n == 2) || (n == 3);
    unsigned long long d = n - 1, s = 0;
    while(d % 2 == 0) d /= 2, ++s;
    for(unsigned long long a : numset){
        if(a % n == 0) return true;
        unsigned long long res = mod_pow(a, d, n);
        if(res == 1) continue;
        bool ok = true;
        for(unsigned int r = 0; r < s; r++){
            if(res == n-1){
                ok = false;
                break;
            }
            res = mod_mul(res, res, n);
        }
        if(ok) return false;
    }
    return true;
}

inline unsigned long long pollard_f(unsigned long long x, unsigned long long n){
    const unsigned long long res = mod_mul(x, x, n);
    return (res == n - 1) ? 0 : (res + 1);
}

unsigned long long gcd(unsigned long long a, unsigned long long b){
    unsigned long long tmp;
    while(b) tmp = a, a = b, b = tmp % b;
    return a;
}

unsigned long long pollard(unsigned long long n){
    unsigned long long x = 0, y = 0, t = 0, i = 2, prod = 2, res;
    while((t++) % 50 || gcd(prod, n) == 1){
        if(x == y) x = i++, y = pollard_f(x, n);
        res = mod_mul(prod, (x > y) ? (x - y) : (y - x), n);
        if(res > 0) prod = res;
        x = pollard_f(x, n), y = pollard_f(pollard_f(y, n), n);
    }
    return gcd(prod, n);
}

vector<pair<unsigned long long, int> > factorize(unsigned long long n){
    if(n == 1) return vector<pair<unsigned long long, int> >();
    if(miller_rabbin(n)) return vector<pair<unsigned long long, int> >{{n, 1}};
    vector<pair<unsigned long long, int> > res;
    const unsigned long long x = pollard(n);
    const auto res1 = factorize(x), res2 = factorize(n / x);
    for(unsigned int id1 = 0, id2 = 0; id1 < res1.size() || id2 < res2.size();){
        if(id1 == res1.size()){
            for(; id2 < res2.size(); ++id2){
                if(res.empty() || res.back().first != res2[id2].first) res.push_back(res2[id2]);
                else res.back().second += res2[id2].second;
            }
        }else if(id2 == res2.size()){
            for(; id1 < res1.size(); ++id1){
                if(res.empty() || res.back().first != res1[id1].first) res.push_back(res1[id1]);
                else res.back().second += res1[id1].second;
            }
        }else{
            if(res1[id1].first < res2[id2].first) res.push_back(res1[id1++]);
            else if(res1[id1].first > res2[id2].first) res.push_back(res2[id2++]);
            else res.emplace_back(res1[id1].first, res1[id1].second + res2[id2].second), ++id1, ++id2;
        }
    }
    return res;
}
