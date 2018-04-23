#include "../header.hpp"

//関節点を求めるアルゴリズム(計算量は線形)
//各2(点)連結成分を求めたいときはコメントアウトをはずす
class Articulation {
public:
	vector<vector<int> > G;
	vector<int> ord;	//訪問時間
	vector<int> low; //low[u]はuから高々1個の後退辺を通ってたどりつけるノードのordの最小値
	vector<bool> visit; //訪問したかどうかのフラグ
	vector<bool> art; //関節点かどうかの判定
	// vector<vector<P> > edgeset;
	// stack<P> st;
	int V;
	Articulation(int node_size){
		V = node_size;
		G.resize(node_size),art.resize(node_size,false);
		ord.resize(node_size, -1),low.resize(node_size),visit.resize(node_size,false);
	}
	void add_edge(int a,int b){
		G[a].push_back(b),G[b].push_back(a);
	}
	void dfs(int v,int p,int &k){
		visit[v] = true;
		ord[v] = low[v] = k++;
		int ct = 0;	//ノードvの次数
		for(int w : G[v]){
            // if(w != p && ord[w] < ord[v]) st.push(P(v, w));
			if(!visit[w]){
				ct++;
				dfs(w,v,k);
				low[v] = min(low[v],low[w]);	//子のノードのlowと比較する
				if(p >= 0 && ord[v] <= low[w]){
					art[v] = true;
				}
                // if(low[w] >= ord[v]){
                //     edgeset.push_back(vector<P>());
                //     while(1){
                //         P p = st.top();
                //         st.pop();
                //         edgeset.back().push_back(p);
                //         if(p == P(v, w)) break;
                //     }
                // }
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
