#include "../header.hpp"

// すべての頂点の出次数が 1 のグラフ
// loop に辺に沿ったループ上の頂点が入る
// visit[v] は頂点 v が i(0_indexed) 番目のループ上の頂点なら i+1 (≧ 1),
// v がループ上になく, i 番目のループにたどり着く場合 -(i+1) (≦ -1) が格納される.
// make_graph() でループから葉へ向かう方向の森を構築

class FunctionalGraph {
private:
    const int V;
    int loop_id;
    vector<int> nx;
    void make_loop(const int st, int nw, vector<int>& vec){
        while(nx[nw] != st){
            vec.push_back(nx[nw]);
            visit[nx[nw]] = loop_id;
            nw = nx[nw];
        }
    }
    int dfs(int u, vector<int>& vec){
        visit[u] = -loop_id;
        int v = nx[u];
        if(visit[v] == -loop_id){
            vec.push_back(u), vec.push_back(v);
            visit[u] = visit[v] = loop_id;
            make_loop(u, v, vec);
            loop_id++;
            return 0;
        }else if(visit[v] == 0){
            const int res = dfs(v, vec);
            if(res == 0) return 0;
            else return visit[u] = res;
        }
        return visit[u] = (visit[v] > 0 ? -visit[v] : visit[v]);
    }
    void make_graph(){
        graph.resize(V);
        for(int i = 0; i < V; i++){
            if(visit[i] < 0) graph[nx[i]].push_back(i);
        }
    }

public:
    vector<int> visit;
    vector<vector<int> > loop, graph;
    FunctionalGraph(const int node_size)
        : V(node_size), loop_id(1), nx(V, 0), visit(V, 0){}
    void add_edge(int u, int v){
        nx[u] = v;
        if(u == nx[u]) visit[u] = loop_id++, loop.push_back({u});
    }
    void solve(){
        for(int i = 0; i < V; i++){
            if(visit[i] == 0){
                vector<int> vec;
                dfs(i, vec);
                if(!vec.empty()) loop.push_back(move(vec));
            }
        }
        // make_graph();
    }
};
