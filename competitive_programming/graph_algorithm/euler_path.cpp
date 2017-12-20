//無向グラフ：全ての頂点の次数が偶数
//有向グラフ：全ての頂点の相対次数が0
//Hierholzer's Algorithm

class EulerPath
{
public:
    vector<vector<int> > G;
    vector<int> degree, path;
    int V; bool directed;
    EulerPath(int node_size, bool flag){
        V = node_size;
        directed = flag;
        G.resize(V,vector<int>(V,0));
        degree.resize(V,0);
    }
    void add_edge(int u,int v){
        if(directed){
            ++G[u][v], --G[v][u];
            ++degree[u], --degree[v];
        }else{
            ++G[u][v], ++G[v][u];
            ++degree[u], ++degree[v];
        }
    }
    void dfs(int u)
    {
        rep(i,V){
            if(G[u][i] > 0){
                if(directed){
                    --G[u][i],++G[i][u];
                }else{
                    --G[u][i],--G[i][u];
                }
                dfs(i);
            }
        }
        path.push_back(u);
    }
    bool solve()
    {
        int s=-1,t=-1;
        if(directed){
            rep(i,V){
                if(degree[i] != 0){
                    if(degree[i] == 1){
                        if(s >= 0){
                            return false;
                        }
                        s = i;
                    }else if(degree[i] == -1){
                        if(t >= 0){
                            return false;
                        }
                        t = i;
                    }else{
                        return false;
                    }
                }
            }
        }else{
            rep(i,V){
                if(degree[i] % 2){
                    if(s < 0){
                        s = i;
                    }else if(t < 0){
                        t = i;
                    }else{
                        return false;
                    }
                }
            }
        }
        if(s < 0) s = 0;
        dfs(s);
        reverse(path.begin(), path.end());
        return true;
    }
};
