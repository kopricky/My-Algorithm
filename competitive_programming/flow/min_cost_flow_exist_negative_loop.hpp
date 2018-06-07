#include "../header.hpp"

//負の有向閉路が存在する場合の最小費用流(ベルマンフォードを用いて計算)
//最小費用がint,頂点数がn
//min_cost_flow<int> mcf(n);
//適宜add_edge
//mcf.solve(始点,終点,流量)で最小費用流を計算
template<typename T> class min_cost_flow{
public:
	struct edge{
		int to,cap;
		T cost;
		int rev;
	};
	vector<vector<edge> > G;
	int V;
	T inf;
	min_cost_flow(int node_size){
		V = node_size, G.resize(node_size);
		inf = numeric_limits<T>::max() / 100;
	}
	void add_edge(int from,int to,int cap,T cost){
		G[from].push_back((edge){to,cap,cost,G[to].size()});
		G[to].push_back((edge){from,0,-cost,G[from].size()-1});
	}
	T solve(const int s,const int t,int f){
		T res =0;
		vector<T> dist(V);
		vector<int> prevv(V),preve(V);
		while(f>0){
			fill(dist.begin(),dist.end(),inf);
			dist[s] = 0;
			bool update = true;
			while(update){
				update = false;
				for(int v=0;v<V;v++){
					if(dist[v] == inf)	continue;
					for(int i=0;i<(int)G[v].size();i++){
						edge &e = G[v][i];
						if(e.cap > 0 && dist[e.to] > dist[v] + e.cost){
							dist[e.to] = dist[v] + e.cost;
							prevv[e.to] = v, preve[e.to] = i;
							update = true;
						}
					}
				}
			}
			if(dist[t] == inf){
				return -1;
			}
			int d = f;
			for(int v=t;v!=s;v=prevv[v]){
				d = min(d,G[prevv[v]][preve[v]].cap);
			}
			f -= d;
			res += d * dist[t];
			for(int v=t;v!=s;v=prevv[v]){
				edge &e = G[prevv[v]][preve[v]];
				e.cap -= d;
				G[v][e.rev].cap += d;
			}
		}
		return res;
	}
};
