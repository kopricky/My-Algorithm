#include "./header.hpp"

template<class edge> class Dinic {
private:
    using Cat = decltype(edge::cap);
    using Cot = decltype(edge::cost);
    vector<vector<edge> >& G;
    const int V;
    vector<int> level, iter, que;
    void bfs(const int s) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        int qh = 0, qt = 0;
        for(que[qt++] = s; qh < qt;){
            const int v = que[qh++];
            for(const edge& e : G[v]){
                if(e.cap > 0 && level[e.to] < 0){
                    level[e.to] = level[v] + 1, que[qt++] = e.to;
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
                    e.cap -= d, G[e.to][e.rev].cap += d;
                    res += d * e.cost;
                    return d;
                }
            }
        }
        return 0;
    }
public:
    Dinic(vector<vector<edge> >& _G)
        : G(_G), V((int)G.size()), level(V), iter(V), que(V){}
    Cot solve(const int s, const int t, Cat f){
        Cot res = 0;
        while(true){
            bfs(s);
            if(level[t] < 0) return numeric_limits<Cot>::max();
            fill(iter.begin(), iter.end(), 0);
            Cat df;
            while((df = dfs(s, t, f, res))){
                if((f -= df) == 0) return res;
            }
        }
    }
};

template<typename CapType, typename CostType> class MinCostFlow {
public:
    using Cat = CapType;
    using Cot = CostType;
    static_assert(std::is_integral<CostType>::value, "Integral required.");
    struct edge;
private:
    const int V;
    int s, t, checker;
    CostType epsilon, factor, total_cost;
    vector<CapType> excess;
    vector<CostType> potential;
    vector<int> cur_edge;
    queue<int> act_ver;
    bool compute_initial_flow(const Cat f){
        Dinic<edge> dn(G);
        total_cost = dn.solve(s, t, f);
        return (total_cost != numeric_limits<Cot>::max());
    }
    void refine(const int u){
        for(int& i = cur_edge[u]; i < (int)G[u].size(); ++i){
            edge& e = G[u][i];
            if(potential[u] + e.cost * factor < potential[e.to] && e.cap > 0){
                if(push(u, e)) return;
            }
        }
        return relabel(u);
    }
    bool push(const int u, edge& e){
        const Cat f = min(e.cap, excess[u]);
        const int v = e.to;
        e.cap -= f, excess[u] -= f;
        G[v][e.rev].cap += f, excess[v] += f;
        total_cost += f * e.cost;
        if(excess[v] > 0 && excess[v] <= f) act_ver.push(v);
        return (excess[u] == 0);
    }
    void relabel(const int u){
        ++checker;
        Cot cur = numeric_limits<Cot>::min();
        for(const edge& e : G[u]){
            if(cur < potential[e.to] - e.cost * factor && e.cap > 0){
                cur = potential[e.to] - e.cost * factor;
            }
        }
        potential[u] = cur - epsilon, cur_edge[u] = 0;
        act_ver.push(u);
    }
public:
    struct edge {
        const int to, rev;
        CapType cap;
        CostType cost;
        edge(const int _to, const int _rev, const CapType _cap, const CostType _cost)
            : to(_to), rev(_rev), cap(_cap), cost(_cost){}
    };
    vector<vector<edge> > G;
    MinCostFlow(const int node_size)
        : V(node_size), epsilon(0), factor(1), total_cost(0), excess(V, 0),
            potential(V, 0), cur_edge(V), G(V){}
    void add_edge(const int _from, const int _to, const Cat _cap, const Cot _cost){
        G[_from].emplace_back(_to, (int)G[_to].size(), _cap, _cost);
        G[_to].emplace_back(_from, (int)G[_from].size() - 1, (Cat)0, -_cost);
        epsilon = max(epsilon, abs(_cost));
    }
    Cot solve(const int source, const int sink, const Cat flow){
        s = source, t = sink;
        if(!compute_initial_flow(flow)) return total_cost;
        int index = 0;
        for(; V * epsilon >= factor; ++index) factor *= 2;
        epsilon *= factor;
        for(; index > 0; --index){
            epsilon /= 2;
            for(int i = 0; i < V; ++i){
                for(edge& e : G[i]){
                    if(potential[i] + e.cost * factor < potential[e.to] && e.cap > 0){
                        total_cost += e.cap * e.cost;
                        excess[e.to] += e.cap, G[e.to][e.rev].cap += e.cap;
                        excess[i] -= e.cap, e.cap = 0;
                    }
                }
            }
            for(int i = 0; i < V; ++i){
                if(excess[i] > 0) act_ver.push(i);
            }
            while(!act_ver.empty()){
                const int u = act_ver.front();
                act_ver.pop();
                refine(u);
            }
        }
        return total_cost;
    }
};