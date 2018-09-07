#include "../header.hpp"

struct RH {
	static long long mod[2], mul[2];
    static vector<long long> pm[2];
	string s; int sz; vector<long long> hs[2];
	void init(string& s){
		this->s = s; sz = (int)s.size();
        for(int i = 0; i < 2; i++){
            hs[i].resize(sz+1,0);
            if(pm[i].empty()) pm[i].push_back(1);
            for(int j = 0; j < sz; j++){
                hs[i][j+1] = (hs[i][j]*mul[i]+s[j])%mod[i];
            }
        }
	}
	pair<long long,long long> hash(int l, int r){	//文字列sのインデックス[l,r)の部分文字列のハッシュ値
		if(l>=r) return make_pair(0,0);
        long long res[2];
        for(int i = 0; i < 2; i++){
            while((int)pm[i].size() <= r){
                pm[i].push_back(pm[i].back()*mul[i]%mod[i]);
            }
            res[i] = (hs[i][r] + mod[i] - hs[i][l] * pm[i][r-l] % mod[i]) % mod[i];
        }
		return make_pair(res[0],res[1]);
	}
};
vector<long long> RH::pm[2];
long long RH::mod[2]={1000000007, 1000000009}, RH::mul[2]={10009, 10007};

int contain(RH& arg1, string& arg2)
{
    int res = 0, sz2 = (int)arg2.size();
	if(arg1.sz < sz2){
		return res;
	}
	RH rh;
	rh.init(arg2);
    for(int i = 0; i < arg1.sz-sz2+1; i++){
		if(arg1.hash(i,i+sz2) == rh.hash(0,sz2)){
			res++;
		}
	}
	return res;
}
