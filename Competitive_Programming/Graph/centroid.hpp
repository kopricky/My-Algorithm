#include "../header.hpp"

//木の重心を求める
class centroid {
private:
	int V;
	vector<vector<int> > G;
	vector<int> sz;
	void dfs(int u,int p, vector<int>& ct){
		bool ok = true;
		sz[u] = 1;
		for(int v : G[u]){
			if(v != p){
				dfs(v, u, ct);
				sz[u] += sz[v];
				if(2*sz[v] > V) ok = false;
			}
		}
		if(ok && V <= 2*sz[u]) ct.push_back(u);
	}

public:
	centroid(int node_size) : V(node_size), G(V), sz(V, 0){}
	void add_edge(int u,int v){
		G[u].push_back(v), G[v].push_back(u);
	}
	vector<int> solve(){
		vector<int> ct;
		dfs(0, -1, ct);
		return ct;
	}
};
