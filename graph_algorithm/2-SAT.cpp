typedef pair<int,int> P;

vector<int> G[2*MAX_N];
vector<int> rG[2*MAX_N];
vector<int> graph[2*MAX_N];
vector<int> post_order; //帰りがけ順の並び
bool used[2*MAX_N]; //すでに調べたかどうか
int cmp[2*MAX_N];	//属する強連結成分のトポロジカル順序

void add_edge(int from,int to)
{
	G[from].push_back(to);
	rG[to].push_back(from);
}

void dfs(int v)
{
	used[v] = true;
	rep(i,G[v].size()){
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
	rep(i,rG[v].size()){
		if(!used[rG[v][i]]){
			rdfs(rG[v][i],k);
		}
	}
}

int scc(int n)	//強連結成分の数を示す
{
	fill(used,used+n,0);
	post_order.clear();
	for(int v=0;v<n;v++){
		if(!used[v]){
			dfs(v);
		}
	}
	fill(used,used+n,0);
	int k=0;
	for(int i=(int)post_order.size()-1;i>=0;i--){
		if(!used[post_order[i]]){
			rdfs(post_order[i],k++);
		}
	}
	return k;
}

bool sat(int n,vector<P> vec) //aVbなら(0,1),bV¬cなら(1,5)みたいな
{
    rep(i,vec.size()){
        add_edge((vec[i].first+n)%(2*n),vec[i].second);
        add_edge((vec[i].second+n)%(2*n),vec[i].first);
    }
    scc(2*n);
    rep(i,n){
        if(cmp[i] == cmp[n+i]){
            return false;
        }
    }
    return true;
}

vector<int> restore(int n)  //真のものは1,偽のものは0を返す
{
    vector<int> vec(n);
    rep(i,n){
        if(cmp[i] > cmp[n+i]){
            vec[i] = 1;
        }else{
            vec[i] = 0;
        }
    }
    return vec;
}
