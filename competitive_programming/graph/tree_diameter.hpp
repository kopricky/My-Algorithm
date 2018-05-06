#include "../header.hpp"

// 木の直径を求める

class TreeDiameter
{
public:
    vector<vector<int> > G;
    vector<int> diameter;
    int V;
    TreeDiameter(int node_size){
        V = node_size;
        G.resize(V);
    }
    void add_edge(int u,int v){
        G[u].push_back(v),G[v].push_back(u);
    }
    void dfs(int u,int p,int d,int& far,int& mx){
        if(mx < d){
            far = u;
            mx = d;
        }
        for(int v : G[u]){
            if(v != p){
                dfs(v,u,d+1,far,mx);
            }
        }
    }
    bool redfs(int u,int p,const int t){
        if(u == t){
            return true;
        }
        for(int v : G[u]){
            if(v != p){
                diameter.push_back(v);
                if(redfs(v,u,t)){
                    return true;
                }else{
                    diameter.pop_back();
                }
            }
        }
        return false;
    }
    void solve(){
        int s,t,mx;
        mx = -1;
        dfs(0,-1,0,s,mx);
        mx = -1;
        dfs(s,-1,0,t,mx);
        diameter.push_back(s);
        redfs(s,-1,t);
    }
};
