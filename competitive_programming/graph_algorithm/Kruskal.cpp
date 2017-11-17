//最小全域木のコストが頂点数がn,枝数がm
//Krusukal<int> kr(n,m);
//適宜add_edge
//kr.comp()で最小全域木のコストが返る

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

template<typename T> class Kruskal{
public:
	struct edge{
		int u,v;
		T cost;
		bool operator<(const edge& another) const {
			return cost < another.cost;
		}
	};
	vector<edge> es;
	vector<int> par,rrank;
	int V;
    Kruskal(int node_size){
        V = node_size;
		par.resize(node_size), rrank.resize(node_size);
	}
	void add_edge(int u,int v,T cost){
		es.push_back((edge){u,v,cost});
	}
	T solve(){
		UF uf(V);
		T res = 0;
		int cnt = 0;
        sort(es.begin(),es.end());
		rep(i,es.size()){
			edge e = es[i];
			if(!uf.same(e.u,e.v)){
				uf.unite(e.u,e.v);
				res += e.cost;
				cnt++;
				if(cnt == V-1){
					break;
				}
			}
		}
		return res;
	}
};
