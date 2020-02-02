// math さんのブログを参照
// http://math314.hateblo.jp/entry/2015/05/07/014908

#include "./header.hpp"

unsigned int mod_pow(unsigned int a, unsigned int n, unsigned int _mod){
    unsigned int ret = 1;
	while(n){
		if(n & 1) ret = (unsigned long long)ret * a % _mod;
		a = (unsigned long long)a * a % _mod;
		n >>= 1;
	}
	return ret;
}

template<unsigned int _mod>
class NTT {
public:
	unsigned int get_mod() const { return _mod; }
    void ntt(vector<int>& a, const bool rev = false){
        unsigned int i, j, k, l, p, q, r, s;
        const unsigned int size = a.size();
    	if(size == 1) return;
    	vector<int> b(size);
    	r = rev ? (_mod - 1 - (_mod - 1) / size) : (_mod - 1) / size;
    	s = mod_pow(3, r, _mod);
    	vector<unsigned int> kp(size / 2 + 1, 1);
    	for(i = 0; i < size / 2; ++i) kp[i + 1] = (unsigned long long)kp[i] * s % _mod;
    	for(i = 1, l = size / 2; i < size; i <<= 1, l >>= 1){
    		for(j = 0, r = 0; j < l; ++j, r += i){
    			for(k = 0, s = kp[i * j]; k < i; ++k){
    				p = a[k + r], q = a[k + r + size / 2];
    				b[k + 2 * r] = ((p + q < _mod) ? (p + q) : (p + q - _mod));
    				b[k + 2 * r + i] = (unsigned long long)((p >= q) ? (p - q) : (_mod - q + p)) * s % _mod;
    			}
    		}
    		swap(a, b);
    	}
        if(rev){
            s = mod_pow(size, _mod - 2, _mod);
            for(i = 0; i < size; ++i){ a[i] = (unsigned long long)a[i] * s % _mod; }
        }
    }
	void convolution(const vector<int>& a, const vector<int>& b, vector<int>& _a, vector<int>& _b,
        unsigned int asize, unsigned int bsize, unsigned int _size){
		unsigned int ntt_size = 1;
		while(ntt_size < asize + bsize) ntt_size *= 2;
        _a.assign(ntt_size, 0), _b.assign(ntt_size, 0);
        for(unsigned int i = 0; i < asize; ++i){ _a[i] = ((a[i] < (int)_mod) ? a[i] : (a[i] % _mod)); }
        for(unsigned int i = 0; i < bsize; ++i){ _b[i] = ((b[i] < (int)_mod) ? b[i] : (b[i] % _mod)); }
		ntt(_a), ntt(_b);
		for(unsigned int i = 0; i < ntt_size; ++i){
			_a[i] = (unsigned long long)_a[i] * _b[i] % _mod;
		}
        ntt(_a, true);
        _a.resize(_size);
	}
};

typedef NTT<167772161> NTT_1;
typedef NTT<469762049> NTT_2;
typedef NTT<1224736769> NTT_3;

vector<int> convolute(const vector<int>& a, const vector<int>& b, int asize, int bsize, int _size){
	NTT_1 ntt1; NTT_2 ntt2; NTT_3 ntt3;
    vector<int> x, _x, y, _y, z, _z;
	ntt1.convolution(a, b, x, _x, asize, bsize, _size);
    ntt2.convolution(a, b, y, _y, asize, bsize, _size);
    ntt3.convolution(a, b, z, _z, asize, bsize, _size);
	const unsigned int m1 = ntt1.get_mod(), m2 = ntt2.get_mod(), m3 = ntt3.get_mod();
	const unsigned int m1_inv_m2 = mod_pow(m1, m2-2, m2);
	const unsigned int m12_inv_m3 = mod_pow((unsigned long long)m1 * m2 % m3, m3-2, m3);
	const unsigned int m12_mod = (unsigned long long)m1 * m2 % MOD;
	vector<int> res(_size);
	for(unsigned int i = 0; i < x.size(); ++i){
	    unsigned int v1 = ((unsigned long long )y[i] + m2 - x[i]) *  m1_inv_m2 % m2;
	    unsigned int v2 = ((unsigned long long)z[i] + m3 - (x[i] + (unsigned long long)m1 * v1) % m3) * m12_inv_m3 % m3;
	    res[i] = (x[i] + (unsigned long long)m1 * v1 + (unsigned long long)m12_mod * v2) % MOD;
	}
    return res;
}
