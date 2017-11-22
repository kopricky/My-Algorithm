//二部グラフの頂点数がそれぞれn,m
//BM bm(n,m);
//適宜add_edge(２つの集合間のedgeのみaddする)
//BM.comp()で最大マッチングの数を計算
class BM {
public:
    struct edge{
        int to,cap,rev;
    };
    vector<vector<edge> > G;
    vector<int> level;
    vector<int> iter;
    int u,v;
    BM(int U,int V){
        u = U,v = V;
        G.resize(u+v+2);
        level.resize(u+v+2);
        iter.resize(u+v+2);
        rep(i,u){
            init_edge(0,i+1);
        }
        rep(i,v){
            init_edge(u+i+1,u+v+1);
        }
    }
    void init_edge(int from,int to){
        G[from].push_back((edge){to,1,(int)G[to].size()});
    	G[to].push_back((edge){from,0,(int)G[from].size()-1});
    }
    void add_edge(int from,int to){
        from += 1,to += u+1;
    	G[from].push_back((edge){to,INF,(int)G[to].size()});
    	G[to].push_back((edge){from,0,(int)G[from].size()-1});
    }
    void bfs(int s){
        fill(level.begin(),level.end(),-1);
    	queue<int> que;
    	level[s] = 0;
    	que.push(s);
    	while(!que.empty()){
    		int v = que.front();
    		que.pop();
    		rep(i,G[v].size()){
    			edge &e = G[v][i];
    			if(e.cap > 0 && level[e.to] < 0){
    				level[e.to] = level[v] + 1;
    				que.push(e.to);
    			}
    		}
    	}
    }
    int dfs(int v,int t,int f){
    	if(v==t){
    		return f;
    	}
    	for(int &i = iter[v];i<(int)G[v].size();i++){
    		edge &e = G[v][i];
    		if(e.cap > 0 && level[v] < level[e.to]){
    			int d = dfs(e.to,t,min(f,e.cap));
    			if(d>0){
    				e.cap -= d;
    				G[e.to][e.rev].cap += d;
    				return d;
    			}
    		}
    	}
    	return 0;
    }
    int solve(){
    	int flow = 0;
        int s=0,t=u+v+1;
    	for(;;){
    		bfs(s);
    		if(level[t]<0){
    			return flow;
    		}
            fill(iter.begin(),iter.end(),0);
    		int f;
    		while((f=dfs(s,t,INF)) > 0){
    			flow += f;
    		}
    	}
    }
};
