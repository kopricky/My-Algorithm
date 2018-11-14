#include "header.hpp"

//最小シュタイナー木を求めるアルゴリズム(Dreyfus-Wagner のアルゴリズムをターミナル数について FPT 版に改良)
//計算量はグラフの頂点数を n, ターミナルの数を t としたとき O(n 3^t + (n+m)logn 2^t)
//軽く verify はした

template<typename T> class SteinerTree {
private:
    struct edge {
        int to;
        T cost;
    };
    int V;
    vector<vector<edge> > G;
    vector<vector<T> > dp;
    const T inf = numeric_limits<T>::max() / 10;
    using pti = pair<T, int>;
    
public:
    SteinerTree(int node_size) : V(node_size), G(V){}
    void add_edge(int u, int v, T cost){
        G[u].push_back((edge){v, cost}), G[v].push_back((edge){u, cost});
    }
    T solve(vector<int>& terminal){
        int t = (int)terminal.size();
        if(t == 0) return (T)0;
        dp.resize((1 << t), vector<T>(V, inf));
        for(int i = 0; i < t; i++){
            dp[(1 << i)][terminal[i]] = 0;
        }
        for(int i = 1; i < (1 << t); i++){
            for(int j = 0; j < V; j++){
                for(int k = i; k > 0; k = (k - 1) & i){
                    dp[i][j] = min(dp[i][j], dp[k][j] + dp[i ^ k][j]);
                }
            }
            if(i == (1 << t) - 1) break;
            priority_queue<pti, vector<pti>, greater<pti> > que;
            for(int j = 0; j < V; j++){
                que.push(make_pair(dp[i][j], j));
            }
            while(!que.empty()){
                pti p = que.top();
                que.pop();
                int v = p.second;
                if(dp[i][v] < p.first) continue;
                for(auto& e : G[v]){
                    if(dp[i][e.to] > dp[i][v] + e.cost){
                        dp[i][e.to] = dp[i][v] + e.cost;
                        que.push(make_pair(dp[i][e.to], e.to));
                    }
                }
            }
        }
        return dp[(1 << t) - 1][terminal[0]];
    }
};