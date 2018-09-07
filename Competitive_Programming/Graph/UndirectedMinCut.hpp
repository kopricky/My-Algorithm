#include "../header.hpp"

//無向グラフの最小カット(n回max_flowを回すより高速)
//Nagamochi and Ibaraki 1992 Stoer and Wagner 1997 を参照
//フィボナッチヒープによる実装を行っていないので計算量はO(n^3)
//縮約された頂点を持てば最小カットを構成する頂点集合を得ることが可能
//verifyしていません(verify問題を知らない)
template<typename T> class UndirectedMinCut
{
public:
    int V;
    vector<vector<T> > G;
    vector<int> order;
    UndirectedMinCut(int node_size) : V(node_size), G(V, vector<T>(V,0)), order(V){
        iota(order.begin(),order.end(),0);
    }
    void add_edge(int u,int v,T cap){
        G[u][v] += cap, G[v][u] += cap;
    }
    T solve(){
        T ans = numeric_limits<T>::max();
        for(int i = V; i > 1; i--){
            vector<T> val(i,0);
            vector<int> new_order(i-1);
            for(int j = 0; j < i-1; j++){
                int mx_id = max_element(val.begin(),val.end()) - val.begin();
                new_order[j] = order[mx_id];
                val[mx_id] = -1;
                for(int k = 0; k < i; k++){
                    if(val[k] >= 0){
                        val[k] += G[order[mx_id]][order[k]];
                    }
                }
            }
            int last = max_element(val.begin(),val.end()) - val.begin();
            int last_ver = order[last];
            int nx_last = new_order.back();
            for(int j = 0; j < (int)new_order.size()-1; j++){
                G[nx_last][new_order[j]] += G[last_ver][new_order[j]];
                G[new_order[j]][nx_last] += G[new_order[j]][last_ver];
            }
            order = new_order;
            ans = min(ans,val[last]);
        }
        return ans;
    }
};
