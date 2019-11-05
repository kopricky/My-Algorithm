#include "../header.hpp"

//負の辺が存在しない場合の最小費用流(Dijkstraでポテンシャルを計算可能)
//最小費用がint,頂点数がn
//MinCostFlow<int> mcf(n);
//適宜add_edge
//mcf.solve(始点,終点,流量)で最小費用流を計算
template<typename CapType, typename CostType> class MinCostFlow {
public:
    using Cat = CapType;
    using Cot = CostType;
    using pti = pair<Cot, int>;
    struct edge {
        int to; Cat cap; Cot cost; int rev;
    };
    const int V;
    const Cot inf;
    vector<vector<edge> > G;
    vector<Cot> h, dist;
    vector<int> prevv, preve;
    MinCostFlow(int node_size) : V(node_size), inf(numeric_limits<Cot>::max() / 4),
        G(V), h(V, 0), dist(V), prevv(V), preve(V){}
    void add_edge(int from, int to, Cat cap, Cot cost){
        G[from].push_back((edge){to, cap, cost, (int)G[to].size()});
        G[to].push_back((edge){from, 0, -cost, (int)G[from].size() - 1});
    }
    Cot solve(int s, int t, Cat f){
        Cot res = 0;
        while(f > 0){
            priority_queue<pti,vector<pti>,greater<pti> > que;
            fill(dist.begin(), dist.end(), inf);
            dist[s] = 0;
            que.push(pti(0, s));
            while(!que.empty()){
                pti p = que.top();
                que.pop();
                int v = p.second;
                if(dist[v] < p.first) continue;
                for(int i = 0; i < (int)G[v].size(); i++){
                    edge& e = G[v][i];
                    if(e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]){
                        dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                        prevv[e.to] = v, preve[e.to] = i;
                        que.push(pti(dist[e.to], e.to));
                    }
                }
            }
            if(dist[t] == inf) return -1;
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
        return res;
    }
};
