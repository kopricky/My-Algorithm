typedef pair<int,int>P;

vector<int> G[MAX_N];
vector<int> deg;

void tsort(int n, vector<int>& res) {
	queue<int> que;
	rep(i,n){
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
	if(*max_element(deg.begin(),deg.end()) != 0){
        res.clear();
    }
}

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n,m;
    cin >> n >> m;
    deg.resize(n);
    rep(i,m){
        int a,b;
        cin >> a >> b;
        G[a].push_back(b);
        deg[b]++;
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
