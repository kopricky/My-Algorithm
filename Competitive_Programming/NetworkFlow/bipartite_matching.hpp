#include "../header.hpp"

//二部グラフの頂点数がそれぞれn,m
//BM bm(n,m);
//適宜add_edge(２つの集合間のedgeのみaddする)
//BM.solve()で最大マッチングの数を計算
class BM {
private:
    struct edge {
        int to,cap,rev;
    };
    int U,V;
    vector<vector<edge> > G;
    vector<int> level;
    vector<int> iter;

public:
    BM(int u,int v) : U(u), V(v), G(U+V+2){
        for(int i = 0; i < U; i++){
            init_edge(0,i+1);
        }
        for(int i = 0; i < V; i++){
            init_edge(U+i+1,U+V+1);
        }
    }
    void init_edge(int from,int to) {
        G[from].push_back((edge){to,1,(int)G[to].size()});
    	G[to].push_back((edge){from,0,(int)G[from].size()-1});
    }
    void add_edge(int from,int to) {
        from += 1,to += U+1;
    	G[from].push_back((edge){to,1,(int)G[to].size()});
    	G[to].push_back((edge){from,0,(int)G[from].size()-1});
    }
    void bfs(int s) {
        fill(level.begin(),level.end(),-1);
    	queue<int> que;
    	level[s] = 0;
    	que.push(s);
    	while(!que.empty()){
            int v = que.front();
            que.pop();
            for(auto& e : G[v]){
                if(e.cap > 0 && level[e.to] < 0){
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }
    int dfs(int v,int t,int f) {
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
    int solve() {
        level.resize(U+V+2), iter.resize(U+V+2);
        int flow = 0;
        int s=0,t=U+V+1;
        for(;;){
            bfs(s);
            if(level[t] < 0) return flow;
            fill(iter.begin(),iter.end(),0);
            int f;
            while((f=dfs(s,t,numeric_limits<int>::max())) > 0){
                flow += f;
            }
        }
    }
    vector<int> allocate() {
        vector<int> res(U, -1);
        for(int i = 0; i < U; i++){
            for(auto& e : G[i+1]){
                if(e.cap == 0 && e.to != 0){
                    res[i] = e.to-U-1;
                    break;
                }
            }
        }
        return res;
    }
};
