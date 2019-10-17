#include "../header.hpp"

//離散対数問題に対するアルゴリズム
int mod_pow(int a, int b, int m)
{
    a %= m;
    int res = 1;
    while(b){
        if(b & 1){
            res = (long long)res * a % m;
        }
        a = (long long)a * a % m;
        b >>= 1;
    }
    return res;
}

int mod_inv(int a, int m){
	int u[] = {a, 1, 0}, v[] = {m, 0, 1}, t;
    while(*v){
		t = *u / *v;
		swap(u[0] -= t * v[0], v[0]);
        swap(u[1] -= t * v[1], v[1]);
        swap(u[2] -= t * v[2], v[2]);
    }
	return (u[1] % m + m) % m;
}

int gcd(int a, int b){
    int tmp;
    while(b) tmp = a, a = b, b = tmp % b;
    return a;
}

// a^x ≡ b (mod p) の解 x を求める (p^(1/2) log p)
int baby_step_giant_step(int a, int b, int p)
{
    int m = ceil(sqrt(p));
    unordered_map<int, int> mp;
    int val = 1;
    for(int i = 0; i < m; ++i){
        if(mp.find(val) == mp.end()) mp[val] = i;
        val = (long long)val * a % p;
    }
    int inv = mod_pow(mod_inv(a, p), m, p);
    int cur = b, res = numeric_limits<int>::max();
    for(int i = 0; i < m; ++i){
        auto it = mp.find(cur);
        if(it != mp.end()){
            res = min(res, i * m + it->second);
        }else{
            cur = (long long)cur * inv % p;
        }
    }
    return (res == numeric_limits<int>::max()) ? -1 : res;
}

int general_baby_step_giant_step(int a, int b, int p)
{
    if(a == 0){
        if(b == 0) return (p == 1) ? 0 : 1;
        else if(b == 1) return 0;
        else return -1;
    }
    int g, cnt = 0;
    for(;;++cnt){
        if((b == 1) || (p == 1)) return cnt;
        if((g = gcd(a, p)) == 1) break;
        if(b % g != 0) return -1;
        b /= g, p /= g;
        b = (long long)mod_inv(a / g, p) * b % p;
    }
    int res = baby_step_giant_step(a, b, p);
    return (res < 0) ? res : res + cnt;
}
