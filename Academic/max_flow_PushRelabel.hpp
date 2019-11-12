#include "header.hpp"

//O(n^2 m^(1/2)) Goldberg and Tarjan 1988 を参照(動的木により計算量は改善可能)
class Stack {
private:
    const int N, H;
    vector<int> node;
public:
    Stack(const int _N, const int _H) : N(_N), H(_H), node(N+H){ clear(); }
    inline bool empty(const int h) const { return node[N+h] == N+h; }
    inline int top(const int h) const { return node[N+h]; }
    inline void pop(const int h){ node[N+h] = node[node[N+h]]; }
    inline void push(const int h, const int u){ node[u] = node[N+h], node[N+h] = u; }
    inline void clear(){ iota(node.begin() + N, node.end(), N); }
};

class List {
public:
    struct node {
        int prev, next;
    };
    const int N, H;
    vector<node> dat;
    List(const int _N, const int _H) : N(_N), H(_H), dat(N+H){ clear(); }
    inline bool empty(const int h) const { return (dat[N+h].next == N+h); }
    inline bool more_one(const int h) const { return dat[N+h].prev != dat[N+h].next; }
    inline void insert(const int h, const int u){
        dat[u].prev = dat[N+h].prev, dat[u].next = N+h;
        dat[dat[N+h].prev].next = u, dat[N+h].prev = u;
    }
    inline void erase(const int u){
        dat[dat[u].prev].next = dat[u].next, dat[dat[u].next].prev = dat[u].prev;
    }
    inline void clear(){
        for(int i = N; i < N+H; ++i) dat[i].prev = dat[i].next = i;
    }
};

template <typename T> class PushRelabel {
public:
    struct edge {
        const int to, rev;
        T cap;
        edge(const int _to, const int _rev, const T _cap) : to(_to), rev(_rev), cap(_cap){}
    };
private:
    const int V;
    int s, t, pot_max, checker;
    vector<T> excess;
    vector<int> potential, cur_edge, que;
    List all_ver;
    Stack act_ver;
    int calc_active(){
        pot_max = -1;
        for(int i = 0; i < V; ++i){
            if(potential[i] < V){
                cur_edge[i] = 0;
                pot_max = max(potential[i], pot_max);
                all_ver.insert(potential[i], i);
                if(excess[i] > 0 && i != t) act_ver.push(potential[i], i);
            }else{
                potential[i] = V+1;
            }
        }
        return pot_max;
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
    int init(){
        potential[s] = V+1;
        bfs();
        for(edge& e : G[s]){
            if(potential[e.to] < V){
                G[e.to][e.rev].cap = e.cap, excess[s] -= e.cap, excess[e.to] += e.cap;
            }
            e.cap = 0;
        }
        return calc_active();
    }
    int global_relabel(){
        bfs();
        all_ver.clear(), act_ver.clear();
        return calc_active();
    }
    void gap_relabel(const int u){
        for(int i = potential[u]; i <= pot_max; ++i){
            for(int id = all_ver.dat[V+i].next; id < V; id = all_ver.dat[id].next){
                potential[id] = V+1;
            }
            all_ver.dat[V+i].prev = all_ver.dat[V+i].next = V+i;
        }
    }
    int discharge(const int u){
        for(int& i = cur_edge[u]; i < (int)G[u].size(); ++i){
            edge& e = G[u][i];
            if(potential[u] == potential[e.to] + 1 && e.cap > 0){
                if(push(u, e)) return potential[u];
            }
        }
        return relabel(u);
    }
    bool push(const int u, edge& e){
        T f = min(e.cap, excess[u]);
        const int v = e.to;
        e.cap -= f, excess[u] -= f;
        G[v][e.rev].cap += f, excess[v] += f;
        if(excess[v] == f && v != t) act_ver.push(potential[v], v);
        return (excess[u] == 0);
    }
    int relabel(const int u){
        ++checker;
        int prv = potential[u], cur = V;
        for(int i = 0; i < (int)G[u].size(); ++i){
            const edge& e = G[u][i];
            if(cur > potential[e.to] + 1 && e.cap > 0){
                cur_edge[u] = i;
                cur = potential[e.to] + 1;
            }
        }
        if(all_ver.more_one(prv)){
            all_ver.erase(u);
            if((potential[u] = cur) == V) return potential[u] = V+1, prv;
            act_ver.push(cur, u);
            all_ver.insert(cur, u);
            pot_max = max(pot_max, cur);
        }else{
            gap_relabel(u);
            return pot_max = prv - 1;
        }
        return cur;
    }
    int validate_discharge(const int u){
        for(int& i = cur_edge[u]; i < (int)G[u].size(); ++i){
            edge& e = G[u][i];
            if(potential[u] == potential[e.to] + 1 && e.cap > 0){
                if(validate_push(u, e)) return potential[u] - V;
            }
        }
        return validate_relabel(u);
    }
    bool validate_push(const int u, edge& e){
        T f = min(e.cap, excess[u]);
        const int v = e.to;
        e.cap -= f, excess[u] -= f;
        G[v][e.rev].cap += f, excess[v] += f;
        if(excess[v] == f) act_ver.push(potential[v] - V, v);
        return (excess[u] == 0);
    }
    int validate_relabel(const int u){
        int cur = 2 * V;
        for(int i = 0; i < (int)G[u].size(); ++i){
            const edge& e = G[u][i];
            if(cur > potential[e.to] + 1 && e.cap > 0){
                cur_edge[u] = i;
                cur = potential[e.to] + 1;
            }
        }
        potential[u] = cur;
        act_ver.push(cur - V, u);
        return cur - V;
    }
    void validate(){
        for(int i = 0; i < V; ++i){
            if(potential[i] >= V){
                cur_edge[i] = 0, potential[i] = V;
                if(excess[i] > 0) act_ver.push(0, i);
            }
        }
        int level = 0;
        while(level >= 0){
            if(act_ver.empty(level)){
                --level;
                continue;
            }
            int u = act_ver.top(level);
            act_ver.pop(level);
            level = validate_discharge(u);
        }
    }
public:
    vector<vector<edge> > G;
    PushRelabel(const int node_size)
        : V(node_size), pot_max(-1), checker(0), excess(V, (T)0),
            potential(V, 0), cur_edge(V), que(V), all_ver(V, V), act_ver(V, V), G(V){}
    void add_edge(const int _from, const int _to, const T _cap){
        G[_from].emplace_back(_to, (int)G[_to].size(), _cap);
        G[_to].emplace_back(_from, (int)G[_from].size() - 1, 0);
    }
    T solve(const int source, const int sink){
        s = source, t = sink;
        int level = init();
        while(level >= 0){
            if(act_ver.empty(level)){
                --level;
                continue;
            }
            int u = act_ver.top(level);
            act_ver.pop(level);
            level = discharge(u);
            if(checker >= V / 2){
                level = global_relabel();
                checker = 0;
            }
        }
        // validate();
        return excess[t];
    }
};
