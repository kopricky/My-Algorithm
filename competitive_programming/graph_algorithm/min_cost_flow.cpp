//最小費用がint,頂点数がn
//min_cost_flow<int> mcf(n);
//適宜add_edge
//mcf.comp(始点,終点,流量)で最小費用を計算
template<typename T> class min_cost_flow{
public:
	struct edge{
		int to,cap;
		T cost;
		int rev;
	};
	vector<vector<edge> > G;
	int V;
	min_cost_flow(int node_size){
		V = node_size, G.resize(node_size);
	}
	void add_edge(int from,int to,int cap,T cost){
		G[from].push_back((edge){to,cap,cost,G[to].size()});
		G[to].push_back((edge){from,0,-cost,G[from].size()-1});
	}
	T comp(const int s,const int t,int f){
		T res =0;
		vector<T> dist(V);
		vector<int> prevv(V),preve(V);
		while(f>0){
			fill(dist,dist+V,numeric_limits<T>::max());
			dist[s] = 0;
			bool update = true;
			while(update){
				update = false;
				for(int v=0;v<V;v++){
					if(dist[v] == numeric_limits<T>::max())	continue;
					for(int i=0;i<(int)G[v].size();i++){
						edge &e = G[v][i];
						if(e.cap > 0 && dist[e.to] > dist[v] + e.cost){
							dist[e.to] = dist[v] + e.cost;
							prevv[e.to] = v;	//一つ前の頂点を記憶
							preve[e.to] = i;	//枝の番目を記憶
							update = true;
						}
					}
				}
			}
			if(dist[t] == numeric_limits<T>::max()){
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
