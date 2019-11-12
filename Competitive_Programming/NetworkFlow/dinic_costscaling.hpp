#include "../header.hpp"

template<typename T> class Dinic {
private:
    static_assert(std::is_integral<T>::value, "Integral required.");
    struct edge{
        int to;
        T cap;
        int rev;
    };
    const int V;
    vector<int> level, iter, que;
    static unsigned long long floor2(unsigned long long v){
        v = v | (v >> 1), v = v | (v >> 2);
        v = v | (v >> 4), v = v | (v >> 8);
        v = v | (v >> 16), v = v | (v >> 32);
        return v - (v >> 1);
    }
    void bfs(const int s, const T base) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        int qh = 0, qt = 0;
        for(que[qt++] = s; qh < qt;){
            int v = que[qh++];
            for(edge& e : G[v]){
                if(level[e.to] < 0 && e.cap >= base){
                    level[e.to] = level[v] + 1;
                    que[qt++] = e.to;
                }
            }
        }
    }
    T dfs(const int v, const int t, const T base, const T f) {
        if(v == t) return f;
        T sum = 0;
        for(int& i = iter[v]; i < (int)G[v].size(); i++){
            edge& e = G[v][i];
            if(e.cap >= base && level[v] < level[e.to]){
                T d = dfs(e.to, t, base, min(f - sum, e.cap));
                if(d > 0){
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    sum += d;
                    if(f - sum < base) break;
                }
            }
        }
        return sum;
    }

public:
    vector<vector<edge> > G;

    Dinic(const int node_size) : V(node_size), level(V), iter(V), que(V), G(V){}
    //辺を張る
    void add_edge(const int from, const int to, const T cap) {
        G[from].push_back((edge){to, cap, (int)G[to].size()});
        G[to].push_back((edge){from, (T)0, (int)G[from].size()-1});
    }
    //最大流を計算(max_cap は辺の容量の上限)
    T solve(const int s, const int t, const T max_cap){
        T flow = 0;
        for(T base = floor2(max_cap); base >= 1;){
            bfs(s, base);
            if(level[t] < 0){
                base >>= 1;
                continue;
            }
            fill(iter.begin(), iter.end(), 0);
            flow += dfs(s, t, base, numeric_limits<T>::max());
        }
        return flow;
    }
};
