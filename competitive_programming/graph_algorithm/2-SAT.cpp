//頂点数がMAX_N
//SAT st(MAX_N);
//st.ok(vec) 充足可能性判定
//vecには節 aV¬bなら(0,MAX_N+1), ¬bVcなら(MAX_N+1,2)のようにペアをもたせる
//st.restore(vec) 解構築
#define rep(i,n) for(int i=0;i<(int)(n);++i)
typedef pair<int,int> P;

class SAT {
public:
	vector<vector<int> > G,rG;
	vector<int> post_order; //帰りがけ順の並び
	vector<bool> used; //すでに調べたかどうか
	vector<int> cmp;	//属する強連結成分のトポロジカル順序
	int V;	//頂点数

	SAT(int node_size){
		V = node_size;
		G.resize(2*node_size),rG.resize(2*node_size);
		used.resize(2*node_size),cmp.resize(2*node_size);
	}
	void add_edge(int from,int to){
		G[from].push_back(to);
		rG[to].push_back(from);
	}
	void dfs(int v){
		used[v] = true;
		rep(i,G[v].size()){
			if(!used[G[v][i]]){
				dfs(G[v][i]);
			}
		}
		post_order.push_back(v);
	}
	void rdfs(int v,int k){
		used[v] = true;
		cmp[v] = k;
		rep(i,rG[v].size()){
			if(!used[rG[v][i]]){
				rdfs(rG[v][i],k);
			}
		}
	}
	int scc(){
		fill(used.begin(),used.end(),false);
		post_order.clear();
		for(int v=0;v<2*V;v++){
			if(!used[v]){
				dfs(v);
			}
		}
		fill(used.begin(),used.end(),false);
		int k=0;
		for(int i=(int)post_order.size()-1;i>=0;i--){
			if(!used[post_order[i]]){
				rdfs(post_order[i],k++);
			}
		}
		return k;
	}
	//充足可能性判定
	bool ok(vector<P>& vec){
	    rep(i,vec.size()){
	        add_edge((vec[i].first+V)%(2*V),vec[i].second);
	        add_edge((vec[i].second+V)%(2*V),vec[i].first);
	    }
	    scc();
	    rep(i,V){
	        if(cmp[i] == cmp[V+i]){
	            return false;
	        }
	    }
	    return true;
	}
	//真のものは1,偽のものは0を返す(解の構成)
	vector<int> restore(){
	    vector<int> vec(V);
	    rep(i,V){
	        if(cmp[i] > cmp[V+i]){
	            vec[i] = 1;
	        }else{
	            vec[i] = 0;
	        }
	    }
	    return vec;
	}
};
