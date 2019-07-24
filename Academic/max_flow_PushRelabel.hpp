#include "header.hpp"

//実装が雑なので速くない
//ヒューリスティックにより高速化可能
//O(n^2 m^(1/2)) Goldberg and Tarjan 1988 を参照(動的木により計算量は改善可能)
class Stack {
private:
    int N, H;
    vector<int> node;
public:
    Stack(int _N, int _H) : N(_N), H(_H), node(N+H){ clear(); }
    inline bool empty(int h){ return node[N+h] == N+h; }
    inline int top(int h){ return node[N+h]; }
    inline void pop(int h){ node[N+h] = node[node[N+h]]; }
    inline void push(int h, int u){ node[u] = node[N+h], node[N+h] = u; }
    inline void clear(){ iota(node.begin() + N, node.end(), N); }
};

class List
{
public:
    struct node {
        int prev, next;
    };
    int N, H;
    vector<size_t> sz;
    vector<node> dat;
    List(int _N, int _H) : N(_N), H(_H), sz(H, 0), dat(N+H){ clear(); }
    inline size_t size(int h) const { return sz[h]; }
    inline void insert(int h, int u){
        ++sz[h];
        dat[u].prev = dat[N+h].prev, dat[u].next = N+h;
        dat[dat[N+h].prev].next = u, dat[N+h].prev = u;
    }
    inline void erase(int h, int u){
        --sz[h];
        dat[dat[u].prev].next = dat[u].next, dat[dat[u].next].prev = dat[u].prev;
    }
    inline void clear(){
        for(int i = N; i < N+H; ++i) dat[i].prev = dat[i].next = i;
    }
};

template <typename T> class PushRelabel
{
public:
    struct edge {
        int to, rev;
        T cap;
        edge(int _to, int _rev, T _cap) : to(_to), rev(_rev), cap(_cap){}
    };
private:
    int V, s, t, pot_max, checker;
    vector<T> excess;
    vector<int> potential, cur_edge, que;
    List all_ver;
    Stack act_ver;
    int init(){
        potential[t] = 0;
        int qh = 0, qt = 0;
        for(que[qt++] = t; qh < qt;){
            int u = que[qh++], h = potential[u] + 1;
            for(edge& e : G[u]){
                if(potential[e.to] == V && G[e.to][e.rev].cap > 0 && e.to != s){
                    potential[e.to] = h, que[qt++] = e.to;
                }
            }
        }
        for(edge& e : G[s]){
            if(potential[e.to] < V){
                G[e.to][e.rev].cap = e.cap, excess[s] -= e.cap, excess[e.to] += e.cap;
            }
            e.cap = 0;
        }
        for(int i = 0; i < V; ++i){
            if(potential[i] < V){
                pot_max = (potential[i] > pot_max) ? potential[i] : pot_max;
                all_ver.insert(potential[i], i);
                if(excess[i] > 0 && i != t) act_ver.push(potential[i], i);
            }
        }
        return pot_max;
    }
    int global_relabel(){
        fill(potential.begin(), potential.end(), V);
        potential[t] = 0;
        int qh = 0, qt = 0;
        for(que[qt++] = t; qh < qt;){
            int u = que[qh++], h = potential[u] + 1;
            for(edge& e : G[u]){
                if(potential[e.to] == V && G[e.to][e.rev].cap > 0 && e.to != s){
                    potential[e.to] = h, que[qt++] = e.to;
                }
            }
        }
        all_ver.clear(), act_ver.clear();
        pot_max = -1;
        for(int i = 0; i < V; ++i){
            if(potential[i] < V){
                cur_edge[i] = 0;
                pot_max = (potential[i] > pot_max) ? potential[i] : pot_max;
                all_ver.insert(potential[i], i);
                if(excess[i] > 0 && i != t) act_ver.push(potential[i], i);
            }
        }
        return pot_max;
    }
    void gap_relabel(const int u){
        for(int i = potential[u]; i <= pot_max; ++i){
            for(int id = all_ver.dat[V+i].next; id < V; id = all_ver.dat[id].next){
                potential[id] = V;
            }
            all_ver.sz[i] = 0;
            all_ver.dat[V+i].prev = all_ver.dat[V+i].next = V+i;
        }
    }
    int discharge(const int u){
        for(int& i = cur_edge[u]; i < (int)G[u].size(); ++i){
            edge& e = G[u][i];
            if(potential[u] == potential[e.to] + 1 && e.cap > 0){
                push(u, e);
                if(excess[u] == 0) return potential[u];
            }
        }
        return relabel(u);
    }
    void push(const int u, edge& e){
        T f = min(e.cap, excess[u]);
        const int v = e.to;
        e.cap -= f, excess[u] -= f;
        G[v][e.rev].cap += f, excess[v] += f;
        if(excess[v] == f && v != t) act_ver.push(potential[v], v);
    }
    int relabel(const int u){
        ++checker;
        int prv = potential[u], cur = V;
        for(int i = 0; i < (int)G[u].size(); ++i){
            edge& e = G[u][i];
            if(cur > potential[e.to] + 1 && e.cap > 0){
                cur_edge[u] = i;
                cur = potential[e.to] + 1;
            }
        }
        if((int)all_ver.size(prv) > 1){
            all_ver.erase(prv, u);
            if((potential[u] = cur) == V) return prv;
            act_ver.push(cur, u);
            all_ver.insert(cur, u);
            pot_max = (pot_max > cur) ? pot_max : cur;
        }else{
            gap_relabel(u);
            return pot_max = prv - 1;
        }
        return cur;
    }
public:
    vector<vector<edge> > G;
    PushRelabel(int node_size)
        : V(node_size), pot_max(-1), checker(0), excess(V, (T)0),
            potential(V, V), cur_edge(V, 0), que(V), all_ver(V, V), act_ver(V, V), G(V){}
    void add_edge(int _from, int _to, T _cap){
        G[_from].emplace_back(_to, (int)G[_to].size(), _cap);
        G[_to].emplace_back(_from, (int)G[_from].size() - 1, 0);
    }
    T solve(int source, int sink){
        s = source, t = sink;
        int level = init();
        while(level >= 0){
            if(act_ver.empty(level)){
                --level;
                continue;
            }
            int u = act_ver.top(level);
            act_ver.pop(level);
            if(excess[u] == 0) continue;
            level = discharge(u);
            if(checker >= V / 2){
                level = global_relabel();
                checker = 0;
            }
        }
        return excess[t];
    }
};
