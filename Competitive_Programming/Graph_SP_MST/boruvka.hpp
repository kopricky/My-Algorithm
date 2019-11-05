#include "../header.hpp"

class UnionFind {
private:
    int sz;
    vector<int> par, nrank;
public:
    UnionFind(){}
    UnionFind(int node_size) : sz(node_size), par(sz), nrank(sz, 0){
        iota(par.begin(), par.end(), 0);
    }
    int find(int x){
        if(par[x] == x) return x;
        else return par[x] = find(par[x]);
    }
    int unite(int x, int y){
    	if(nrank[x] < nrank[y]) swap(x,y);
        par[y] = x;
        if(nrank[x] == nrank[y]) nrank[x]++;
        return x;
    }
};

template<typename T> class Boruvka {
private:
    struct edge {
        int to;
        T cost;
        typename list<edge>::iterator rev;
        edge(const int _to, const T _cost)
            : to(_to), cost(_cost){}
        edge(const int _to, const T _cost, const typename list<edge>::iterator _rev)
            : to(_to), cost(_cost), rev(_rev){}
    };
    struct info {
        T cost;
        typename list<edge>::iterator itr;
        info(){}
        info(const T _cost) : cost(_cost){}
        info(const T _cost, const typename list<edge>::iterator _itr) : cost(_cost), itr(_itr){}
    };
    const int V;
    const T inf;
    UnionFind uf;
    vector<list<edge> > G;
    void cleanup(vector<int>& rem_node, vector<info>& check){
        vector<int> tmp;
        for(int v : rem_node){
            for(auto it = G[v].begin(); it != G[v].end();){
                const edge& e = *it;
                const int p = uf.find(e.to);
                if(p == v){
                    it = G[v].erase(it);
                    continue;
                }
                if(e.cost < check[p].cost){
                    if(check[p].cost == inf) tmp.push_back(p);
                    else G[p].erase(check[p].itr->rev), G[v].erase(check[p].itr);
                    check[p].cost = e.cost, check[p].itr = it++;
                }else{
                    G[p].erase(e.rev), it = G[v].erase(it);
                }
            }
            for(int id : tmp) check[id].cost = inf;
            tmp.clear();
        }
    }
    bool contract(vector<int>& rem_node, vector<int>& check){
        vector<int> new_rem_node;
        bool update = false;
        for(int v : rem_node){
            const int p = uf.find(v);
            if(G[p].empty() || p != v) continue;
            T mn = inf; int id = -1;
            for(const edge& e : G[v]){
                if(e.cost < mn) mn = e.cost, id = e.to;
            }
            id = uf.find(id);
            if(id != v){
                ans += mn, update = true, uf.unite(id, v);
            }
        }
        for(int v : rem_node){
            const int p = uf.find(v);
            if(v == p) new_rem_node.push_back(v);
            else G[p].splice(G[p].begin(), move(G[v]));
        }
        swap(rem_node, new_rem_node);
        return update;
    }

public:
    T ans;
    Boruvka(const int node_size)
        : V(node_size), inf(numeric_limits<T>::max()), uf(V), G(V), ans(0){}
    void add_edge(const int from, const int to, const T cost){
        G[from].emplace_back(to, cost);
        G[to].emplace_back(from, cost, --G[from].end());
        G[from].back().rev = --G[to].end();
    }
    T solve(){
        vector<int> rem_node(V), erase_check(V, 0);
        iota(rem_node.begin(), rem_node.end(), 0);
        vector<info> multiple_check(V);
        for(int i = 0; i < V; ++i) multiple_check[i] = info(inf);
        while((int)rem_node.size() > 1){
            if(!contract(rem_node, erase_check)) return inf;
            cleanup(rem_node, multiple_check);
        }
        return ans;
    }
};
