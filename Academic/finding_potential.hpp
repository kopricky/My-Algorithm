#include "./header.hpp"

template<typename _Tp> class FindingPotential {
public:
    static_assert(std::is_integral<_Tp>::value, "Integral required.");
private:
    struct base_edge {
        int to;
        _Tp cost;
        base_edge(const int _to, const _Tp _cost) : to(_to), cost(_cost){}
    };
    struct edge : base_edge {
        _Tp roundup;
        edge(const int _to, const _Tp _cost) : base_edge(_to, _cost), roundup(0){}
    };
    const int V;
    int E, comp_count, head;
    _Tp min_cost;
    vector<vector<edge> > G;
    vector<vector<base_edge> > graph;
    vector<vector<int> > dag, que;
    vector<int> ord, low, st, cmp, repr, dist, prv, ng, layer_cnt;
    int compute_roundup(){
        int cnt = 0;
        _Tp tmp = -min_cost;
        while(tmp >= 2) tmp >>= 1, ++cnt;
        if(cnt > 0){
            for(int i = 0; i < V; ++i){
                for(edge& e : G[i]){
                    if(e.cost < 0){
                        _Tp tmp = -e.cost;
                        for(int j = 0; j < cnt; ++j){
                            e.roundup <<= 1, e.roundup |= (tmp & 1);
                            tmp >>= 1;
                        }
                        e.cost = -tmp;
                    }else{
                        for(int j = 0; j < cnt; ++j){
                            e.roundup <<= 1, e.roundup |= (e.cost & 1);
                            e.cost = ((e.cost + 1) >> 1);
                        }
                    }
                }
            }
        }
        return cnt;
    }
    void scc_dfs(const int u, int& tm){
        ord[u] = low[u] = tm++, st[++head] = u;
        for(const edge& e : G[u]){
            if(e.cost + potential[u] - potential[e.to] > 0) continue;
            const int v = e.to;
            if(ord[v] < 0){
                scc_dfs(v, tm);
                low[u] = min(low[u], low[v]);
            }else if(ord[v] < V){
                low[u] = min(low[u], ord[v]);
            }
        }
        if(ord[u] == low[u]){
            while(true){
                const int v = st[head--];
                ord[v] = V, cmp[v] = comp_count;
                if(v == u) break;
            }
            repr[comp_count++] = u;
        }
    }
    void strongly_connected_component(){
        comp_count = 0, head = -1;
        fill(ord.begin(), ord.end(), -1);
        int tm = 0;
        for(int i = 0; i < V; ++i){
            if(ord[i] < 0) scc_dfs(i, tm);
        }
    }
    bool construct_dag(bool& finish){
        for(int i = 0; i < comp_count; ++i) dag[i].clear();
        bool minus_edge = false;
        for(int i = 0; i < V; ++i){
            for(const edge& e : G[i]){
                const _Tp edge_cost = e.cost + potential[i] - potential[e.to];
                if(cmp[i] == cmp[e.to]){
                    if(edge_cost < 0) return false;
                }else{
                    if(edge_cost == 0) dag[cmp[i]].emplace_back(cmp[e.to]);
                    else if(edge_cost < 0){
                        dag[cmp[i]].emplace_back(cmp[e.to] + V);
                        minus_edge = true;
                    }
                }
            }
        }
        if(!minus_edge) finish = true;
        return true;
    }
    bool contract_graph(bool& finish){
        strongly_connected_component();
        return construct_dag(finish);
    }
    void graph_layering(){
        fill(dist.begin(), dist.begin() + comp_count, 0);
        fill(ng.begin(), ng.begin() + comp_count, 0);
        for(int i = comp_count - 1; i >= 0; --i){
            for(const int v : dag[i]){
                const int w = ((v < V) ? v : v - V);
                ng[w] |= (v >= V);
                if(dist[w] > dist[i] + (v < V) - 1){
                    dist[w] = dist[i] + (v < V) - 1, prv[w] = i;
                }
            }
        }
    }
    void cancel_layer(const int layer_num){
        for(int i = 0; i < V; ++i){
            potential[i] -= (-dist[cmp[i]] >= layer_num);
        }
    }
    bool cancel_path(const int max_len){
        for(int i = 0; i < max_len; ++i){
            for(int j = 0; j < (int)que[i].size(); ++j){
                const int u = que[i][j];
                if(dist[u] < i) continue;
                for(const edge& e : G[u]){
                    const _Tp edge_cost = e.cost + potential[u] - potential[e.to];
                    const _Tp edge_cost_plus = (edge_cost >= 0) ? edge_cost : 0;
                    if(edge_cost < 0){
                        if(prv[cmp[e.to]] < 0 && dist[e.to] >= dist[u]) return false;
                    }else if(dist[e.to] > dist[u] + edge_cost_plus){
                        dist[e.to] = dist[u] + edge_cost_plus;
                        que[dist[e.to]].push_back(e.to);
                    }
                }
            }
        }
        for(int i = 0; i < V; ++i) potential[i] += dist[i];
        return true;
    }
    bool cancel_negative_edges(){
        fill(layer_cnt.begin(), layer_cnt.begin() + comp_count, 0);
        int max_len = 0, max_vertex = 0, max_cnt = 0, max_layer = 0;
        for(int i = 0; i < comp_count; ++i){
            if(dist[i] < 0 && ng[i] > 0 && max_cnt < ++layer_cnt[-dist[i]]){
                ++max_cnt, max_layer = -dist[i];
            }
            if(max_len < -dist[i]){
                max_len = -dist[i], max_vertex = i;
            }
        }
        if((long long)(4 * E + 3 * max_len + V) * max_cnt >= (long long)V * max_len){
            cancel_layer(max_layer);
            return true;
        }else{
            fill(dist.begin(), dist.end(), max_len);
            int nx = -1;
            for(int i = max_len; i > 0; --i){
                dist[repr[max_vertex]] = max_len - i;
                que[max_len - i] = {repr[max_vertex]};
                nx = prv[max_vertex];
                prv[max_vertex] = -1;
                max_vertex = nx;
            }
            cancel_path(max_len);
            return true;
        }
    }
    bool solve(){
        bool finish = false;
        while(!finish){
            if(!contract_graph(finish)) return false;
            if(finish) return true;
            graph_layering();
            if(!cancel_negative_edges()) return false;
        }
        return true;
    }
    void change_cost(){
        _Tp min_potential = *min_element(potential.begin(), potential.end());
        for(int i = 0; i < V; ++i){
            potential[i] = 2 * (potential[i] - min_potential);
            for(edge& e : G[i]){
                e.cost = e.cost * 2 - (e.roundup & 1), e.roundup >>= 1;
            }
        }
    }

public:
    vector<_Tp> potential;
    FindingPotential(const int node_size)
        : V(node_size), E(0), min_cost(0), G(V), graph(V), dag(V), que(V),
            ord(V), low(V), st(V), cmp(V), repr(V), dist(V), prv(V, 0), ng(V),
                layer_cnt(V), potential(V, 0){}
    void add_edge(const int u, const int v, const _Tp cost){
        G[u].emplace_back(v, cost), ++E;
        min_cost = min(min_cost, cost);
    }
    bool compute_potential(){
        if(min_cost < 0){
            int num = compute_roundup();
            for(; num >= 0; --num){
                if(!solve()) return false;
                if(num > 0) change_cost();
            }
        }
        return true;
    }
    // DEBUG
    bool check(){
        for(int i = 0; i < V; ++i){
            for(const edge& e : G[i]){
                if(e.cost + potential[i] - potential[e.to] < 0) return false;
            }
        }
        return true;
    }
};
