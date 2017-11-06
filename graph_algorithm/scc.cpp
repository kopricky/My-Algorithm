vector<int> G[MAX_N];
vector<int> rG[MAX_N];
vector<int> post_order; //帰りがけ順の並び
bool used[MAX_N]; //すでに調べたかどうか
int cmp[MAX_N];	//属する強連結成分のトポロジカル順序
vector<int> graph[MAX_N];

void add_edge(int from,int to)
{
	G[from].push_back(to);
	rG[to].push_back(from);
}

void dfs(int v)
{
	used[v] = true;
	for(int i=0;i<(int)G[v].size();i++){
		if(!used[G[v][i]]){
			dfs(G[v][i]);
		}
	}
	post_order.push_back(v);
}

void rdfs(int v,int k)
{
	used[v] = true;
	cmp[v] = k;
	for(int i=0;i<(int)rG[v].size();i++){
		if(!used[rG[v][i]]){
			rdfs(rG[v][i],k);
		}
	}
}

int scc(int node_size)	//強連結成分の数を示す
{
	fill(used,used+node_size,0);
	post_order.clear();
	for(int v=0;v<node_size;v++){
		if(!used[v]){
			dfs(v);
		}
	}
	fill(used,used+node_size,0);
	int k=0;
	for(int i=(int)post_order.size()-1;i>=0;i--){
		if(!used[post_order[i]]){
			rdfs(post_order[i],k++);
		}
	}
	rep(i,node_size){
		rep(j,(int)G[i].size()){
			if(cmp[i] != cmp[G[i][j]]){
				graph[cmp[i]].push_back(cmp[G[i][j]]);
			}
		}
	}
}
