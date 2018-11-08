#include "../header.hpp"

//最小流量制約付き最大流
//check:達成できるかどうかを判定
//max_flow:最大流を計算
template <typename T>
class Dinic
{
private:
    struct edge
    {
        int to;
        T cap;
        int rev;
    };
    int V;
    T cap_sum;
    vector<vector<edge> > G;
    vector<int> level, iter;
    //辺を張る
    void add_edge(int from, int to, T cap)
    {
        G[from].push_back((edge){to, cap, (int)G[to].size()});
        G[to].push_back((edge){from, (T)0, (int)G[from].size() - 1});
    }
    void bfs(int s)
    {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (auto& e : G[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }
    T dfs(int v, int t, T f)
    {
        if (v == t) {
            return f;
        }
        for (int& i = iter[v]; i < (int)G[v].size(); i++) {
            edge& e = G[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                T d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    T solve()
    {
        T flow = 0;
        for (;;) {
            bfs(V);
            if (level[V + 1] < 0) {
                return flow;
            }
            fill(iter.begin(), iter.end(), 0);
            T f;
            while ((f = dfs(V, V + 1, numeric_limits<T>::max())) > 0) {
                flow += f;
            }
        }
    }

public:
    Dinic(int node_size) : V(node_size), cap_sum(0), G(V + 2), level(V + 2), iter(V + 2) {}
    void add_edge(int from, int to, T min_cap, T max_cap)
    {
        add_edge(from, to, max_cap - min_cap);
        if(min_cap){
            cap_sum += min_cap;
            add_edge(V, to, min_cap);
            add_edge(from, V + 1, min_cap);
        }
    }
    bool check(int s, int t)
    {
        auto tmpG = G;
        add_edge(t, s, numeric_limits<T>::max());
        T res = solve();
        G = tmpG;
        return (res == cap_sum);
    }
    //最大流を計算
    T max_flow(int s, int t)
    {
        add_edge(V, s, numeric_limits<T>::max());
        add_edge(t, V + 1, numeric_limits<T>::max());
        return solve() - cap_sum;
    }
};
