struct edge{
	int u;
	int v;
	int cost;
	bool operator<(const edge& another) const {
		return cost < another.cost;
	}
};

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

edge es[MAX_N];
int par[MAX_N];
int rrank[MAX_N];

int kruskal(int n,int m){
	UF uf(n);
	int res = 0;
	int cnt = 0;
	rep(i,m){
		edge e = es[i];
		if(!uf.same(e.u,e.v)){
			uf.unite(e.u,e.v);
			res += e.cost;
			cnt++;
			if(cnt == n-1){
				break;
			}
		}
	}
	return res;
}
