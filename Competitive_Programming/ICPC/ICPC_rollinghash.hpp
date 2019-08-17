#include "../header.hpp"

struct RH {
	static int mod[2], mul[2];
    static vector<int> pm[2];
    int sz;
    vector<vector<int> > hs;
	RH(const string& s) : sz((int)s.size()), hs(2, vector<int>(sz + 1, 0)){
        for(int i = 0; i < 2; i++){
            if(pm[i].empty()) pm[i].push_back(1);
            for(int j = 0; j < sz; j++){
                hs[i][j+1] = ((long long)hs[i][j] * mul[i] + s[j]) % mod[i];
            }
        }
	}
    // 文字列 s のインデックス [l,r) の部分文字列のハッシュ値
	pair<int, int> hash(const int l, const int r){
		if(l >= r) return make_pair(0, 0);
        int res[2];
        for(int i = 0; i < 2; i++){
            while((int)pm[i].size() <= r){
                pm[i].push_back((long long)pm[i].back() * mul[i] % mod[i]);
            }
            res[i] = (hs[i][r] - (long long)hs[i][l] * pm[i][r-l]) % mod[i] + mod[i];
            res[i] = (res[i] >= mod[i]) ? (res[i] - mod[i]) : res[i];
        }
		return make_pair(res[0], res[1]);
	}
    // 文字列 s 中の pattern の含まれる位置を pos に格納
    void contain(const string& pattern, vector<int>& pos){
        int length = (int)pattern.size();
        if(sz < length) return;
        RH rh(pattern);
        pair<int, int> _hash = rh.hash(0, length);
        for(int i = 0; i < sz - length + 1; i++){
    		if(hash(i, i + length) == _hash){
    			pos.push_back(i);
    		}
    	}
        return;
    }
};
vector<int> RH::pm[2];
int RH::mod[2] = {1000000007, 1000000009}, RH::mul[2] = {10009, 10007};
