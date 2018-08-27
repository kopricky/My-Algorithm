#include "../header.hpp"
//unite操作を巻き戻せるunion find
//オフラインクエリのグラフの動的連結性判定などに使える
class UF_pop {
public:
    int sz;
    vector<int> par,nrank; vector<vector<int> > hist;
    UF_pop(int node_size) : sz(node_size), par(sz), nrank(sz, 0){
        rep(i,sz){
            par[i] = i;
        }
    }
    void reinit(){
        rep(i,sz){
            nrank[i] = 0; par[i] = i;
        }
    }
    int find(int x){
        return (par[x] == x)?(x):find(par[x]);
    }
    void pop(){
         if(!hist.empty()){
            auto& a = hist.back();
            par[a[0]] = a[0], nrank[a[0]] = a[1], par[a[2]] = a[2];
            hist.pop_back();
        }
    }
    void unite(int x,int y) {
        x = find(x); y = find(y);
        hist.push_back({x,nrank[x],y});
        if(x == y) return;
        if(nrank[x] < nrank[y]){
            par[x] = y;
        }else{
            nrank[x] += (nrank[x] == nrank[y]);
            par[y] = x;
        }
    }
    bool same(int x,int y){
        return find(x) == find(y);
    }
};
