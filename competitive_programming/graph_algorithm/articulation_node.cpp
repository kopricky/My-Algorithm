class Articulation {
public:
	vector<vector<int> > G, graph;
	vector<int> ord;	//訪問時間
	vector<int> low; //low[u]はuから高々1個の後退辺を通ってたどりつけるノードのordの最小値
	vector<bool> visit; //訪問したかどうかのフラグ
	vector<bool> art; //関節点かどうかの判定
	int V;
	Articulation(int node_size){
		V = node_size;
		G.resize(node_size),art.resize(node_size,false);
		ord.resize(node_size),low.resize(node_size),visit.resize(node_size,false);
	}
	void add_edge(int a,int b){
		G[a].push_back(b),G[b].push_back(a);
	}
	void dfs(int v,int p,int &k){
		visit[v] = true;
		ord[v] = low[v] = k++;
		int ct = 0;	//ノードvの次数
		for(int w : G[v]){
			if(!visit[w]){
				ct++;
				dfs(w,v,k);
				low[v] = min(low[v],low[w]);	//子のノードのlowと比較する
				if(p >= 0 && ord[v] <= low[w]){
					art[v] = true;
				}
			}else if(w != p && ord[w] < low[v]){
				low[v] = ord[w];
			}
		}
		if(p == -1 && ct > 1){
			art[v] = true;	//根の次数が2以上なら根は関節点
		}
	}
	void solve(){
		rep(i,V){
			int k = 0;
			if(!visit[i]){
				dfs(i, -1, k);
			}
		}
	}
};
