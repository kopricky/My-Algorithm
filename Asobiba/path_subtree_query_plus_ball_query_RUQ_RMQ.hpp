#include "./header.hpp"

template<typename T> class segtree {
private:
    int n, sz;
    vector<T> node;
public:
    void init(const vector<T>& v){
        n = 1, sz = (int)v.size();
        while(n < sz) n *= 2;
        node.assign(2 * n, numeric_limits<T>::max());
        for(int i = 0; i < sz; ++i) node[i + n] = v[i];
        for(int i = n - 1; i >= 1; --i) node[i] = min(node[i << 1], node[(i << 1) | 1]);
    }
    void update(int k, const T a){
        const T old = node[k += n];
        if(old == a) return;
        node[k] = a;
        while(k >>= 1){
            if(node[k] > a) node[k] = a;
            else if(node[k] == old) node[k] = min(node[k << 1], node[(k << 1) | 1]);
            else break;
        }
    }
    T query(int a, int b) const {
        if(a == 0 && b == sz) return node[1];
        T res1 = numeric_limits<T>::max(), res2 = numeric_limits<T>::max();
        a += n, b += n;
        while(a != b){
            if(a & 1) res1 = min(res1, node[a++]);
            if(b & 1) res2 = min(node[--b], res2);
            a >>= 1, b >>= 1;
        }
        return min(res1, res2);
    }
};

