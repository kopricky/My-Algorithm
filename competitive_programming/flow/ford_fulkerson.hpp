#include "../header.hpp"

template<typename T> class FordFulkerson{
    struct edge{
        int to;
        T cap;
        int rev;
    };
    int V;
    vector<vector<edge> > G;
    vector<bool> used;
    FordFulkerson(int node_size) : V(node_size), G(V), used(V, false){};
    void add_edge(int from, int to, T cap){
        G[from].push_back((edge){to,cap,(int)G[to].size()});
    	G[to].push_back((edge){from,0,(int)G[from].size()-1});
    }
    T dfs(int v, int t, T f){
    	if(v == t) return f;
    	used[v] = true;
    	for(auto& e : G[v]){
    		if(!used[e.to] && e.cap > 0){
    			T d = dfs(e.to, t, min(f, e.cap));
    			if(d > 0){
    				e.cap -= d;
    				G[e.to][e.rev].cap += d;
    				return d;
    			}
    		}
    	}
    	return 0;
    }
    T max_flow(int s, int t){
        T flow = 0;
        for(;;){
            fill(used.begin(), used.end(), false);
    		T f = dfs(s, t, numeric_limits<T>::max());
    		if(!f) return flow;
    		flow += f;
    	}
    }
};
