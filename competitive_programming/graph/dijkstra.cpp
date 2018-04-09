//有向グラフの場合は要変更
template<typename T> class Dijkstra {
public:
	struct edge{
		int to; T cost;
	};
	vector<vector<edge> > G;
	vector<T> d;
	int V;
	using pti = pair<T,int>;
	Dijkstra(int node_size){
		V = node_size;
		G.resize(V),d.resize(V,numeric_limits<T>::max());
	}
	//無向グラフの場合
	void add_edge(int u,int v,T cost){
		G[u].push_back((edge){v,cost}),G[v].push_back((edge){u,cost});
	}
	void solve(int s){
		priority_queue<pti,vector<pti>,greater<pti> > que;
		d[s] = 0;
		que.push(pti(0,s));
		while(!que.empty()){
			pti p = que.top();
			que.pop();
			int v = p.second;
			if(d[v] < p.first) continue;
			for(auto& w : G[v]){
				if(d[w.to] > d[v] + w.cost){
					d[w.to] = d[v] + w.cost;
					que.push(pti(d[w.to],w.to));
				}
			}
		}
	}
};
