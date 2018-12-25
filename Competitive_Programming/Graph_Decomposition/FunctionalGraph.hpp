#include "../header.hpp"

//すべての頂点の出次数が1のグラフ(自己ループはない)
//ループの番号はすべて負の数(各ループ内の頂点のインデックスはすべて同じ)
class FunctionalGraph {
public:
    int V;
    vector<int> nx,visit;
    vector<vector<int> > loop;
    vector<vector<int> > G;
    vector<int> st;
    FunctionalGraph(int node_size)  : V(node_size), nx(V, 0), visit(V, 0){}
    void add_edge(int u,int v){
        nx[u] = v;
        if(nx[u] == u) visit[u] = -1, loop.push_back({u});
    }
    void make_loop(const int st,int nw,int ch,vector<int>& vec){
        while(nx[nw] != st){
            vec.push_back(nx[nw]);
            visit[nx[nw]] = ch;
            nw = nx[nw];
        }
    }
    void dfs(int u,int kind,vector<int>& vec){
        visit[u] = kind;
        int v = nx[u];
        if(visit[v] == kind){
            vec.push_back(u),vec.push_back(v);
            int ch = (int)(loop.size()) + 1;
            visit[u] = visit[v] = -ch;
            make_loop(u,v,-ch,vec);
        }else if(visit[v] == 0){
            dfs(v,kind,vec);
        }
    }
    void solve(){
        int id = 1;
        for(int i = 0; i < V; i++){
            if(visit[i] == 0){
                vector<int> vec;
                dfs(i,id++,vec);
                if((int)vec.size()){
                    loop.push_back(vec);
                }
            }
        }
        // make_graph();
    }
    void make_graph(){
        G.resize(V);
        vector<bool> flag(V,false);
        for(int i = 0; i < V; i++){
            if(visit[i] >= 0){
                G[nx[i]].push_back(i);
                flag[nx[i]] = (visit[nx[i]] < 0);
            }
        }
        for(int i = 0; i < V; i++){
            if(flag[i]){
                st.push_back(i);
            }
        }
    }
};
