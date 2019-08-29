#include "./header.hpp"

// 負の有向閉路が存在する場合の最小費用流問題を解くアルゴリズム.
// 計算量 O(min(m^2 n^2 log(n |cost_min|), m^3 n^2 log n)

template<class edge> class Dinic {
private:
    using Cat = decltype(edge::cap);
    using Cot = decltype(edge::cost);
    vector<vector<edge> >& G;
    const int V;
    vector<int> level, iter;
    void bfs(const int s){
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while(!que.empty()){
            const int v = que.front();
            que.pop();
            for(const auto& e : G[v]){
                if(e.cap > 0 && level[e.to] < 0){
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }
    Cat dfs(const int v, const int t, const Cat f, Cot& res){
        if(v == t) return f;
        for(int& i = iter[v]; i < (int)G[v].size(); ++i){
            edge& e = G[v][i];
            if(e.cap > 0 && level[v] < level[e.to]){
                Cat d = dfs(e.to, t, min(f, e.cap), res);
                if(d > 0){
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    res += d * e.cost;
                    return d;
                }
            }
        }
        return 0;
    }

public:
    Dinic(vector<vector<edge> >& _G) : G(_G), V((int)G.size()), level(V), iter(V){}
    Cot solve(const int s, const int t, Cat f){
        Cot res = 0;
        for(;;){
            bfs(s);
            if(level[t] < 0) return numeric_limits<Cot>::min();
            fill(iter.begin(), iter.end(), 0);
            while(true){
                Cat df = dfs(s, t, f, res);
                if(df > 0){
                    f -= df;
                    if(f == 0) return res;
                }else{
                    break;
                }
            }
        }
    }
};

template<typename CapType, typename CostType> class MinCostFlow
{
public:
    struct edge;
private:
    using Cat = CapType;
    using Cot = CostType;
    const int V;
    const Cot inf;
    vector<vector<Cot> > dp;
    vector<vector<edge*> > prev;
    pair<int, int> minimum_mean_cycle(){
        for(int k = 0; k < V; ++k){
            for(int i = 0; i < V; ++i){
                dp[k+1][i] = inf;
            }
            for(int i = 0; i < V; ++i){
                for(edge& e : G[i]){
                    if(e.cap > 0 && dp[k+1][e.to] > dp[k][i] + e.cost){
                        dp[k+1][e.to] = dp[k][i] + e.cost;
                        prev[k+1][e.to] = &e;
                    }
                }
            }
        }
        if(*min_element(dp[V].begin(), dp[V].end()) == inf) return make_pair(-1, -1);
        int start = -1;
        Cot tnum = inf / V;
        int tden = 1;
        for(int i = 0; i < V; ++i){
            if(dp[V][i] == inf) continue;
            Cot num = -inf / V;
            int den = 1;
            for(int k = 0; k < V; ++k){
                if(dp[k][i] == inf) continue;
                if(num * (V-k) <= den * (dp[V][i] - dp[k][i])){
                    num = dp[V][i] - dp[k][i], den = V - k;
                }
            }
            if(num > -inf / V){
                if((num * tden < den * tnum) || (num * tden == den * tnum && den < tden)){
                    tnum = num, tden = den;
                    start = i;
                }
            }
        }
        return (tnum < 0) ? make_pair(start, tden) : make_pair(-1, -1);
    }
    bool update(Cot& res){
        int level = V, start, length;
        pair<int, int> info = minimum_mean_cycle();
        start = info.first, length = info.second;
        if(start < 0) return false;
        Cat df = numeric_limits<Cat>::max();
        for(int i = 0; i < length; ++i){
            edge *e = prev[level--][start];
            df = min(df, e->cap);
            start = G[e->to][e->rev].to;
        }
        level = V;
        for(int i = 0; i < length; ++i){
            edge *e = prev[level--][start];
            edge *reve = &G[e->to][e->rev];
            e->cap -= df, reve->cap += df, res += df * e->cost;
            start = reve->to;
        }
        return true;
    }
public:
    struct edge {
        int to; Cat cap; Cot cost; int rev;
    };
    vector<vector<edge> > G;
    MinCostFlow(const int node_size)
     : V(node_size), inf(numeric_limits<Cot>::max()),
        dp(V+1, vector<Cat>(V, 0)), prev(V+1, vector<edge*>(V)), G(V){}
    void add_edge(const int from, const int to, const Cat cap, const Cot cost){
        G[from].push_back((edge){to, cap, cost, (int)G[to].size()});
        G[to].push_back((edge){from, 0, -cost, (int)G[from].size() - 1});
    }
    Cot solve(const int s, const int t, Cat f){
        Dinic<edge> dn(G);
        Cot res = dn.solve(s, t, f);
        if(res == numeric_limits<Cot>::min()) return res;
        while(update(res));
        return res;
    }
};
