#include "../header.hpp"

//DAGになっている場合の最小費用流(ポテンシャルをトポソDPを用いて計算)
//最小費用がint,頂点数がn
//min_cost_flow<int> mcf(n);
//適宜add_edge
//mcf.solve(始点,終点,流量)で最小費用流を計算
template<typename CapType, typename CostType> class MinCostFlowDAG {
public:
    using Cat = CapType;
    using Cot = CostType;
    using pti = pair<Cot, int>;
    struct edge {
        int to, rev;
        Cat cap;
        Cot cost;
    };
    const int V;
    const Cot inf;
    vector<vector<edge> > G;
    vector<Cot> h, dist;
    vector<int> deg, ord, prevv, preve;
    MinCostFlowDAG(const int node_size) : V(node_size), inf(numeric_limits<Cot>::max() / 4),
        G(V), h(V, inf), dist(V), deg(V, 0), prevv(V), preve(V){}
    void add_edge(const int from, const int to, const Cat cap, const Cot cost){
        G[from].push_back((edge){to, (int)G[to].size(), cap, cost});
        G[to].push_back((edge){from, (int)G[from].size() - 1, 0, -cost});
        ++deg[to];
    }
    bool tsort(){
        queue<int> que;
        for(int i = 0; i < V; ++i){
            if(deg[i] == 0) que.push(i);
        }
        while(!que.empty()){
            const int p = que.front();
            que.pop();
            ord.push_back(p);
            for(auto& e : G[p]){
                if(e.cap > 0 && --deg[e.to] == 0) que.push(e.to);
            }
        }
        return (*max_element(deg.begin(), deg.end()) == 0);
    }
    void calc_potential(const int s){
        h[s] = 0;
        for(const int v : ord){
            if(h[v] == inf) continue;
            for(edge& e : G[v]){
                if(e.cap > 0) h[e.to] = min(h[e.to], h[v] + e.cost);
            }
        }
    }
    void Dijkstra(const int s){
        priority_queue<pti,vector<pti>,greater<pti> > que;
        fill(dist.begin(), dist.end(), inf);
        dist[s] = 0;
        que.push(pti(0, s));
        while(!que.empty()){
            pti p = que.top();
            que.pop();
            const int v = p.second;
            if(dist[v] < p.first) continue;
            for(int i = 0; i < (int)G[v].size(); ++i){
                edge& e = G[v][i];
                if(e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]){
                    dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                    prevv[e.to] = v, preve[e.to] = i;
                    que.push(pti(dist[e.to], e.to));
                }
            }
        }
    }
    void update(const int s, const int t, Cat& f, Cot& res){
        for(int i = 0; i < V; i++){
            h[i] += dist[i];
        }
        Cat d = f;
        for(int v = t; v != s; v = prevv[v]){
            d = min(d, G[prevv[v]][preve[v]].cap);
        }
        f -= d;
        res += h[t] * d;
        for(int v = t; v != s; v = prevv[v]){
            edge& e = G[prevv[v]][preve[v]];
            e.cap -= d;
            G[v][e.rev].cap += d;
        }
    }
    Cot solve(const int s, const int t, Cat f){
        if(!tsort()) assert(false); // not DAG
        calc_potential(s);
        Cot res = 0;
        while(f > 0){
            Dijkstra(s);
            if(dist[t] == inf) return -inf;
            update(s, t, f, res);
        }
        return res;
    }
};
