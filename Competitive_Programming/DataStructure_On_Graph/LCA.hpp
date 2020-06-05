#include "../header.hpp"

template<typename T> class segtree {
private:
    int n,sz;
    vector<pair<T, int> > node;
public:
    void resize(vector<T>& v){
        sz = (int)v.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.resize(2*n);
        for(int i = 0; i < sz; i++){
            node[i+n] = make_pair(v[i], i);
        }
        for(int i=n-1; i>=1; i--){
            node[i] = min(node[2*i], node[2*i+1]);
        }
    }
    void update(int k, T a)
    {
    	node[k+=n] = make_pair(a, k);
    	while(k>>=1){
            node[k] = min(node[2*k], node[2*k+1]);
    	}
    }
    pair<T, int> query(int a,int b)
    {
        pair<T, int> res1 = make_pair(numeric_limits<T>::max(), -1);
        pair<T, int> res2 = make_pair(numeric_limits<T>::max(), -1);
        a += n, b += n;
        while(a != b){
            if(a % 2) res1 = min(res1, node[a++]);
            if(b % 2) res2 = min(res2, node[--b]);
            a >>= 1, b >>= 1;
        }
        return min(res1, res2);
    }
};

class LCA{
public:
    int V;
    vector<vector<int> > G;
    vector<int> ord,depth,id;
    segtree<int> st;
    LCA(int node_size) : V(node_size), G(V), depth(V), id(V, -1){}
    void add_edge(int from,int to){
        G[from].push_back(to),G[to].push_back(from);
    }
    void dfs(int u,int p,int k){
        id[u] = (int)ord.size();
        ord.push_back(u);
        depth[u] = k;
        for(int v : G[u]){
            if(v != p){
                dfs(v,u,k+1);
                ord.push_back(u);
            }
        }
    }
    void build(){
        ord.reserve(2*V-2);
        for(int i = 0; i < V; i++){
            if(id[i] < 0){
                dfs(i,-1,0);
            }
        }
        vector<int> stvec(2*V-2);
    	for(int i = 0; i < 2*V-2; i++){
    		stvec[i] = depth[ord[i]];
    	}
        st.resize(stvec);
    }
    int solve(int u,int v){
        return ord[st.query(min(id[u],id[v]),max(id[u],id[v])+1).second];
    }
    int dist(int u,int v){
        int lca = solve(u,v);
        return depth[u] + depth[v] - 2*depth[lca];
    }
};

// 重みつき版
// template<typename T> class LCA{
// public:
//     struct edge {
//         int to; T cost;
//         edge(int _to, T _cost) : to(_to), cost(_cost){}
//     };
//     int V;
//     vector<vector<edge> > G;
//     vector<T> depth;
//     vector<int> ord, id;
//     segtree<T> st;
//     LCA(int node_size) : V(node_size), G(V), depth(V), id(V, -1){}
//     void add_edge(int from, int to, T cost){
//         G[from].emplace_back(to, cost),G[to].emplace_back(from, cost);
//     }
//     void dfs(int u, int p, T k){
//         id[u] = (int)ord.size();
//         ord.push_back(u);
//         depth[u] = k;
//         for(const edge& e : G[u]){
//             if(e.to != p){
//                 dfs(e.to, u, k + e.cost);
//                 ord.push_back(u);
//             }
//         }
//     }
//     void build(){
//         ord.reserve(2*V-2);
//         for(int i = 0; i < V; i++){
//             if(id[i] < 0){
//                 dfs(i, -1, 0);
//             }
//         }
//         vector<T> stvec(2*V-2);
//     	for(int i = 0; i < 2*V-2; i++){
//     		stvec[i] = depth[ord[i]];
//     	}
//         st.resize(stvec);
//     }
//     int solve(int u,int v){
//         return ord[st.query(min(id[u],id[v]),max(id[u],id[v])+1).second];
//     }
//     T dist(int u,int v){
//         int lca = solve(u,v);
//         return depth[u] + depth[v] - 2*depth[lca];
//     }
// };