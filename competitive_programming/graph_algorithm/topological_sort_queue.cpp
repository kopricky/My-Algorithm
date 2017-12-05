//頂点数がn
//tsort ts(n);
//適宜add_edge
//ts.make()でresに結果が返る(閉路がある場合はresになにも入っていない)

class tsort {
	using P = pair<int,int>;
public:
	vector<vector<int> > G;
	vector<int> deg;
	vector<int> order;
	int V;
	tsort(int node_size){
		V = node_size;
		G.resize(node_size),deg.resize(node_size,0);
	}
	void add_edge(int from,int to){
		G[from].push_back(to);
		deg[to]++;
	}
	void solve() {
		vector<int> order;
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
	            if(--deg[G[p][i]] == 0){
	                que.push(G[p][i]);
	            }
	        }
		}
		if(*max_element(deg.begin(),deg.end()) != 0){
	        order.clear();
	    }
	}
};
