#include "header.hpp"

//最小シュタイナー木を求めるアルゴリズム(Dreyfus-Wagnerのアルゴリズム)
//計算量はグラフの頂点数n,ターミナルの数をtとしたとき O(n*3^t) である
//verifyはしていません(verifyの問題を知らない)
template <typename T> class SteinerTree{
private:
    struct edge {
        int to;
        T cost;
    };
    int V;
    vector<vector<edge> > G;
    vector<vector<T> > dist;
public:
    SteinerTree(int node_size) : V(node_size), G(V), dist(V, vector<T>(V, numeric_limits<T>::max()/10)){
        rep(i,V) dist[i][i] = 0;
    }
    void add_edge(int u, int v, T cost){
        G[u].emplace_back(v,cost), G[v].emplace_back(u,cost);
    }
    T solve(vector<int>& terminal){
        rep(i,V){
            for(auto& e : G[i]){
                dist[i][e.to] = min(dist[i][e.to], e.cost);
            }
        }
        rep(k,V){
            rep(i,V){
                rep(j,V){
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
        int sz = (int)terminal.size();
        vector<vector<T> > opt(1 << sz, vector<T>(V, numeric_limits<T>::max()/10));
        rep(i,sz){
            rep(j,V){
                opt[1 << i][j] = dist[terminal[i]][j];
                opt[0][j] = 0;
            }
        }
        for(int i = 1; i < (1 << sz); i++){
            for(int j = i & -i; j < i; j = (j-i)&i){
                rep(k,V){
                    opt[i][k] = min(opt[i][k], opt[j][k] + opt[i^j][k]);
                }
            }
            rep(j,V){
                rep(k,V){
                    opt[i][k] = min(opt[i][k], opt[i][j] + dist[j][k]);
                }
            }
        }
        return *min_element(opt[(1 << sz) - 1].begin(), opt[(1 << sz) - 1].end());
    }
};