template<typename T> class HLdecomposition {
private:
    struct node {
        int left, right, lb, rb;
        segtree<T> seg[3];
        vector<pair<int, int> > pvec[3];
    };
    int V;
    vector<vector<int> > G;
    vector<node> nodes;
    vector<T> value;
    vector<int> sz, depth, pathup, path_id, node_id, wbt_root;
    void predfs(const int u, const int p, const int d){
        sz[u] = 1, depth[u] = d;
        for(int& v : G[u]){
            if(v == p){
                if(v == G[u].back()) break;
                else swap(v, G[u].back());
            }
            predfs(v, u, d + 1);
            sz[u] += sz[v];
            if(sz[v] > sz[G[u][0]]) swap(v, G[u][0]);
        }
    }
    void dfs(const int u, const int p, const int par, int& tm, vector<vector<int> >& hpath){
        for(const int v : G[u]){
            if(v == p) continue;
            if(v == G[u][0]){
                pathup[v] = par, path_id[v] = tm, hpath[tm].push_back(v);
                dfs(v, u, par, tm, hpath);
            }else{
                pathup[v] = u, path_id[v] = ++tm, hpath.push_back({v});
                dfs(v, u, u, tm, hpath);
            }
        }
    }
    void node_merge(segtree<T>& sg, vector<pair<int, int> >& p, const vector<pair<int, int> >& q, const vector<pair<int, int> >& r){
        const int n = (int)q.size() + (int)r.size();
        p.resize(n);
        merge(q.begin(), q.end(), r.begin(), r.end(), p.begin());
        vector<T> val(n);
        for(int i = 0; i < n; ++i) val[i] = value[p[i].second];
        sg.init(val);
    }
    int build_rec(const int l, const int r, const vector<int>& vers, const vector<long long>& sm, const vector<vector<vector<int> > >& info, int& id){
        const int k = id++;
        nodes[k].lb = l, nodes[k].rb = r;
        if(r - l == 1){
            const int u = vers[l];
            for(int i = 0; i < 3; ++i) nodes[k].pvec[i].resize(sz[u]);
            vector<T> val(sz[u]);
            int id = 0;
            for(int i = 0; i < (int)info[u].size(); ++i){
                for(const int v : info[u][i]){
                    nodes[k].pvec[0][id] = {i - l, v}, nodes[k].pvec[1][id] = {i, v}, nodes[k].pvec[2][id] = {i + l, v};
                    val[id++] = value[v];
                }
            }
            for(int i = 0; i < 3; ++i) nodes[k].seg[i].init(val);
            return node_id[u] = k;
        }
        int mid = l + 1;
        long long cri = min(sm[mid] - sm[l], sm[r] - sm[mid]);
        for(int i = l + 2; i < r; ++i){
            if(cri < min(sm[i] - sm[l], sm[r] - sm[i])){
                mid = i, cri = min(sm[mid] - sm[l], sm[r] - sm[mid]);
            }
        }
        const int left = build_rec(l, mid, vers, sm, info, id), right = build_rec(mid, r, vers, sm, info, id);
        nodes[k].left = left, nodes[k].right = right;
        for(int i = 0; i < 3; ++i) node_merge(nodes[k].seg[i], nodes[k].pvec[i], nodes[left].pvec[i], nodes[right].pvec[i]);
        return k;
    }
    int build_wbt(const vector<int>& vers, int& id, const vector<vector<vector<int> > >& info){
        int n = (int)vers.size();
        vector<long long> sm(n + 1, 0);
        for(int i = 0; i < n; ++i) sm[i + 1] = sm[i] + sz[vers[i]];
        return build_rec(0, (int)vers.size(), vers, sm, info, id);
    }
    void build_impl(const int root){
        predfs(root, -1, 0);
        int tm = 0;
        vector<vector<int> > hpath;
        pathup[root] = -1, path_id[root] = 0, hpath.push_back({root});
        dfs(root, -1, -1, tm, hpath);
        vector<vector<vector<int> > > info(V);
        for(int i = 0; i < V; ++i){
            const int d = depth[i];
            for(int u = i; u >= 0; u = pathup[u]){
                if((int)info[u].size() <= d - depth[u]) info[u].resize(d - depth[u] + 1);
                info[u][d - depth[u]].push_back(i);
            }
        }
        int id = 0;
        for(const vector<int>& path : hpath){
            for(int i = 0; i < (int)path.size() - 1; ++i) sz[path[i]] -= sz[path[i + 1]];
            wbt_root.push_back(build_wbt(path, id, info));
        }
    }
    void vertex_update_impl(const int u, const T x){
        for(int v = u; v >= 0; v = pathup[v]){
            const int d = depth[u] - depth[v];
            const int id = nodes[node_id[v]].lb;
            int w = wbt_root[path_id[v]];
            while(true){
                node& cur = nodes[w];
                cur.seg[0].update((int)(lower_bound(cur.pvec[0].begin(), cur.pvec[0].end(), make_pair(d - id, u)) - cur.pvec[0].begin()), x);
                cur.seg[1].update((int)(lower_bound(cur.pvec[1].begin(), cur.pvec[1].end(), make_pair(d, u)) - cur.pvec[1].begin()), x);
                cur.seg[2].update((int)(lower_bound(cur.pvec[2].begin(), cur.pvec[2].end(), make_pair(d + id, u)) - cur.pvec[2].begin()), x);
                if(nodes[w].rb - nodes[w].lb == 1) break;
                else if(id < nodes[nodes[w].left].rb) w = nodes[w].left;
                else w = nodes[w].right;
            }
        }
    }
    T query(int a, int b, const int x, const node& cur, int t) const {
        const int l = cur.lb, r = cur.rb;
        if(r <= a || b <= l) return numeric_limits<T>::max();
        if(a <= l && r <= b){
            return cur.seg[t].query(0, (int)(upper_bound(cur.pvec[t].begin(), cur.pvec[t].end(), make_pair(x, V)) - cur.pvec[t].begin()));
        }else{
            return min(query(a, b, x, nodes[cur.left], t), query(a, b, x, nodes[cur.right], t));
        }
    }
    T path_query_impl(int u, int v, const int d) const {
        T res = numeric_limits<T>::max();
        while(path_id[u] != path_id[v]){
            if(path_id[u] < path_id[v]){
                const int id = nodes[node_id[v]].lb;
                const node& wroot = nodes[wbt_root[path_id[v]]];
                res = min({res, query(0, id + 1, d, wroot, 1), query(id + 1, wroot.rb, d + id, wroot, 2)});
                v = pathup[v];
            }else{
                const int id = nodes[node_id[u]].lb;
                const node& wroot = nodes[wbt_root[path_id[u]]];
                res = min({res, query(0, id + 1, d, wroot, 1), query(id + 1, wroot.rb, d + id, wroot, 2)});
                u = pathup[u];
            }
        }
        if(depth[u] > depth[v]) swap(u, v);
        const int L = nodes[node_id[u]].lb, R = nodes[node_id[v]].lb;
        const node& wroot = nodes[wbt_root[path_id[u]]];
        res = min({res, query(0, L, d - L, wroot, 0), query(L, R + 1, d, wroot, 1), query(R + 1, wroot.rb, d + R, wroot, 2)});
        const int lca = depth[u];
        for(u = pathup[u]; u >= 0 && depth[u] >= lca - d; u = pathup[u]){
            const int id = nodes[node_id[u]].lb;
            const node& wroot = nodes[wbt_root[path_id[u]]];
            res = min({res, query(0, id + 1, d - lca + depth[u] - id, wroot, 0), query(id + 1, wroot.rb, d - lca + depth[u] + id, wroot, 2)});
        }
        return res;
    }
    T subtree_query_impl(const int u, const int d) const {
        const int id = nodes[node_id[u]].lb;
        const node& wroot = nodes[wbt_root[path_id[u]]];
        return query(id, wroot.rb, d + id, wroot, 2);
    }
public:
    HLdecomposition(const vector<T>& _value)
         : V((int)_value.size()), G(V), nodes(2 * V - 1), value(_value),
             sz(V), depth(V), pathup(V), path_id(V), node_id(V){}
    void add_edge(const int u, const int v){
        G[u].push_back(v), G[v].push_back(u);
    }
    void build(const int root=0){
        build_impl(root);
    }
    void vertex_update(const int u, const T x){
        vertex_update_impl(u, x);
    }
    T path_query(const int u, const int v, const int d) const {
        assert(d >= 0);
        return path_query_impl(u, v, d);
    }
    T subtree_query(const int u, const int d) const {
        assert(d >= 0);
        return subtree_query_impl(u, d);
    }
};