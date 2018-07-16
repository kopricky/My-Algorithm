#include "../header.hpp"

//根付きの重心分解(こういう概念が実際に存在するかは知らない)
//重心分解を根付きにすることでパスに対するクエリをHL分解よりも高速に処理できる
//枝iの重みをA[i]*x+B[i] (A[i],B[i]は定数) としたとき
//パス上の枝に対して加算(x→x+a)と総和をO(log(n))で計算する
//verifyしていない(上記のクエリを処理するような問題ありませんかね~)のでバグがないかが不安

template<typename T> class RootedCentroidDecomposition
{
public:
    struct edge {
        int to;
        T A, B;
    };
    int V, LOGV, CentroidRoot;
    vector<vector<edge> > G;
    vector<vector<int> > TreeID, centroid, parent;
    vector<vector<T> > height;
    vector<bool> used;
    vector<int> depth, stsize;
    vector<T> sumA, sumB, gap, plus;
    void FirstDFS(int u, int par, int dep, T valA, T valB){
        sumA[u] = valA, sumB[u] = valB;
        parent[0][u] = par, depth[u] = dep, stsize[u] = 1;
        for(auto& e : G[u]){
            if(!used[e.to] && e.to != par){
                FirstDFS(e.to, u, dep + 1, valA + e.A, valB + e.B);
                stsize[u] += stsize[e.to];
            }
        }
    }
    void CalcSize(int u, int par){
        stsize[u] = 1;
        for(auto& e : G[u]){
            if(!used[e.to] && e.to != par){
                CalcSize(e.to, u);
                stsize[u] += stsize[e.to];
            }
        }
    }
    void CalcRootHeight(int u, int par, const int cent, const int dep, T val){
        TreeID[dep][u] = 0, height[dep][u] = val;
        for(auto& e : G[u]){
            if(!used[e.to] && e.to != par){
                if(TreeID[dep][e.to] == 0){
                    CalcRootHeight(e.to, u, cent, dep, val + e.A);
                }else{
                    CalcRootHeight(e.to, u, cent, dep, val);
                }
            }
        }
    }
    void CalcHeight(int u, int par, const int ID, const int dep, const T val){
        TreeID[dep][u] = ID, height[dep][u] = val;
        for(auto& e : G[u]){
            if(!used[e.to] && e.to != par){
                CalcHeight(e.to, u, ID, dep, val);
            }
        }
    }
    void RecBuild(int r, int par, int ID, int dep){
        if(par < 0){
            FirstDFS(r, -1, 0, 0, 0);
        }else{
            CalcSize(r, -1);
        }
        int sz = stsize[r], p = -1;
        bool ok = false;
        int cent = r;
        while(!ok){
            ok = true;
            for(auto& e : G[cent]){
                if(!used[e.to] && e.to != p && 2*stsize[e.to] > sz){
                    p = cent, cent = e.to, ok = false;
                    break;
                }
            }
        }
        if(par >= 0){
            centroid[par][ID] = cent;
        }else{
            CentroidRoot = cent;
        }
        int nwnode = cent;
        while(nwnode != r){
            TreeID[dep][nwnode] = 0;
            nwnode = parent[0][nwnode];
        }
        used[cent] = true;
        T diff = r?(sumA[r] - sumA[parent[0][r]]):0;
        height[dep][cent] = sumA[cent] - sumA[r] + diff;
        int childID = 1;
        for(auto& e : G[cent]){
            if(!used[e.to]){
                if(e.to == parent[0][cent]){
                    CalcRootHeight(r, -1, cent, dep, diff);
                }else{
                    CalcHeight(e.to, -1, childID++, dep, height[dep][cent]);
                }
            }
        }
        centroid[cent].resize(childID, -1);
        childID = 1;
        for(auto& e : G[cent]){
            if(!used[e.to]){
                if(e.to == parent[0][cent]){
                    RecBuild(r, cent, 0, dep+1);
                }else{
                    RecBuild(e.to, cent, childID++, dep+1);
                }
            }
        }
    }
    int LCA(int u, int v){
        if(depth[u] > depth[v]) swap(u,v);
        for(int i = 0; i < LOGV; i++){
            if((depth[v] - depth[u]) & (1 << i)){
                v = parent[i][v];
            }
        }
        if(u == v) return u;
        for(int i = LOGV - 1; i >= 0; i--){
            if(parent[i][u] != parent[i][v]){
                u = parent[i][u], v = parent[i][v];
            }
        }
        return parent[0][u];
    }
    void Range(const int u, int cent, int dep, const T val){
        gap[cent] += val * height[dep][u];
        int NextID = TreeID[dep][u];
        if(NextID < 0) return;
        if(NextID != 0){
            plus[cent] += val;
        }
        if(u != cent){
            Range(u, centroid[cent][NextID], dep + 1, val);
        }
    }
    T Query(const int u, int dep, int cent){
        if(u == cent) return gap[cent] + sumB[cent];
        int NextID = TreeID[dep][u];
        if(NextID < 0){
            return sumB[u];
        }
        if(NextID == 0){
            return plus[cent] * height[dep][u] + Query(u, dep + 1, centroid[cent][NextID]);
        }else{
            return gap[cent] + Query(u, dep + 1, centroid[cent][NextID]);
        }
    }
public:
    RootedCentroidDecomposition(const int node_size) : V(node_size), LOGV(ceil(log2(V))),
        CentroidRoot(-1), G(V), TreeID(LOGV, vector<int>(V, -1)), centroid(V), parent(LOGV, vector<int>(V, -1)),
        height(LOGV, vector<T>(V, 0)), used(V, false), depth(V, 0), stsize(V, 0), sumA(V, 0), sumB(V, 0),
        gap(V, 0), plus(V, 0){}
    void add_edge(const int u, const int v, const T a, const T b){
        G[u].push_back((edge){v, a, b}),G[v].push_back((edge){u, a, b});
    }
    void build(){
        RecBuild(0, -1, 0, 0);
        for(int i = 0; i < LOGV - 1; i++){
            for(int j = 0; j < V; j++){
                if(parent[i][j] >= 0){
                    parent[i+1][j] = parent[i][parent[i][j]];
                }
            }
        }
    }
    void range(int u, int v, const T val){
        if(u == v) return;
        Range(u, CentroidRoot, 0, val), Range(v, CentroidRoot, 0, val);
        Range(LCA(u, v), CentroidRoot, 0, -2 * val);
    }
    T query(int u, int v){
        if(u == v) return 0;
        return  Query(u, 0, CentroidRoot) + Query(v, 0, CentroidRoot) - 2 * Query(LCA(u, v), 0, CentroidRoot);
    }
};
