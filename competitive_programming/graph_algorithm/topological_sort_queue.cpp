typedef pair<int,int> P;

vector<int> G[MAX_N];
int deg[MAX_N];

void add_edge(int from,int to)
{
	G[from].push_back(to);
	deg[to]++;
}

void tsort(int V, vector<int>& res) {
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
	if(*max_element(deg,deg+V) != 0){
        res.clear();
    }
}

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n,m;
    cin >> n >> m;
    rep(i,m){
        int a,b;
        cin >> a >> b;
		add_edge(a,b);
    }
    vector<int> res;
    tsort(n,res);
    if((int)res.size() != 0){
        rep(i,res.size()){
            cout << res[i] << endl;
        }
    }else{
        cout << "-1\n";	//閉路が存在する
    }
    return 0;
}
