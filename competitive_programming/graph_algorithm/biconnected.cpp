class biconnected{
public:
	vector<vector<int> > G,tree;
	vector<int> ord,low,cmp;
    vector<vector<P> > check;
	vector<P> bridge;
	vector<bool> visit;
	int V,kind;
	biconnected(int node_size){
		V = node_size; kind = 0;
		G.resize(V);
		check.resize(V),ord.resize(V),low.resize(V);
		cmp.resize(V),visit.resize(V,false);
	}
	void build(){
		int id = 0;
		rep(i,V){
			if(!visit[i]){
				dfs(i,-1,id);
			}
		}
	}
	void dfs(int v,int p,int &k)
	{
		visit[v] = true;
		ord[v] = k++;
		low[v] = ord[v];
		rep(i,(int)G[v].size()){
			int w = G[v][i];
			if(!visit[w]){
				dfs(w,v,k);
				low[v] = min(low[v],low[w]);
				if(ord[v] < low[w]){
				    bridge.pb(P(v,w));
	                check[v][i].second = 1;
	                check[w][check[v][i].first].second = 1;
				}
			//(v,w)は後退辺
			}else if(w != p){
				low[v] = min(low[v],ord[w]);
			}
		}
	}

	void add_edge(int u,int v)
	{
	    G[u].push_back(v),G[v].push_back(u);
	    check[u].push_back(P((int)check[v].size(),0));
	    check[v].push_back(P((int)check[u].size()-1,0));
	}
	void restrict_dfs(int u,int p,int kind,queue<int>& que)
	{
	    visit[u] = true;
	    cmp[u] = kind;
	    rep(i,(int)G[u].size()){
			int w = G[u][i];
	        if(check[u][i].second){
	            if(!visit[w]){
	                que.push(w);
	            }
	        }else if(!visit[w]){
	            restrict_dfs(w,u,kind,que);
	        }
	    }
	}
    void rebuild()
    {
	    fill(visit.begin(),visit.end(),false);
	    rep(i,V){
	        if(!visit[i]){
	            queue<int> que;
	            que.push(i);
	            while(!que.empty()){
	                int p = que.front();
	                que.pop();
	                restrict_dfs(p,-1,kind,que);
	                kind++;
	            }
	        }
	    }
    }
	// auxiliary graph を作る
	void make_bctree()
	{
		tree.resize(V);
		rebuild();
	    rep(i,(int)bridge.size()){
	        int a = cmp[bridge[i].first];
	        int b = cmp[bridge[i].second];
	        tree[a].push_back(b), tree[b].push_back(a);
	    }
	}
};
