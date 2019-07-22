#include "header.hpp"

//実装が雑なので速くない
//ヒューリスティックにより高速化可能
//O(n^2 m^(1/2)) Goldberg and Tarjan 1988 を参照(動的木により計算量は改善可能)
template<typename T> class PushRelabel{
private:
    struct edge{
        int to, rev;
        T cap;
        typename list<edge*>::iterator it;
        edge(int _to, int _rev, T _cap) : to(_to), rev(_rev), cap(_cap){}
    };
    int V, s, t;
    vector<vector<edge> > G;
    vector<list<edge*> > adjacent;
    vector<list<int> > activate;
    vector<T> excess;
    vector<int> potential;
    unique_ptr<bool[]> is_activate;
    int init(){
        global_relabel();
        potential[s] = V;
        int st_level = -1;
        is_activate[s] = is_activate[t] = true;
        for(edge& e : G[s]){
            G[e.to][e.rev].cap = e.cap;
            excess[s] -= e.cap;
            excess[e.to] += e.cap;
            if(e.cap > 0 && !is_activate[e.to]){
                is_activate[e.to] = true;
                if(potential[e.to] != 4*V){
                    activate[potential[e.to]].push_back(e.to);
                    st_level = (potential[e.to] > st_level) ? potential[e.to] : st_level;
                }
            }
            e.cap = 0;
        }
        for(int i = 0; i < V; ++i){
            if(i != s && i != t){
                for(edge& e : G[i]){
                    if(potential[i] == potential[e.to] + 1 && e.cap > 0){
                        adjacent[i].push_back(&e);
                        e.it = --adjacent[i].end();
                    }
                }
            }
        }
        return st_level;
    }
    void global_relabel(){
        queue<int> que;
        que.push(t);
        potential[t] = 0;
        while(!que.empty()){
            int u = que.front();
            que.pop();
            for(edge& e : G[u]){
                if(e.cap == 0 && potential[e.to] == 4*V){
                    potential[e.to] = potential[u] + 1;
                    que.push(e.to);
                }
            }
        }
    }
    void push(int& level, int u){
        edge& e = *adjacent[u].back();
        if(e.cap > excess[u]){
            if(potential[u] + 1 == potential[e.to] && G[e.to][e.rev].cap == 0 && e.to != s && e.to != t){
                adjacent[e.to].push_back(&G[e.to][e.rev]);
                G[e.to][e.rev].it = --adjacent[e.to].end();
            }
            e.cap -= excess[u];
            G[e.to][e.rev].cap += excess[u];
            excess[e.to] += excess[u];
            activate[level].pop_back();
            is_activate[u] = false;
            if(!is_activate[e.to]){
                activate[potential[e.to]].push_back(e.to);
                is_activate[e.to] = true;
                level = (potential[e.to] > level) ? potential[e.to] : level;
            }
            excess[u] = 0;
        }else{
            if(potential[u] + 1 == potential[e.to] && G[e.to][e.rev].cap == 0 && e.to != s && e.to != t){
                adjacent[e.to].push_back(&G[e.to][e.rev]);
                G[e.to][e.rev].it = --adjacent[e.to].end();
            }
            G[e.to][e.rev].cap += e.cap;
            excess[u] -= e.cap;
            excess[e.to] += e.cap;
            if(excess[u] == 0){
                activate[level].pop_back();
                is_activate[u] = false;
            }
            if(!is_activate[e.to]){
                activate[potential[e.to]].push_back(e.to);
                is_activate[e.to] = true;
                level = (potential[e.to] > level) ? potential[e.to] : level;
            }
            e.cap = 0;
            adjacent[u].pop_back();
        }
    }
    void relabel(int& level, int u){
        int prev = potential[u];
        activate[level].pop_back();
        potential[u] = numeric_limits<int>::max();
        for(edge& e : G[u]){
            if(potential[u] > potential[e.to] + 1 && e.cap > 0){
                potential[u] = potential[e.to] + 1;
            }
        }
        level = potential[u];
        activate[level].push_back(u);
        for(edge& e : G[u]){
            if(potential[u] == potential[e.to] + 1 && e.cap > 0){
                adjacent[u].push_back(&e);
                e.it = --adjacent[u].end();
            }
            if(prev + 1 == potential[e.to] && G[e.to][e.rev].cap > 0 && e.to != s && e.to != t){
                adjacent[e.to].erase(G[e.to][e.rev].it);
            }
        }
    }
public:
    PushRelabel(int node_size)
     : V(node_size), G(V), adjacent(V), activate(2*V),
        excess(V, (T)0), potential(V, 4*V), is_activate(new bool[V]()){}
    void add_edge(int from, int to, T cap){
        G[from].emplace_back(to, (int)G[to].size(), cap);
        G[to].emplace_back(from, (int)G[from].size()-1, (T)0);
    }
    T solve(int _s, int _t){
        s = _s, t = _t;
        int level = init();
        while(level >= 0){
            if(activate[level].empty()){
                --level;
                continue;
            }
            int u = activate[level].back();
            if(adjacent[u].empty()){
                relabel(level, u);
            }else{
                push(level, u);
            }
        }
        T flow = 0;
        for(edge& e : G[s]){
            if(potential[e.to] != 4*V) flow += G[e.to][e.rev].cap;
        }
        return flow;
    }
};
