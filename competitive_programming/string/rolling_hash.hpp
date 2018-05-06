#include "../header.hpp"

struct RollingHash {
	static const ll mo0=1000000007,mo1=1000000009; static ll mul0,mul1;
	static const ll add0=1000010007,add1=1003333331; static vector<ll> pmo[2];
	string s; int sz; vector<ll> hash_[2];
	void init(string s) {
		this->s=s; sz=(int)s.size();
		hash_[0].resize(sz+1,0),hash_[1].resize(sz+1,0);
		if(!mul0) mul0=10009+(((ll)&mul0)>>5)%259,mul1=10007+(((ll)&mul1)>>5)%257;
		if(pmo[0].empty()) pmo[0].push_back(1),pmo[1].push_back(1);
		rep(i,sz) hash_[0][i+1]=(hash_[0][i]*mul0+add0+s[i])%mo0;	//hash_[0][i]はインデックス0~i-1までの文字列のハッシュ値
		rep(i,sz) hash_[1][i+1]=(hash_[1][i]*mul1+add1+s[i])%mo1;
	}
	pair<ll,ll> hash(int l,int r) {	//文字列sのインデックスl~rまでの部分文字列のハッシュ値
		if(l>r) return make_pair(0,0);
		while((int)pmo[0].size()<r+2) pmo[0].push_back(pmo[0].back()*mul0%mo0), pmo[1].push_back(pmo[1].back()*mul1%mo1);
		return make_pair((hash_[0][r+1]+(mo0-hash_[0][l]*pmo[0][r+1-l]%mo0))%mo0,
			             (hash_[1][r+1]+(mo1-hash_[1][l]*pmo[1][r+1-l]%mo1))%mo1);
	}
	pair<ll,ll> hash(string s) { init(s); return hash(0,(int)s.size()-1); }	//文字列s全体のハッシュ値
	static pair<ll,ll> concat(pair<ll,ll> L,pair<ll,ll> R,int RL) { //文字列L+Rのハッシュ値,RLはRの文字列の長さ
		while((int)pmo[0].size()<RL+2) pmo[0].push_back(pmo[0].back()*mul0%mo0), pmo[1].push_back(pmo[1].back()*mul1%mo1);
		return make_pair((R.first + L.first*pmo[0][RL])%mo0,(R.second + L.second*pmo[1][RL])%mo1);
	}
};
vector<ll> RollingHash::pmo[2]; ll RollingHash::mul0,RollingHash::mul1;

vector<int> contain(RollingHash& arg1, string& arg2)
{
	vector<int> res;
	if(arg1.sz < (int)arg2.size()){
		return res;
	}
	RollingHash RH;
	pair<ll,ll> hs = RH.hash(arg2);
	rep(i,arg1.sz-(int)arg2.size()+1){
		if(arg1.hash(i,i+(int)arg2.size()-1) == hs){
			res.push_back(i);
		}
	}
	return res;
}
