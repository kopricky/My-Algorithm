typedef pair<int,int> P;

struct edge{
	int to;
	int cost;
};

int d[MAX_N];
vector<edge> G[MAX_N];

void dijkstra(int s)
{
	priority_queue<P,vector<P>,greater<P> > que;
	d[s] = 0;
	que.push(P(0,s));
	while(!que.empty()){
		P p = que.top();
		que.pop();
		int v = p.second;
		if(d[v] < p.first) continue;
		for(auto w : G[v]){
			if(d[w.to] > d[v] + w.cost){
				d[w.to] = d[v] + w.cost;
				que.push(P(d[w.to],w.to));
			}
		}
	}
}
