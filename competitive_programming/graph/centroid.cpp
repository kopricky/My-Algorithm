class centroid
{
private:
    vector<vector<int> > G;
    vector<int> sz;
    int V;
public:
    centroid(int node_size){
        V = node_size;
        G.resize(V),sz.resize(V,0);
    }
    void add_edge(int u,int v){
        G[u].push_back(v),G[v].push_back(u);
    }
    void dfs(int u,int p){
        sz[u] = 1;
        for(int v : G[u]){
            if(v != p){
                dfs(v,u);
                sz[u] += sz[v];
            }
        }
    }
    int solve(){
        int u = 0;
        dfs(u,-1);
        int tot = sz[u];
        bool ok = false;
        int p = -1;
        while(!ok){
            ok = true;
            for(int v : G[u]){
                if(v != p && 2*sz[v] > tot){
                    p = u, u = v, ok = false;
                    break;
                }
            }
        }
        return u;
    }
};
