#include "../header.hpp"

class range_tsort {
private:
    int n, sz, V;
	vector<vector<int> > G;
	vector<int> deg;
    void add_edge(const int from, const int to){
        G[from].push_back(to), ++deg[to];
    }
public:
    vector<int> res;
	range_tsort(const int node_size) : n(node_size), sz(1){
        while(sz < n) sz *= 2;
		V = 3 * sz - 1;
		G.resize(V), deg.resize(V, 0);
		for(int i = 1; i < sz; ++i){
			add_edge(i, 2 * i), add_edge(i, 2 * i + 1);
			if(2 * i >= sz) add_edge(2 * i, V - i), add_edge(2 * i + 1, V - i);
			else add_edge(V - 2 * i, V - i), add_edge(V - 2 * i - 1, V - i);
		}
    }
    // [p, q) → [r, s)
    void add_edge(int p, int q, int r, int s){
		if(p >= q || r >= s) return;
        p += sz, q += sz, r += sz, s += sz;
		G.push_back({}), deg.push_back(0);
		while(p != q){
			if(p & 1) add_edge((p >= sz) ? (p++) : (V - p++), (int)G.size()  - 1);
			if(q & 1) add_edge((--q >= sz) ? (q) : (V - q), (int)G.size()  - 1);
			p >>= 1, q >>= 1;
		}
		while(r != s){
			if(r & 1) add_edge((int)G.size()  - 1, r++);
			if(s & 1) add_edge((int)G.size()  - 1, --s);
			r >>= 1, s >>= 1;
		}
    }
	bool solve() {
		V = (int)G.size();
		queue<int> que;
		for(int i = 0; i < V; i++){
			if(deg[i] == 0) que.push(i);
		}
		while(!que.empty()){
			int p = que.front();
			que.pop();
			if(p >= sz && p < sz + n) res.push_back(p - sz);
			for(int v : G[p]){
				if(--deg[v] == 0) que.push(v);
			}
		}
		return (*max_element(deg.begin(),deg.end()) == 0);
	}
};