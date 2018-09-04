#include "../header.hpp"

struct RH　{
    static ll mod[2], mul[4];
    static vector<ll> pm[4];
    vector<string> s; int rw, cl; vector<vector<ll> > hs[2];
    void init(vector<string>& s){
        this->s = s; rw = (int)s.size(), cl = (int)s[0].size();
        for(int i = 0; i < 2; i++){
            hs[i].resize(rw+1,vector<ll>(cl+1,0));
            if(pm[i].empty()) pm[i].push_back(1), pm[i+2].push_back(1);
            for(int j = 0; j < rw; j++){
                for(int k = 0; k < cl; k++){
                    hs[i][j+1][k+1] = (hs[i][j+1][k]*mul[i]+s[j][k])%mod[i];
                }
            }
            for(int j = 0; j < rw; j++){
                for(int k = 0; k < cl; k++){
                    hs[i][j+1][k+1] = (hs[i][j][k+1]*mul[i+2]+hs[i][j+1][k+1])%mod[i];
                }
            }
        }
    }
    ll comp(int u, int l, int d, int r, int id, const ll mod){
        return (hs[id][d][r]+mod-hs[id][d][l]*pm[id][r-l]%mod
                +mod-hs[id][u][r]*pm[id+2][d-u]%mod
                +hs[id][u][l]*pm[id][r-l]%mod*pm[id+2][d-u])%mod;
    }
    pair<ll,ll> hash(int u,int l,int d,int r){ //文字列sのインデックス[u,d),[l,r)までの部分文字列のハッシュ値
        if(l>=r||u>=d) return make_pair(0,0);
        for(int i = 0; i < 2; i++){
            while((int)pm[i].size() <= r) pm[i].push_back(pm[i].back()*mul[i]%mod[i]);
            while((int)pm[i+2].size() <= d) pm[i+2].push_back(pm[i+2].back()*mul[i+2]%mod[i]);
        }
        return make_pair(comp(u,l,d,r,0,mod[0]),comp(u,l,d,r,1,mod[1]));
    }
};
vector<ll> RH::pm[4];
ll RH::mod[2]={1000000007,1000000009}, RH::mul[4]={10009,10007,10039,10037};

bool contain(RH& arg1, vector<string>& arg2){
    int r = (int)arg2.size(), c = (int)arg2[0].size();
    if(arg1.rw < r || arg1.cl < c){
        return false;
    }
    RH rh;
    rh.init(arg2);
    pair<ll,ll> hs = rh.hash(0, 0, r, c);
    for(int i = 0; i < arg1.rw-r+1; i++){
        for(int j = 0; j < arg1.cl-c+1; j++){
            if(arg1.hash(i,j,i+r,j+c) == hs){
                return true;
            }
        }
    }
    return false;
}
