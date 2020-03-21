#include "./header.hpp"

class Stack {
private:
    const int N, H;
    int sz;
    vector<int> node;
public:
    Stack(const int _N, const int _H) : N(_N), H(_H), node(N+H){ clear(); }
    bool empty() const { return sz == 0; }
    bool empty(const int h) const { return node[N+h] == N+h; }
    int top(const int h) const { return node[N+h]; }
    void pop(const int h){ --sz, node[N+h] = node[node[N+h]]; }
    void push(const int h, const int u){ ++sz, node[u] = node[N+h], node[N+h] = u; }
    void clear(){ sz = 0, iota(node.begin() + N, node.end(), N); }
};

class Queue {
private:
    const int N, H;
	int sz;
    vector<int> node, last;
public:
    Queue(const int _N, const int _H) : N(_N), H(_H), node(N+H), last(H){ clear(); }
    bool empty() const { return sz == 0; }
    bool empty(const int h) const { return node[N+h] == N+h; }
    int top(const int h) const { return node[N+h]; }
    void pop(const int h){ --sz, node[N+h] = node[node[N+h]]; if(empty(h)) last[h] = N+h; }
    void push(const int h, const int u){ ++sz, node[node[last[h]] = u] = N+h, last[h] = u; }
    void clear(){ sz = 0, iota(node.begin() + N, node.end(), N), iota(last.begin(), last.end(), N); }
};

