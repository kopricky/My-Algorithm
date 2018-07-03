#include "../header.hpp"

//DAGになっている場合の最小費用流(ポテンシャルをトポソDPを用いて計算)
//最小費用がint,頂点数がn
//min_cost_flow<int> mcf(n);
//適宜add_edge
//mcf.solve(始点,終点,流量)で最小費用流を計算
template<typename T> class min_cost_flow{
public:
    struct edge {
        int to,cap;
        T cost;
        int rev;
        bool is_rev;
    };
	class tsort {
	public:
        vector<vector<edge> >& G;
		vector<int> deg;
	    vector<int> order;
        int V;
	    tsort(vector<vector<edge> >& g) : G{g}{
            V = (int)G.size();
	        deg.resize(V,0);
            rep(i,V){
                for(edge& e : G[i]){
                    if(!e.is_rev){
                        deg[e.to]++;
                    }
                }
            }
        }
        void solve(){
	        queue<int> que;
	        rep(i,V){
	            if(deg[i] == 0){
	                que.push(i);
	            }
	        }
	        while(!que.empty()){
	            int p = que.front();
	            que.pop();
	            order.push_back(p);
	            rep(i,G[p].size()){
	                if(--deg[G[p][i].to] == 0){
	                    que.push(G[p][i].to);
	                }
	            }
	        }
	        if(*max_element(deg.begin(),deg.end()) != 0){
	            order.clear();
	        }
	    }
	};
	using pti = pair<T,int>;
    vector<vector<edge> > G;
	vector<T> h,dist;
	vector<int> prevv,preve;
    tsort ts;
	T inf;
    int V;
    min_cost_flow(int node_size){
        V = node_size;
		inf = numeric_limits<T>::max() / 100;
        G.resize(V), h.resize(V), dist.resize(V), prevv.resize(V), preve.resize(V);
	}
    void add_edge(int from, int to, int cap, T cost){
        G[from].push_back((edge){to, cap, cost, (int)G[to].size(),false});
        G[to].push_back((edge){from, 0, -cost, (int)G[from].size() - 1,true});
    }
    T solve(int s,int t,int f){
        T res = 0;
		fill(h.begin(),h.end(),0);
        tsort ts(G);
        ts.solve();
        rep(i,V){
            h[i] = inf;
        }
        h[s] = 0;
        for(int i : ts.order){
            if(h[i] != inf){
                rep(j,G[i]){
                    h[G[i][j].to] = min(h[G[i][j].to],h[i]+G[i][j].cost);
                }
            }
        }
        while(f > 0){
            priority_queue<pti,vector<pti>,greater<pti> > que;
            fill(dist.begin(),dist.end(),inf);
            dist[s] = 0;
            que.push(pti(0,s));
            while(!que.empty()){
                pti p = que.top();
                que.pop();
                int v = p.second;
                if(dist[v] < p.first){
                    continue;
                }
                rep(i,G[v].size()){
                    edge& e = G[v][i];
                    if(e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]){
                        dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                        prevv[e.to] = v, preve[e.to] = i;
                        que.push(pti(dist[e.to], e.to));
                    }
                }
            }
            if(dist[t] == inf){
                return -1;
            }
            rep(i,V){
                h[i] += dist[i];
            }
            int d = f;
            for(int v = t; v != s; v = prevv[v]){
                d = min(d, G[prevv[v]][preve[v]].cap);
            }
            f -= d;
            res += d * h[t];
            for(int v = t; v != s;v = prevv[v]){
                edge& e = G[prevv[v]][preve[v]];
                e.cap -= d;
                G[v][e.rev].cap += d;
            }
        }
        return res;
    }
};
