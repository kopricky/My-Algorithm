//並列二分探索
//以下は併合クエリに対していつ初めて同じ連結成分になったかについて
//併合クエリの時系列を二分探索することで求めている
#include "../header.hpp"

const int MAX_N = 100005;
//2*10^5の場合
const int DEPTH = 18;

class UF {
private:
    int sz; vector<int> par,nrank;
public:
    UF(){}
    UF(int node_size){ sz = node_size; par.resize(sz),nrank.resize(sz); rep(i,sz){ par[i] = i; nrank[i] = 0; } }
    void build(int node_size){ sz = node_size; par.resize(sz),nrank.resize(sz); rep(i,sz){ par[i] = i; nrank[i] = 0; } }
    int find(int x){ if(par[x] == x){ return x; }else{ return par[x] = find(par[x]); } }
    void unite(int x,int y)
    { x = find(x),y = find(y); if(x == y) return;
    	if(nrank[x] < nrank[y]){ par[x] = y; }else{ par[y] = x; if(nrank[x] == nrank[y]) nrank[x]++; } }
    bool same(int x,int y){ return find(x) == find(y); }
};

UF uf[DEPTH];
int n,m;
bool flag[DEPTH];
int ed[DEPTH], ans[MAX_N];
//que;連結時間クエリ,id:クエリの番号,vec:併合操作クエリ
void dfs(int l, int r, int h, const vector<P>& que, const vector<int>& id, const vector<P>& vec)
{
    if(!flag[h]){
        flag[h] = true;
        uf[h].build(n);
    }
    if(r-l==1){
        if(l == m){
            rep(i,(int)id.size()){
                ans[id[i]] = -1;
            }
        }else{
            rep(i,(int)id.size()){
                ans[id[i]] = l+1;
            }
        }
        if(l != m){
            uf[h].unite(vec[l].first,vec[l].second);
        }
        return;
    }
    vector<P> lq,rq;
    vector<int> li,ri;
    int mid = (l+r)/2;
    for(;ed[h]<mid;ed[h]++){
        uf[h].unite(vec[ed[h]].first,vec[ed[h]].second);
    }
    rep(i,(int)que.size()){
        if(uf[h].same(que[i].first,que[i].second)){
            lq.push_back(que[i]), li.push_back(id[i]);
        }else{
            rq.push_back(que[i]), ri.push_back(id[i]);
        }
    }
    dfs(l,mid,h+1,lq,li,vec), dfs(mid,r,h+1,rq,ri,vec);
}
