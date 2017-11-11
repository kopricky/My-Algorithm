//頂点数がn
//SCC sc(n);
//sc.comp()で強連結成分分解
//sc.make_graph()で強連結成分分解後のグラフを構築

#define rep(i,n) for(int i=0;i<(int)(n);++i)

class SCC {
public:
	vector<vector<int> > G,rG;
	vector<int> post_order; //帰りがけ順の並び
	vector<bool> used; //すでに調べたかどうか
	vector<int> cmp;	//属する強連結成分のトポロジカル順序
	vector<vector<int> > graph;	//強連結成分分解後のグラフ
	int V,VV;	//強連結成分分解前後の頂点数
	SCC(int node_size){
		V = node_size;
		G.resize(node_size),rG.resize(node_size),used.resize(node_size,false);
		cmp.resize(node_size);
	}
	void add_edge(int from,int to){
		G[from].push_back(to), rG[to].push_back(from);
	}
	void dfs(int v){
		used[v] = true;
		for(int i=0;i<(int)G[v].size();i++){
			if(!used[G[v][i]]){
				dfs(G[v][i]);
			}
		}
		post_order.push_back(v);
	}
	void rdfs(int v,int k){
		used[v] = true;
		cmp[v] = k;
		for(int i=0;i<(int)rG[v].size();i++){
			if(!used[rG[v][i]]){
				rdfs(rG[v][i],k);
			}
		}
	}
	void comp(){
		fill(used.begin(),used.end(),0);
		post_order.clear();
		for(int v=0;v<V;v++){
			if(!used[v]){
				dfs(v);
			}
		}
		fill(used.begin(),used.end(),0);
		int k=0;
		for(int i=(int)post_order.size()-1;i>=0;i--){
			if(!used[post_order[i]]){
				rdfs(post_order[i],k++);
			}
		}
		VV = k;
	}
	void make_graph(){
		graph.resize(VV);
		rep(i,V){
			rep(j,G[i].size()){
				if(cmp[i] != cmp[G[i][j]]){
					graph[i].push_back(G[i][j]);
				}
			}
		}
	}
};
