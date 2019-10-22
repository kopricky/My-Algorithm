#include "./header.hpp"

class Stack {
private:
    const int N, H;
    int sz;
    vector<int> node;
public:
    Stack(const int _N, const int _H) : N(_N), H(_H), node(N+H){ clear(); }
    inline bool empty() const { return (sz == 0); }
    inline bool empty(const int h) const { return node[N+h] == N+h; }
    inline int top(const int h) const { return node[N+h]; }
    inline void pop(const int h){ --sz, node[N+h] = node[node[N+h]]; }
    inline void push(const int h, const int u){ ++sz, node[u] = node[N+h], node[N+h] = u; }
    inline void clear(){ sz = 0, iota(node.begin() + N, node.end(), N); }
};

class List {
public:
    struct node {
        int prev, next;
    };
    const int N, H;
    vector<size_t> sz;
    vector<node> dat;
    List(const int _N, const int _H) : N(_N), H(_H), sz(H), dat(N+H){ clear(); }
    inline size_t size(const int h) const { return sz[h]; }
    inline bool empty(const int h) const { return (sz[h] == 0); }
    inline void insert(const int h, const int u){
        ++sz[h];
        dat[u].prev = dat[N+h].prev, dat[u].next = N+h;
        dat[dat[N+h].prev].next = u, dat[N+h].prev = u;
    }
    inline void erase(const int h, const int u){
        --sz[h];
        dat[dat[u].prev].next = dat[u].next, dat[dat[u].next].prev = dat[u].prev;
    }
    inline void clear(){
        for(int i = N; i < N+H; ++i) sz[i-N] = 0, dat[i].prev = dat[i].next = i;
    }
};

template <typename T> class PushRelabel {
public:
    struct edge {
        const int to, rev;
        T cap;
        edge(const int _to, const int _rev, const T _cap)
            : to(_to), rev(_rev), cap(_cap){}
    };
private:
    const int V;
    int s, t, logU, pot_max, level, checker;
    T U;
    vector<T> excess;
    vector<int> potential, cur_edge, que;
    List all_ver;
    Stack act_ver;
    int calc_active(const T K){
        pot_max = -1;
        int act_min = V;
        for(int i = 0; i < V; ++i){
            if(potential[i] < V){
                cur_edge[i] = 0;
                pot_max = max(potential[i], pot_max);
                all_ver.insert(potential[i], i);
                if(excess[i] >= K && i != t){
                    act_ver.push(potential[i], i);
                    act_min = min(act_min, potential[i]);
                }
            }else{
                potential[i] = V+1;
            }
        }
        return act_min;
    }
    void bfs(){
        for(int i = 0; i < V; ++i) potential[i] = max(potential[i], V);
        potential[t] = 0;
        int qh = 0, qt = 0;
        for(que[qt++] = t; qh++ < qt;){
            int u = que[qh-1];
            for(const edge& e : G[u]){
                if(potential[e.to] == V && G[e.to][e.rev].cap > 0){
                    potential[e.to] = potential[u] + 1, que[qt++] = e.to;
                }
            }
        }
    }
    void init(){
        potential[s] = V+1;
        for(edge& e : G[s]){
            if(potential[e.to] < V && e.cap > 0){
                U = max(U, e.cap), logU = max(logU, (int)ceil(log2(e.cap)));
                G[e.to][e.rev].cap = e.cap, excess[s] -= e.cap, excess[e.to] += e.cap;
            }
            e.cap = 0;
        }
    }
    int global_relabel(const T K){
        bfs();
        all_ver.clear(), act_ver.clear();
        return calc_active(K);
    }
    void gap_relabel(const int u){
        for(int i = potential[u]; i <= pot_max; ++i){
            for(int id = all_ver.dat[V+i].next; id < V; id = all_ver.dat[id].next){
                potential[id] = V+1;
            }
            all_ver.sz[i] = 0;
            all_ver.dat[V+i].prev = all_ver.dat[V+i].next = V+i;
        }
    }
    void discharge(const int u, const T K){
        for(int& i = cur_edge[u]; i < (int)G[u].size(); ++i){
            edge& e = G[u][i];
            if(potential[u] == potential[e.to] + 1 && e.cap > 0){
                if(push(u, e, K)) return;
            }
        }
        relabel(u);
    }
    bool push(const int u, edge& e, const T K){
        T f = min(e.cap, K);
        const int v = e.to;
        e.cap -= f, excess[u] -= f;
        G[v][e.rev].cap += f, excess[v] += f;
        if(excess[v] >= K && excess[v] < K + f && v != t){
            act_ver.push(potential[v], v), level = min(level, potential[v]);
        }
        return (excess[u] < K);
    }
    void relabel(const int u){
        ++checker;
        int prv = potential[u], cur = V;
        for(int i = 0; i < (int)G[u].size(); ++i){
            const edge& e = G[u][i];
            if(cur > potential[e.to] + 1 && e.cap > 0){
                cur_edge[u] = i;
                cur = potential[e.to] + 1;
            }
        }
        if((int)all_ver.size(prv) > 1){
            all_ver.erase(prv, u);
            if((potential[u] = cur) == V) return;
            act_ver.push(cur, u);
            all_ver.insert(cur, u);
            pot_max = max(pot_max, cur);
        }else{
            gap_relabel(u);
        }
    }
public:
    vector<vector<edge> > G;
    PushRelabel(const int node_size)
        : V(node_size), logU(0), pot_max(-1), checker(0), excess(V, (T)0),
            potential(V, 0), cur_edge(V), que(V), all_ver(V, V), act_ver(V, V), G(V){}
    void add_edge(const int _from, const int _to, const T _cap){
        G[_from].emplace_back(_to, (int)G[_to].size(), _cap);
        G[_to].emplace_back(_from, (int)G[_from].size() - 1, 0);
    }
    T solve(const int source, const int sink){
        s = source, t = sink;
        init();
        for(int i = logU - (U < ((T)1 << logU)); i >= 0; --i){
            T K = (T)1 << i;
            level = calc_active(K);
            while(!act_ver.empty() && level <= pot_max){
                if(act_ver.empty(level)){
                    ++level;
                    continue;
                }
                int u = act_ver.top(level);
                act_ver.pop(level);
                if(potential[u] >= V) continue;
                discharge(u, K);
                if(checker >= V / 2){
                    level = global_relabel(K);
                    checker = 0;
                }
            }
        }
        return excess[t];
    }
};
