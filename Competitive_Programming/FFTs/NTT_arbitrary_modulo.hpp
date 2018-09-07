// math さんのブログを参照
// http://math314.hateblo.jp/entry/2015/05/07/014908

#include "../header.hpp"

#define MOD 1000000007

inline int mod_pow(int a, int n, int mod){
    int ret = 1;
	while(n){
		if(n & 1) ret = (long long)ret * a % mod;
		a = (long long)a * a % mod;
		n >>= 1;
	}
	return ret;
}

template<int mod, int primitive_root>
class NTT {
public:
	inline int get_mod() const { return mod; }
	inline void _ntt(vector<int>& a, int sign){
		const int n = (int)a.size();
		const int g = 3; //g is primitive root of mod
		int h = mod_pow(g, (mod - 1) / n, mod); // h^n = 1
		if(sign == -1) h = mod_pow(h, mod-2, mod); //h = h^-1 % mod
        int s,x,m,j,k;
        int i = 0;
		for(j = 1; j < n - 1; ++j){
			for(k = n >> 1; k >(i ^= k); k >>= 1);
			if (j < i) swap(a[i], a[j]);
		}
		for (m = 1; m < n; m *= 2){
			const int m2 = 2 * m;
			const int base = mod_pow(h, n / m2, mod);
			int w = 1;
			for(x = 0; x < m; x++){
				for(s = x; s < n; s += m2){
					unsigned int u = a[s];
					unsigned int d = (long long)a[s + m] * w % mod;
					a[s] = (u + d) % mod;
					a[s + m] = (u + mod - d) % mod;
				}
				w = (long long)w * base % mod;
			}
		}
	}
	inline void ntt(vector<int>& input){
		_ntt(input, 1);
	}
	inline void intt(vector<int>& input){
		_ntt(input, -1);
		const int n_inv = mod_pow((int)(input.size()), mod-2, mod);
		for(auto& x : input) x = (long long)x * n_inv % mod;
	}
	inline void convolution(const vector<int>& a, const vector<int>& b, vector<int>& _a, vector<int>& _b){
		int ntt_size = 1;
		while (ntt_size < (int)(a.size()) + (int)(b.size())) ntt_size *= 2;
        _a = a, _b = b;
		_a.resize(ntt_size); _b.resize(ntt_size);
		ntt(_a), ntt(_b);
		for(int i = 0; i < ntt_size; i++){
			_a[i] = (long long)_a[i] * _b[i] % mod;
		}
		intt(_a);
	}
};

typedef NTT<167772161, 3> NTT_1;
typedef NTT<469762049, 3> NTT_2;
typedef NTT<1224736769, 3> NTT_3;

inline void fast_int32mod_convolution(const vector<int>& a, const vector<int>& b, vector<int>& res){
	NTT_1 ntt1; NTT_2 ntt2; NTT_3 ntt3;
    vector<int> x,_x,y,_y,z,_z;
	ntt1.convolution(a, b, x, _x), ntt2.convolution(a, b, y, _y), ntt3.convolution(a, b, z, _z);
	const int m1 = ntt1.get_mod(), m2 = ntt2.get_mod(), m3 = ntt3.get_mod();
	const int m1_inv_m2 = mod_pow(m1, m2-2, m2);
	const int m12_inv_m3 = mod_pow((long long)m1 * m2 % m3, m3-2, m3);
	const int m12_mod = (long long)m1 * m2 % MOD;
	res.resize((int)(x.size()));
	for(int i = 0; i < (int)(x.size()); i++){
	    int v1 = ((long long )y[i] + m2 - x[i]) *  m1_inv_m2 % m2;
	    int v2 = ((long long)z[i] + m3 - (x[i] + (long long)m1 * v1) % m3) * m12_inv_m3 % m3;
	    res[i] = (x[i] + (long long)m1 * v1 + (long long)m12_mod * v2) % MOD;
	}
}
