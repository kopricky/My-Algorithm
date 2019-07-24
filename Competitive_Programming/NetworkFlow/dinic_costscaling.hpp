#include "../header.hpp"

template<typename T> class Dinic {
private:
    int V;
    vector<int> level, iter, que;
    void bfs(int s, T base) {
        fill(level.begin(),level.end(),-1);
        level[s] = 0;
        int qh = 0, qt = 0;
        for(que[qt++] = s; qh < qt;){
            int v = que[qh++];
            for(auto& e : G[v]){
                if(level[e.to] < 0 && e.cap >= base){
                    level[e.to] = level[v] + 1;
                    que[qt++] = e.to;
                }
            }
        }
    }
    T dfs(int v, int t, T f) {
        if(v == t) return f;
        for(int& i = iter[v]; i < (int)G[v].size(); i++){
            edge& e = G[v][i];
            if(level[v] < level[e.to] && e.cap >= f){
                T d = dfs(e.to, t, f);
                if(d > 0){
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

public:
    struct edge{
        int to;
        T cap;
        int rev;
    };
    vector<vector<edge> > G;

    Dinic(int node_size) : V(node_size), level(V), iter(V), que(V), G(V){}
    //辺を張る
    void add_edge(int from,int to,T cap) {
        G[from].push_back((edge){to,cap,(int)G[to].size()});
        G[to].push_back((edge){from,(T)0,(int)G[from].size()-1});
    }
    //最大流を計算(maximum_flow は最大流の上限)
    T solve(int s, int t, T maximum_flow){
        if(maximum_flow == 0) return (T)0;
        T flow = 0;
        for(T base = (T(1)<<(63-__builtin_clzll(maximum_flow))); base >= 1;){
            bfs(s, base);
            if(level[t] < 0){
                base >>= 1;
                continue;
            }
            fill(iter.begin(),iter.end(),0);
            T f;
            while((f = dfs(s,t,base)) > 0){
                flow += f;
            }
        }
        return flow;
    }
};
