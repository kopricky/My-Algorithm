#include "../header.hpp"

//彩色多項式の値を包除原理で計算
//最小クリーク被覆は補グラフの彩色数を考える
class ChromaticrNumber {
public:
    int V;
    vector<int> adj;
    const static int MOD = 1000000007;
    ChromaticrNumber(int node_size) : V(node_size), adj(V, 0){
        for(int i = 0; i < V; i++){
            adj[i] = (1 << i);
        }
    }
    void add_edge(int u, int v){
        adj[u] |= (1 << v), adj[v] |= (1 << u);
    }
    int solve(){
        vector<int> t(1 << V, 0), I(1 << V, 0);
        t[(1 << V) - 1] = I[0] = 1;
        for(int i = 1; i < (1 << V); i++){
            int v = __builtin_ctz(i);
            I[i] = I[i ^ (1 << v)] + I[i & (~adj[v])];
            I[i] = (I[i] >= MOD) ? (I[i] - MOD) : I[i];
            t[i-1] = (((V - __builtin_popcount(i - 1)) % 2) ? (MOD - 1) : 1);
        }
        for(int k = 1; k < V; k++){
            long long res = 0;
            for(int i = 0; i < (1 << V); i++){
                res += (t[i] = (long long)t[i] * I[i] % MOD);
            }
            if(res % MOD) return k;
        }
        return V;
    }
};
