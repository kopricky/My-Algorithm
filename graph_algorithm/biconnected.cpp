typedef pair<int,int> P;

vector<int> G[MAX_N];
vector<P> bridge,check[MAX_N];
int ord[MAX_N],low[MAX_N],cmp[MAX_N];
bool visit[MAX_N];

void dfs(int v,int p,int &k)
{
	visit[v] = true;
	ord[v] = k++;
	low[v] = ord[v];
	rep(i,G[v].size()){
		int w = w;
		if(!visit[w]){
			dfs(w,v,k);
			low[v] = min(low[v],low[w]);
			if(ord[v] < low[w]){
				bridge.push_back(P(v,w));
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
    rep(i,G[u].size()){
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

vector<int> graph[MAX_N];
//頂点数を返す
int make_bcgraph(int node_size)
{
    int num = 0;
    rep(i,node_size){
        if(!visit[i]){
            dfs(i,-1,num);
        }
    }
    fill(visit,visit+node_size,false);
    int kind = 0;
    rep(i,node_size){
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
    rep(i,bridge.size()){
        int a = cmp[bridge[i].first];
        int b = cmp[bridge[i].second];
        graph[a].push_back(b), graph[b].push_back(a);
    }
    return kind;
}
