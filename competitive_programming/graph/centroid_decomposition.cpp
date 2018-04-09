class CentroidDecomposition
{
public:
    vector<vector<int> > G;
    vector<bool> used;
    //sz:重心分解後の最大部分木に含まれる頂点の数(自分を含める)
    //par:重心分解後の親の頂点
    vector<int> sz,par;
    //部分木に含まれる頂点
    int V;
    CentroidDecomposition(int node_size){
        V = node_size;
        G.resize(V);
    }
    void add_edge(int u,int v){
        G[u].push_back(v),G[v].push_back(u);
    }
    //部分木のサイズを計算
    void calcSize(int u,int p){
        sz[u] = 1;
        for(int v : G[u]){
            if(!used[v] && v != p){
                calcSize(v,u);
                sz[u] += sz[v];
            }
        }
    }
    void cdBuild(int u,int p){
        calcSize(u,-1);
        int tot = sz[u];
        bool ok = false;
        int pp = -1;
        //いま見ている部分木での重心を見つける
        while(!ok){
            ok = true;
            for(int v : G[u]){
                if(!used[v] && v != pp && 2*sz[v] > tot){
                    pp = u, u = v, ok = false;
                    break;
                }
            }
        }
        par[u] = p;
        //何らかの操作
        used[u] = true;
        //深さ優先でたどる
        for(int v : G[u]){
            if(!used[v]){
                cdBuild(v,u);
            }
        }
    }
    void build(){
        used.resize(V,false),sz.resize(V,0),par.resize(V,-1);
        cdBuild(0,-1);
    }
};
