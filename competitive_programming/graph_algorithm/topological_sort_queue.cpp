//頂点数がn
//tsort ts(n);
//適宜add_edge
//ts.solve()で可能かを判定し結果がresに入る

#define rep(i,n) for(int i=0;i<(int)(n);++i)

class tsort {
public:
	vector<vector<int> > G;
	vector<int> deg,res;
	int V;
	tsort(int node_size){
		V = node_size;
		G.resize(node_size),deg.resize(node_size,0);
	}
	void add_edge(int from,int to){
		G[from].push_back(to);
		deg[to]++;
	}
 	bool solve() {
		queue<int> que;
		rep(i,V){
	        if(deg[i] == 0){
	            que.push(i);
	        }
	    }
		while(!que.empty()){
			int p = que.front();
	        que.pop();
			res.push_back(p);
			rep(i,G[p].size()){
	            if(--deg[G[p][i]] == 0){
	                que.push(G[p][i]);
	            }
	        }
		}
		return (*max_element(deg.begin(),deg.end()) != 0);
	}
};
