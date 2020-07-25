#include "../header.hpp"

class biconnected {
private:
    void dfs(int u, int p, int& tm)
    {
        bool flag = false;
        ord[u] = low[u] = tm++, st.push(u);
        for(int v : G[u]){
            if(ord[v] < 0){
                dfs(v, u, tm);
                low[u] = min(low[u], low[v]);
            }else if(v != p){
                low[u] = min(low[u], ord[v]);
            }else{
                if(flag) low[u] = min(low[u], ord[v]);
                else flag = true;
            }
        }
        if(ord[u] == low[u]){
            if(p >= 0) bridge.emplace_back(u, p);
            while(true){
                const int v = st.top();
                st.pop();
                cmp[v] = kind;
                if(v == u) break;
            }
            ++kind;
        }
    }

public:
    const int V;
    int kind;
    vector<vector<int> > G, tree;
    vector<pair<int, int> > bridge;
    vector<int> ord, low, cmp;
    stack<int> st;
    biconnected(int node_size)
        : V(node_size), kind(0), G(V), ord(V, -1), low(V), cmp(V){}
    void add_edge(const int u, const int v){
        G[u].push_back(v), G[v].push_back(u);
    }
    // 橋を検出する(二(辺)連結成分の個数を返す).
    int identify_bridge(){
        int tm = 0;
        for(int i = 0; i < V; ++i){
            if(ord[i] < 0) dfs(i, -1, tm);
        }
        for(int i = 0; i < V; ++i) cmp[i] = kind - 1 - cmp[i];
        return kind;
    }
    // 2(辺)連結成分を頂点とする木(tree) を作る(事前に identify_bridge() を呼んでおく必要がある).
    // 木の頂点数を返す.
    // cmp[i] は元の頂点 i の含まれる頂点(2(辺)連結成分) を表す.
    int make_bctree()
    {
        tree.resize(kind);
        for(int i = 0; i < (int)bridge.size(); i++){
            int a = cmp[bridge[i].first], b = cmp[bridge[i].second];
            tree[a].push_back(b), tree[b].push_back(a);
        }
        return kind;
    }
};
