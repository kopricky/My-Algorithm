#include "../header.hpp"

class CountingC4 {
private:
    int V, threshold;
    vector<vector<int> > G;
    vector<vector<array<int, 2> > > memo;
    vector<int> flag1, flag2;
    void process_high_degree(long long& ans){
        for(int i = 0; i < V; ++i){
            if((int)G[i].size() <= threshold) continue;
            for(const int u : G[i]){
                if(u > i) flag1[u] = 1;
                flag2[u] = 1;
            }
            for(int j = 0; j < i; ++j){
                if((int)G[j].size() > threshold) continue;
                long long cnt1 = 0, cnt2 = 0;
                for(const int u : G[j]){
                    if(u < j || !flag2[u]) continue;
                    if((int)G[u].size() > threshold) ++cnt1;
                    else ++cnt2; 
                }
                ans += (cnt1 + cnt2) * (cnt1 + cnt2 - 1) / 2;
            }
            for(int j = i + 1; j < V; ++j){
                long long cnt = 0;
                for(const int u : G[j]){
                    if(flag1[u]) ++cnt;
                }
                ans += cnt * (cnt - 1) / 2;
            }
            for(const int u : G[i]) flag1[u] = flag2[u] = 0;
        }
    }
    void process_low_degree(long long& ans){
        for(int i = 0; i < V; ++i){
            if((int)G[i].size() > threshold) continue;
            for(const int u : G[i]){
                for(const int v : G[i]){
                    if(v > u) memo[u].push_back({i, v});
                }
            }
        }
        for(int i = 0; i < V; ++i){
            for(const auto& e : memo[i]){
                if(e[0] < i) ++flag1[e[1]];
                else ++flag2[e[1]];
            }
            for(const auto& e : memo[i]){
                ans += (long long)(flag1[e[1]] + 2 * flag2[e[1]] - 1) * flag1[e[1]] / 2;
                flag1[e[1]] = flag2[e[1]] = 0;
            }
        }
    }
public:
    CountingC4(const int node_size)
         : V(node_size), threshold(0), G(V), memo(V), flag1(V, 0), flag2(V, 0){}
    void add_edge(const int u, const int v){
        G[u].push_back(v), G[v].push_back(u);
        ++threshold;
    }
    long long solve(){
        threshold = floor(sqrt(2 * threshold)) / 2;
        long long ans = 0;
        process_high_degree(ans);
        process_low_degree(ans);
        return ans;
    }
};
