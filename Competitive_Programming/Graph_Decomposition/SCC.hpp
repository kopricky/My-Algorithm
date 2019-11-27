#include "../header.hpp"

// 有向グラフを強連結成分分解する(O(n+m))
class SCC {
private:
    const int V;
    vector<vector<int> > G;
    vector<int> ord, low;
    stack<int> st;
    void dfs(const int u, int& tm){
        ord[u] = low[u] = tm++, st.push(u);
        for(int v : G[u]){
            if(ord[v] < 0){
                dfs(v, tm);
                low[u] = min(low[u], low[v]);
            }else if(cmp[v] < 0){
                low[u] = min(low[u], ord[v]);
            }
        }
        if(ord[u] == low[u]){
            while(true){
                const int v = st.top();
                st.pop();
                cmp[v] = cnt;
                if(v == u) break;
            }
            ++cnt;
        }
    }
public:
    vector<vector<int> > graph;
    vector<int> cmp;
    int cnt;
    SCC(const int node_size)
        : V(node_size), G(V), ord(V, -1), low(V), cmp(V, -1), cnt(0){}
    void add_edge(const int from, const int to){
        G[from].push_back(to);
    }
    int solve(){ // 強連結成分の数を返す
        int tm = 0;
        for(int i = 0; i < V; ++i){
            if(ord[i] < 0) dfs(i, tm);
        }
        for(int i = 0; i < V; ++i) cmp[i] = cnt - 1 - cmp[i];
        return cnt;
    }
    void make_graph(){
        graph.resize(cnt);
        for(int i = 0; i < V; ++i){
            for(int j : G[i]){
                if(cmp[i] != cmp[j]){
                    graph[cmp[i]].push_back(cmp[j]);
                }
            }
        }
    }
};
