typedef pair<int,int>P;

const int MAX_N = 100000;

vector<int> G[MAX_N];
vector<int> art;
int ord[MAX_N];	//訪問時間
int low[MAX_N]; //low[u]はuから高々1個の後退辺を通ってたどりつけるノードのordの最小値
bool visit[MAX_N]; //訪問したかどうかのフラグ

void dfs(int v,int p,int &k) //&にすることでkを保持できる pはvの親
{
	visit[v] = true;
	ord[v] = low[v] = k++;
	int ct = 0;	//ノードvの次数
	for(int i=0;i<(int)G[v].size();i++){
		if(!visit[G[v][i]]){
			ct++;
			dfs(G[v][i],v,k);
			low[v] = min(low[v],low[G[v][i]]);	//子のノードのlowと比較する
			if(p >= 0 && ord[v] <= low[G[v][i]]){
				art.push_back(v);
			}
		}else if(G[v][i] != p && ord[G[v][i]] < low[v]){
			low[v] = ord[G[v][i]];
		}
	}
	if(p == -1 && ct > 1){
		art.push_back(v);	//根の次数が2以上なら根は関節点
	}
}

int main()
{
	int n,m;
	scanf("%d %d",&n,&m);
	for(int i=0;i<m;i++){
		int x,y;
		scanf("%d %d",&x,&y);
		G[x].push_back(y);
		G[y].push_back(x);
	}
	int k = 0;
	fill(visit,visit+n,false);
	for(int i=0;i<n;i++){
		if(!visit[i]){
			dfs(i, -1, k);
		}
	}
	sort(art.begin(),art.end());
	for (int i=0;i<art.size();i++){
		printf("%d\n",art[i]);
	}
}