template <typename T> class PushRelabel {
public:
    static_assert(std::is_integral<T>::value, "Integral required.");
    struct edge {
        const int to, rev;
        T cap;
        edge(const int _to, const int _rev, const T _cap) : to(_to), rev(_rev), cap(_cap){}
    };
private:
    const int V;
    int s, t, checker;
    vector<T> excess;
    vector<int> potential, cur_edge, que;
    Stack act_ver;
    // Queue act_ver;
    static unsigned long long ceil2(unsigned long long v){
        --v;
        v = v | (v >> 1), v = v | (v >> 2);
        v = v | (v >> 4), v = v | (v >> 8);
        v = v | (v >> 16), v = v | (v >> 32);
        return ++v;
    }
    int calc_active(const T delta){
        int pot_min = V;
        for(int i = 0; i < V; ++i){
            if(potential[i] < V){
                if(excess[i] >= delta && i != t){
                    act_ver.push(potential[i], i);
                    pot_min = min(pot_min, potential[i]);
                }
            }else{
                potential[i] = V + 1;
            }
        }
        return pot_min;
    }
    void bfs(){
        for(int i = 0; i < V; ++i) potential[i] = max(potential[i], V);
        potential[t] = 0;
        int qh = 0, qt = 0;
        for(que[qt++] = t; qh++ < qt;){
            int u = que[qh - 1];
            for(const edge& e : G[u]){
                if(potential[e.to] == V && G[e.to][e.rev].cap > 0){
                    potential[e.to] = potential[u] + 1, que[qt++] = e.to;
                }
            }
        }
    }
    T init(){
        T mx = 0;
        potential[s] = V + 1;
        bfs();
        for(edge& e : G[s]){
            if(potential[e.to] < V){
                G[e.to][e.rev].cap = e.cap, excess[s] -= e.cap, excess[e.to] += e.cap;
                mx = max(mx, e.cap), e.cap = 0;
            }
        }
        return mx;
    }
    int global_relabel(const T delta){
        bfs();
        act_ver.clear();
        return calc_active(delta);
    }
    int discharge(const int u, const T delta){
        for(int& i = cur_edge[u]; i < (int)G[u].size(); ++i){
            edge& e = G[u][i];
            if(potential[u] == potential[e.to] + 1 && e.cap > 0){
                return push(u, e, delta) ? potential[e.to] : potential[u];
            }
        }
        return relabel(u);
    }
    bool push(const int u, edge& e, const T delta){
        const int v = e.to;
        T f = min(e.cap, excess[u]);
        if(v != t) f = min(f, 2 * delta - 1 - excess[v]);
        e.cap -= f, excess[u] -= f;
        G[v][e.rev].cap += f, excess[v] += f;
        if(excess[u] >= delta) act_ver.push(potential[u], u);
        if(excess[v] >= delta && v != t){
            act_ver.push(potential[v], v);
            return true;
        }else{
            return false;
        }
    }
    int relabel(const int u){
        ++checker;
        int prv = potential[u], cur = V;
        for(int i = 0; i < (int)G[u].size(); ++i){
            const edge& e = G[u][i];
            if(cur > potential[e.to] + 1 && e.cap > 0){
                cur_edge[u] = i, cur = potential[e.to] + 1;
            }
        }
        if((potential[u] = cur) == V) return potential[u] = V + 1, prv;
        act_ver.push(cur, u);
        return prv;
    }
    int validate_calc_active(const T delta){
        int pot_min = 2 * V;
        for(int i = 0; i < V; ++i){
            if(excess[i] >= delta && i != t){
                act_ver.push(potential[i] - V, i);
                pot_min = min(pot_min, potential[i]);
            }
        }
        return pot_min;
    }
    int validate_discharge(const int u, const T delta){
        for(int& i = cur_edge[u]; i < (int)G[u].size(); ++i){
            edge& e = G[u][i];
            if(potential[u] == potential[e.to] + 1 && e.cap > 0){
                return validate_push(u, e, delta) ? potential[e.to] : potential[u];
            }
        }
        return validate_relabel(u);
    }
    bool validate_push(const int u, edge& e, const T delta){
        const int v = e.to;
        T f = min({e.cap, excess[u], 2 * delta - 1 - excess[v]});
        e.cap -= f, excess[u] -= f;
        G[v][e.rev].cap += f, excess[v] += f;
        if(excess[u] >= delta) act_ver.push(potential[u] - V, u);
        if(excess[v] >= delta){
            act_ver.push(potential[v] - V, v);
            return true;
        }else{
            return false;
        }
    }
    int validate_relabel(const int u){
        int prv = potential[u], cur = 2 * V;
        for(int i = 0; i < (int)G[u].size(); ++i){
            const edge& e = G[u][i];
            if(cur > potential[e.to] + 1 && e.cap > 0){
                cur_edge[u] = i, cur = potential[e.to] + 1;
            }
        }
        potential[u] = cur, act_ver.push(cur - V, u);
        return prv;
    }
    void validate(){
        T mx = 0;
        for(int i = 0; i < V; ++i){
            if(potential[i] >= V){
                cur_edge[i] = 0, potential[i] = V + 1;
                mx = max(mx, excess[i]);
            }
        }
        potential[s] = V;
        T delta = (ceil2(mx + 1) >> 1);
        int level = validate_calc_active(delta) - V;
        while(delta > 0){
            while(!act_ver.empty()){
                if(act_ver.empty(level)){
                    ++level;
                    continue;
                }
                const int u = act_ver.top(level);
                act_ver.pop(level);
                level = validate_discharge(u, delta) - V;
            }
            if(delta == 1) break;
            delta >>= 1, level = validate_calc_active(delta) - V;
        }
    }
public:
    vector<vector<edge> > G;
    PushRelabel(const int node_size)
        : V(node_size), checker(0), excess(V, (T)0),
            potential(V, 0), cur_edge(V), que(V), act_ver(V, V), G(V){}
    void add_edge(const int _from, const int _to, const T _cap){
        G[_from].emplace_back(_to, (int)G[_to].size(), _cap);
        G[_to].emplace_back(_from, (int)G[_from].size() - 1, 0);
    }
    T solve(const int source, const int sink){
        s = source, t = sink;
        T delta = (ceil2(init() + 1) >> 1);
        int level = calc_active(delta);
        while(delta > 0){
            while(!act_ver.empty()){
                if(act_ver.empty(level)){
                    ++level;
                    continue;
                }
                const int u = act_ver.top(level);
                act_ver.pop(level);
                level = discharge(u, delta);
                if(checker >= V / 2){
                    level = global_relabel(delta);
                    checker = 0;
                }
            }
            if(delta == 1) break;
            delta >>= 1, level = calc_active(delta);
        }
        // validate();
        return excess[t];
    }
};
