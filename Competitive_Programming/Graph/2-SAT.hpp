#include "../header.hpp"

class TwoSAT {
private:
	const int V;
	vector<vector<int> > G, rG;
	vector<int> ps, cmp;
	void add_edge(int from, int to){
		G[from].push_back(to), rG[to].push_back(from);
	}
	void dfs(int u){
        cmp[u] = 0;
        for(int v : G[u]){
			if(cmp[v] == -1) dfs(v);
		}
		ps.push_back(u);
	}
	void rdfs(int u, int k){
		cmp[u] = k;
		for(int v : rG[u]){
			if(cmp[v] == -1) rdfs(v, k);
		}
	}
	int scc(){
		for(int i = 0; i < 2 * V; ++i){
			if(cmp[i] == -1) dfs(i);
		}
		fill(cmp.begin(), cmp.end(), -1);
		int k = 0;
		for(int i = 2 * V - 1; i >= 0; --i){
			if(cmp[ps[i]] == -1) rdfs(ps[i], k++);
		}
		return k;
	}

public:
	vector<int> ans;
	TwoSAT(const int literal_count)
	 	: V(literal_count), G(2 * V), rG(2 * V), cmp(2 * V, -1), ans(V){}
	void add_closure(int x, int y){
		add_edge((x + V) % (2 * V), y), add_edge((y + V) % (2 * V), x);
	}
	// 充足可能性判定
	// 真のものは 1,偽のものは 0 が ans に格納される(解の構成)
	bool satisfy(){
		scc();
		for(int i = 0; i < V; i++){
			if(cmp[i] == cmp[V + i]) return false;
			ans[i] = (cmp[i] > cmp[V + i]);
		}
		return true;
	}
};
