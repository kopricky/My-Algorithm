//頂点数がn,枝数がm
//bellman_ford bf(n,m);
//適宜add_edge
//bf.comp(出発点)でdに最短路長が入る
//負の有向閉路には3種類存在することに注意する

template<typename T> class bellman_ford {
public:
	struct edge{
		int from,to;
		T cost;
	};
	vector<int> d;
	vector<edge> es;
	int edge_id;
	int V,E;
	bellman_ford(int node_size){
		edge_id = 0, V = node_size;
		d.resize(node_size,numeric_limits<T>::max());
	}
	void add_edge(int from,int to,T cost){
		es.push_back((edge){from,to,cost});
	}
	//sからの最短路長およびsからたどり着ける負の閉路の検出(trueなら負の閉路が存在する)
	bool solve(int s){
		E = (int)es.size();
		int cnt = 0;
		d[s] = 0;
		while(cnt < V){
			bool update = false;
			rep(i,E){
				edge e = es[i];
				if(d[e.from] != numeric_limits<T>::max() && d[e.to] > d[e.from] + e.cost){
					d[e.to] = d[e.from] + e.cost;
					update = true;
				}
			}
			if(!update) break;
			cnt++;
		}
		return (cnt == V);
	}
	//すべての負の閉路の検出(trueなら負の閉路が存在する)
	bool find_negative_loop(){
		E = (int)es.size();
	    fill(d.begin(),d.end(),0);
		int cnt = 0;
		while(cnt < V){
 		   bool update = false;
 		   rep(i,E){
 			   edge e = es[i];
 			   if(d[e.to] > d[e.from] + e.cost){
 				   d[e.to] = d[e.from] + e.cost;
 				   update = true;
 			   }
 		   }
 		   if(!update) break;
 		   cnt++;
 	   }
 	   return (cnt == V);
	}
	//sからtへの最短路上に存在する負の閉路の検出(trueなら負の閉路が存在する)
	bool find_negative_loop(int s,int t){
		E = (int)es.size();
		d[s] = 0;
		rep(i,2*V){
			rep(j,E){
				edge e = es[j];
				if(d[e.from] != numeric_limits<T>::max() && d[e.to] > d[e.from] + e.cost){
					d[e.to] = d[e.from] + e.cost;
					if(i >= V-1 && e.to == t){
						return true;
					}
				}
			}
		}
		return false;
	}
};
