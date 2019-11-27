#include "../header.hpp"

//関節点を求めるアルゴリズム(計算量は線形)
//各2(点)連結成分を求めたいときはコメントアウトをはずす
class Articulation {
public:
	using P = pair<int, int>;
	const int V;
	vector<vector<int> > G;
	vector<int> ord;	//訪問時間
	vector<int> low; //low[u]はuから高々1個の後退辺を通ってたどりつけるノードのordの最小値
	vector<bool> art; //関節点かどうかの判定
	// vector<vector<P> > edgeset;
	// stack<P> st;
	Articulation(const int node_size)
	 	: V(node_size), G(V), ord(V, -1), low(V), art(V, false){}
	void add_edge(const int a, const int b){
		G[a].push_back(b), G[b].push_back(a);
	}
	void dfs(const int u, const int p, int &tm){
		ord[u] = low[u] = tm++;
		int ct = 0;	// ノードvの次数
		for(int v : G[u]){
			// if(v != p && ord[v] < ord[u]) st.push(P(u, v));
			if(ord[v] < 0){
				++ct;
				dfs(v, u, tm);
				low[u] = min(low[u], low[v]);	//子のノードのlowと比較する
				if(p >= 0 && ord[u] <= low[v]) art[u] = true;
				// if(low[v] >= ord[u]){
				//     edgeset.push_back(vector<P>());
				//     while(true){
				//         P p = st.top();
				//         st.pop();
				//         edgeset.back().push_back(p);
				//         if(p == P(u, v)) break;
				//     }
				// }
			}else{
				low[u] = min(low[u], ord[v]);
			}
		}
		if(p == -1 && ct > 1) art[u] = true;	//根の次数が2以上なら根は関節点
	}
	void solve(){
		int tm = 0;
		for(int i = 0; i < V; ++i){
			if(ord[i] < 0) dfs(i, -1, tm);
		}
	}
};
