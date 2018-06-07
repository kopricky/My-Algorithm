#include "../header.hpp"

template<typename T> class Prim{
public:
	struct edge{
		int to;
		T cost;
	};
    using pti = pair<T, int>;
    int V;
	vector<vector<edge> > G;
    vector<T> d;
    vector<bool> used;
    Prim(int node_size) : V(node_size), G(V), d(V, numeric_limits<T>::max()), used(V, false){}
	void add_edge(int u, int v, T val){
        G[u].push_back((edge){v, val}), G[v].push_back((edge){u, val});
	}
	T solve(){
        priority_queue<pti, vector<pti>, greater<pti> > que;
        T res = 0;
		d[0] = 0;
        que.push(pti(0, 0));
        while(!que.empty()){
            pti p = que.top();
            que.pop();
            int v = p.second;
            if(used[v]) continue;
            res += p.first;
            used[v] = true;
            for(auto& e : G[v]){
                if(!used[e.to] && d[e.to] > e.cost){
                    d[e.to] = e.cost;
                    que.push(pti(d[e.to], e.to));
                }
            }
        }
        return res;
	}
};
