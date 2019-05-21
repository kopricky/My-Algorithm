#include "../header.hpp"

class biconnected {
public:
	using P = pair<int, int>;
	int V,kind;
	vector<vector<int> > G,tree;
	vector<vector<P> > check;
	vector<int> ord,low,cmp;
	vector<P> bridge;
	vector<bool> visit;
	biconnected(int node_size) : V(node_size), kind(0), G(V), check(V), ord(V), low(V)
									,cmp(V), visit(V, false){}
	void build(){
		int id = 0;
		for(int i = 0; i < V; i++){
			if(!visit[i]){
				dfs(i,-1,id);
			}
		}
	}
	void dfs(int u,int p,int &k)
	{
		visit[u] = true;
		ord[u] = k++;
		low[u] = ord[u];
		for(int i = 0; i < (int)G[u].size(); i++){
			int v = G[u][i];
			if(!visit[v]){
				dfs(v,u,k);
				low[u] = min(low[u],low[v]);
				if(ord[u] < low[v]){
					bridge.push_back(P(u,v));
					check[u][i].second = 1;
					check[v][check[u][i].first].second = 1;
				}
			}else if(v != p){
				low[u] = min(low[u],ord[v]);
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
		for(int i = 0; i < (int)G[u].size(); i++){
			int v = G[u][i];
			if(check[u][i].second){
				if(!visit[v]){
					que.push(v);
				}
			}else if(!visit[v]){
				restrict_dfs(v,u,kind,que);
			}
		}
	}
    void rebuild()
	{
		fill(visit.begin(),visit.end(),false);
		for(int i = 0; i < V; i++){
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
	// 2(辺)連結成分を頂点とする木を作る
	void make_bctree()
	{
		tree.resize(V);
		rebuild();
		for(int i = 0; i < (int)bridge.size(); i++){
			int a = cmp[bridge[i].first], b = cmp[bridge[i].second];
			tree[a].push_back(b), tree[b].push_back(a);
		}
	}
};
