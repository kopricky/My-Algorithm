#include "../header.hpp"

class PartialPersistentUnionFind {
public:
    const int V;
    int last;
    vector<pair<int, int> > par;
    vector<vector<pair<int, int> > > sz;
    PartialPersistentUnionFind(int node_size)
        : V(node_size), last(numeric_limits<int>::min()), par(V), sz(V){
        for(int i = 0; i < V; ++i){
            par[i] = {1, i};
            sz[i].emplace_back(numeric_limits<int>::min(), 1);
        }
    }
    int find(const int u, const int _time){
        if(u == par[u].second || par[u].first > _time){
            return u;
        }else{
            return find(par[u].second, _time);
        }
    }
    bool unite(int u, int v, const int _time){
        assert(last <= _time);
        if(same(u, v, _time)) return false;
        last = _time;
        u = find(u, _time), v = find(v, _time);
        if(par[u].first < par[v].first){
            swap(u, v);
        }
        par[v] = {_time, u};
        sz[u].emplace_back(_time, sz[u].back().second + sz[v].back().second);
        if(par[u].first == par[v].first){
            ++par[u].first;
        }
        return true;
    }
    bool same(const int u, const int v, const int _time){
        return find(u, _time) == find(v, _time);
    }
    int size(int u, const int _time){
        u = find(u, _time);
        return sz[u][static_cast<int>(upper_bound(sz[u].begin(), sz[u].end(),
            make_pair(_time, V+1)) - sz[u].begin()) - 1].second;
    }
};
