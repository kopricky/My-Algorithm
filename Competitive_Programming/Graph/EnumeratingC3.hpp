#include "../header.hpp"

// グラフは単純な無向グラフであることを仮定する
class EnumeratingC3 {
private:
    int V, threshold;
    vector<vector<int> > G;
    vector<vector<array<int, 2> > > memo;
    bool *flag;
    void process_high_degree(){
        for(int i = 0; i < V; ++i){
            if((int)G[i].size() <= threshold) continue;
            for(const int u : G[i]) flag[u] = true;
            for(const int u : G[i]){
                for(const int v : G[u]){
                    if(flag[v]) ans.push_back({i, u, v});
                }
            }
            for(const int u : G[i]) flag[u] = false;
        }
    }
    void process_low_degree(){
        for(int i = 0; i < V; ++i){
            if((int)G[i].size() > threshold) continue;
            for(const int u : G[i]){
                for(const int v : G[i]){
                    if(v > u) memo[u].push_back({i, v});
                }
            }
        }
        for(int i = 0; i < V; ++i){
            for(const int u : G[i]) flag[u] = true;
            for(const auto& e : memo[i]){
                if(flag[e[1]]) ans.push_back({e[0], i, e[1]});
            }
            for(const int u : G[i]) flag[u] = false;
        }
    }
public:
    // ans に三角形 (a, b, c) (a < b < c) が重複なく格納される
    vector<array<int, 3> > ans;
    EnumeratingC3(const int node_size)
         : V(node_size), threshold(0), G(V), memo(V), flag(new bool[V]()){}
    ~EnumeratingC3(){ delete[] flag; }
    void add_edge(const int u, const int v){
        if(u < v) G[u].push_back(v);
        else G[v].push_back(u);
        ++threshold;
    }
    void solve(){
        threshold = floor(sqrt(threshold)) / 2;
        process_high_degree();
        process_low_degree();
    }
};
