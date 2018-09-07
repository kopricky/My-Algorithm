#include "../header.hpp"

// なもりグラフ(n頂点n辺からなる連結な無向グラフ)を閉路とその他に分ける(前処理)

class Namori{
public:
    int V;
    vector<vector<int> > G, graph;
    vector<int> deg, loop;
    vector<bool> flag;
    Namori(int node_size) : V(node_size), G(V), deg(V, 0), flag(V, false){}
    void add_edge(int u,int v){
        G[u].push_back(v),G[v].push_back(u);
        deg[u]++, deg[v]++;
    }
    void dfs(int u){
        loop.push_back(u);
        for(int v : G[u]){
            if(flag[v]) continue;
            flag[v] = true;
            dfs(v);
        }
    }
    void build(){
        graph.resize(V);
        queue<int> que;
        for(int i = 0; i < V; i++){
            if(deg[i] == 1){
                que.push(i);
                flag[i] = true;
            }
        }
        while(!que.empty()){
            int p = que.front();
            que.pop();
            for(int v : G[p]){
                if(flag[v]) continue;
                deg[v]--;
                //サイクルから端
                graph[v].push_back(p);
                //端からサイクル
                graph[p].push_back(v);
                if(deg[v] > 1) continue;
                que.push(v);
                flag[v] = true;
            }
        }
        for(int i = 0; i < V; i++){
            if(flag[i]) continue;
            flag[i] = true;
            dfs(i);
            break;
        }
    }
};
