#include "../header.hpp"

template<typename T>
class DirectedMinimumMeanWeightCycle
{
private:
    struct edge {
        int to;
        T cost;
    };
    const int V;
    const T inf;
    vector<vector<edge> > G;
    vector<vector<T> > dp;
    vector<vector<int> > prev;
public:
    vector<int> ans;
    DirectedMinimumMeanWeightCycle(const int node_size)
        : V(node_size), inf(numeric_limits<T>::max()),
            G(V), dp(V + 1, vector<T>(V, 0)), prev(V + 1, vector<int>(V)){}
    void add_edge(const int from, const int to, const T cost){
        G[from].push_back((edge){to, cost});
    }
    pair<T, int> solve(){
        for(int k = 0; k < V; ++k){
            for(int i = 0; i < V; ++i){
                dp[k + 1][i] = inf;
            }
            for(int i = 0; i < V; ++i){
                if(dp[k][i] == inf) continue;
                for(const edge& e : G[i]){
                    if(dp[k + 1][e.to] > dp[k][i] + e.cost){
                        dp[k + 1][e.to] = dp[k][i] + e.cost;
                        prev[k + 1][e.to] = i;
                    }
                }
            }
        }
        if(*min_element(dp[V].begin(), dp[V].end()) == inf) return make_pair(-inf, -1);
        int start = -1;
        T tnum = inf / V;
        int tden = 1;
        for(int i = 0; i < V; ++i){
            if(dp[V][i] == inf) continue;
            T num = -inf / V;
            int den = 1;
            for(int k = 0; k < V; ++k){
                if(dp[k][i] != inf && num * (V - k) <= den * (dp[V][i] - dp[k][i])){
                    num = dp[V][i] - dp[k][i], den = V - k;
                }
            }
            if(num * tden < den * tnum){
                tnum = num, tden = den, start = i;
            }
        }
        vector<int> route, used(V, -1);
        int level = V;
        while(true){
            if(used[start] >= 0){
                for(int i = (int)route.size() - 1; i >= used[start]; --i){
                    ans.push_back(route[i]);
                }
                break;
            }
            used[start] = (int)route.size();
            route.push_back(start);
            start = prev[level--][start];
        }
        return {tnum * (int)ans.size() / tden, (int)ans.size()};
    }
};
