//最大流量がint,頂点数がMAX_N
//Dinic<int> dn(MAX_N);
//適宜add_edge,max_flowを用いる
template<typename T> class Dinic {
public:
    struct edge{
        int to;
        T cap;
        int rev;
    };
    vector<vector<edge> > G;
    vector<int> level,iter;
    int n;
    Dinic(int node_size){
        n = node_size;
        G.resize(node_size),level.resize(node_size),iter.resize(node_size);
    }
    //辺を張る
    void add_edge(int from,int to,T cap)
    {
    	G[from].push_back((edge){to,cap,(int)G[to].size()});
    	G[to].push_back((edge){from,(T)0,(int)G[from].size()-1});
    }
    void bfs(int s)
    {
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
    T dfs(int v,int t,T f)
    {
    	if(v==t){
    		return f;
    	}
    	for(int &i = iter[v];i<(int)G[v].size();i++){
    		edge &e = G[v][i];
    		if(e.cap > 0 && level[v] < level[e.to]){
    			T d = dfs(e.to,t,min(f,e.cap));
    			if(d>0){
    				e.cap -= d;
    				G[e.to][e.rev].cap += d;
    				return d;
    			}
    		}
    	}
    	return 0;
    }
    //最大流を計算
    T max_flow(int s,int t)
    {
    	T flow = 0;
    	for(;;){
    		bfs(s);
    		if(level[t]<0){
    			return flow;
    		}
    		fill(iter.begin(),iter.end(),0);
    		T f;
    		while((f=dfs(s,t,numeric_limits<T>::max())) > 0){
    			flow += f;
    		}
    	}
    }
};
