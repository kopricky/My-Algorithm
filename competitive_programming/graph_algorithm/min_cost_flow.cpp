const int MAX_V = 100;

struct edge{
	int to,cap,cost,rev; //行き先,容量,費用,逆辺
};

vector<edge> G[MAX_V];

void add_edge(int from,int to,int cap,int cost){
	G[from].push_back((edge){to,cap,cost,G[to].size()});
	G[to].push_back((edge){from,0,-cost,G[from].size()-1});
}

int min_cost_flow(const int V,const int s,const int t,int f)	//Vは頂点数
{
	int res =0;
	int dist[MAX_V];
	int prevv[MAX_V];
	int preve[MAX_V];
	while(f>0){
		fill(dist,dist+V,INF);
		dist[s] = 0;
		bool update = true;
		while(update){
			update = false;
			for(int v=0;v<V;v++){
				if(dist[v] == INF)	continue;
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

		if(dist[t] == INF){
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
