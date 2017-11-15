//頂点数がn,枝数がm
//bellman_ford bf(n,m);
//適宜add_edge
//bf.comp(出発点)でdに最短路長が入る

#define rep(i,n) for(int i=0;i<(int)(n);++i)

template<typename T> class bellman_ford {
public:
	struct edge{
		int from,to;
		T cost;
	};
	vector<int> d;
	vector<edge> es;
	bool flag;	//負の有向閉路の判定
	int edge_id;
	int V,E;
	bellman_ford(int node_size,int edge_size){
		edge_id = 0, V = node_size, E = edge_size, flag = false;
		d.resize(node_size,numeric_limits<T>::max());
	}
	void add_edge(int from,int to,T cost){
		es[edge_id++] = (edge){from,to,cost};
	}
	//sからたどり着ける負の閉路検出可能
	void solve(int s){
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
		if(cnt == V){
			flag = true;
		}
	}
	//すべての負の閉路の検出を行なえる
	bool find_negative_loop(){
	    fill(d.begin(),d.end(),0);
	    rep(i,V){
	        rep(j,E){
	            edge e = es[j];
	            if(d[e.to] > d[e.from] + e.cost){
	                d[e.to] = d[e.from] + e.cost;
	                if(i == V-1){
	                    return true;
	                }
	            }
	        }
	    }
	    return false;
	}
};
