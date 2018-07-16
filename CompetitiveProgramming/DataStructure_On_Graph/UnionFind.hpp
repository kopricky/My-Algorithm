#include "../header.hpp"

//size無し
class UF {
private:
    int sz; vector<int> par,nrank;
public:
    UF(){}
    UF(int node_size){ sz = node_size; par.resize(sz),nrank.resize(sz); rep(i,sz){ par[i] = i; nrank[i] = 0; } }
    int find(int x){ if(par[x] == x){ return x; }else{ return par[x] = find(par[x]); } }
    void unite(int x,int y)
    { x = find(x),y = find(y); if(x == y) return;
    	if(nrank[x] < nrank[y]){ par[x] = y; }else{ par[y] = x; if(nrank[x] == nrank[y]) nrank[x]++; } }
    bool same(int x,int y){ return find(x) == find(y); }
};

//size有り
class UF {
private:
    int sz; vector<int> par,nrank,size;
public:
    UF(){}
    UF(int node_size){ sz = node_size; par.resize(sz),nrank.resize(sz),size.resize(sz);
        rep(i,sz){ par[i] = i; nrank[i] = 0; size[i] = 1;} }
    int find(int x){ if(par[x] == x){ return x; }else{ return par[x] = find(par[x]); } }
    void unite(int x,int y)
    { x = find(x),y = find(y); if(x == y) return; if(nrank[x] < nrank[y]) swap(x,y);
        par[y] = x; size[x] += size[y]; if(nrank[x] == nrank[y]) nrank[x]++; }
    int query(int x){ x = find(x); return size[x]; }
    bool same(int x,int y){ return find(x) == find(y); }
};
