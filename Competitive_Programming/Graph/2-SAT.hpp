#include "../header.hpp"

//頂点数がMAX_N
//SAT st(MAX_N);
//st.ok(vec) 充足可能性判定
//vecには節 aV¬bなら(0,MAX_N+1), ¬bVcなら(MAX_N+1,2)のようにペアをもたせる
//st.restore(vec) 解構築
class SAT {
private:
	int V;	//頂点数
	vector<vector<int> > G,rG;
	vector<int> post_order; //帰りがけ順の並び
	vector<bool> used; //すでに調べたかどうか
	vector<int> cmp;	//属する強連結成分のトポロジカル順序
	void _add_edge(int from,int to){
		G[from].push_back(to);
		rG[to].push_back(from);
	}
	void _dfs(int v){
		used[v] = true;
		rep(i,G[v].size()){
			if(!used[G[v][i]]){
				_dfs(G[v][i]);
			}
		}
		post_order.push_back(v);
	}
	void _rdfs(int v,int k){
		used[v] = true;
		cmp[v] = k;
		rep(i,rG[v].size()){
			if(!used[rG[v][i]]){
				_rdfs(rG[v][i],k);
			}
		}
	}
	int _scc(){
		fill(used.begin(),used.end(),false);
		post_order.clear();
		for(int v=0;v<2*V;v++){
			if(!used[v]){
				_dfs(v);
			}
		}
		fill(used.begin(),used.end(),false);
		int k=0;
		for(int i=2*V-1;i>=0;i--){
			if(!used[post_order[i]]){
				_rdfs(post_order[i],k++);
			}
		}
		return k;
	}

public:
	SAT(int node_size) : V(node_size), G(2*V), rG(2*V), used(2*V), cmp(2*V){}
	//充足可能性判定
	bool ok(vector<P>& vec){
	    rep(i,vec.size()){
	        _add_edge((vec[i].first+V)%(2*V),vec[i].second);
	        _add_edge((vec[i].second+V)%(2*V),vec[i].first);
	    }
	    _scc();
	    rep(i,V){
	        if(cmp[i] == cmp[V+i]){
	            return false;
	        }
	    }
	    return true;
	}
	//真のものは1,偽のものは0を返す(解の構成)
	void restore(vector<int>& ans){
		ans.resize(V);
	    rep(i,V){
			ans[i] = (cmp[i] > cmp[V+i]);
	    }
	}
};
