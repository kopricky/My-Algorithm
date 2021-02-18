#include "./header.hpp"

template<typename T> class BIT {
private:
	int n;
	vector<T> bit;
    T sm;
public:
    void init(const vector<T>& v){
        n = (int)v.size() + 1, bit.resize(n, 0);
        sm = accumulate(v.begin(), v.end(), (T)0);
        int j;
        for(int i = 1; i < n; ++i){
            bit[i] += v[i - 1], j = i + (i & -i);
            if(j < n) bit[j] += bit[i];
        }
    }
	void add(int i, const T x){
		++i, sm += x;
		while(i < n) bit[i] += x, i += i & -i;
	}
	T sum(int i) const {
        if(i == -1) return 0;
        else if(i == n - 2) return sm;
        else{
            ++i;
            T s = 0;
            while(i > 0) s += bit[i], i -= i & -i;
            return s;
        }
	}
};

template<typename T> class HLdecomposition {
private:
    struct node {
        int left, right, lb, rb;
        BIT<T> bt[3];
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
    void node_merge(BIT<T>& bt, vector<pair<int, int> >& p, const vector<pair<int, int> >& q, const vector<pair<int, int> >& r){
        const int n = (int)q.size() + (int)r.size();
        p.resize(n);
        merge(q.begin(), q.end(), r.begin(), r.end(), p.begin());
        vector<T> val(n);
        for(int i = 0; i < n; ++i) val[i] = value[p[i].second];
        bt.init(val);
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
            for(int i = 0; i < 3; ++i) nodes[k].bt[i].init(val);
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
        for(int i = 0; i < 3; ++i) node_merge(nodes[k].bt[i], nodes[k].pvec[i], nodes[left].pvec[i], nodes[right].pvec[i]);
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
    void vertex_add_impl(const int u, const T x){
        for(int v = u; v >= 0; v = pathup[v]){
            const int d = depth[u] - depth[v];
            const int id = nodes[node_id[v]].lb;
            int w = wbt_root[path_id[v]];
            while(true){
                node& cur = nodes[w];
                cur.bt[0].add((int)(lower_bound(cur.pvec[0].begin(), cur.pvec[0].end(), make_pair(d - id, u)) - cur.pvec[0].begin()), x);
                cur.bt[1].add((int)(lower_bound(cur.pvec[1].begin(), cur.pvec[1].end(), make_pair(d, u)) - cur.pvec[1].begin()), x);
                cur.bt[2].add((int)(lower_bound(cur.pvec[2].begin(), cur.pvec[2].end(), make_pair(d + id, u)) - cur.pvec[2].begin()), x);
                if(nodes[w].rb - nodes[w].lb == 1) break;
                else if(id < nodes[nodes[w].left].rb) w = nodes[w].left;
                else w = nodes[w].right;
            }
        }
    }
    T query(int a, int b, const int x, const node& cur, int t) const {
        const int l = cur.lb, r = cur.rb;
        if(r <= a || b <= l) return 0;
        if(a <= l && r <= b){
            return cur.bt[t].sum((int)(upper_bound(cur.pvec[t].begin(), cur.pvec[t].end(), make_pair(x, V)) - cur.pvec[t].begin()) - 1);
        }else{
            return query(a, b, x, nodes[cur.left], t) + query(a, b, x, nodes[cur.right], t);
        }
    }
    T path_query_impl(int u, int v, const int d) const {
        T res = 0;
        while(path_id[u] != path_id[v]){
            if(path_id[u] < path_id[v]){
                const int id = nodes[node_id[v]].lb;
                const node& wroot = nodes[wbt_root[path_id[v]]];
                res += query(0, id + 1, d, wroot, 1) + query(id + 1, wroot.rb, d + id, wroot, 2) - query(0, wroot.rb, d - 1, wroot, 2);
                v = pathup[v];
            }else{
                const int id = nodes[node_id[u]].lb;
                const node& wroot = nodes[wbt_root[path_id[u]]];
                res += query(0, id + 1, d, wroot, 1) + query(id + 1, wroot.rb, d + id, wroot, 2) - query(0, wroot.rb, d - 1, wroot, 2);
                u = pathup[u];
            }
        }
        if(depth[u] > depth[v]) swap(u, v);
        const int L = nodes[node_id[u]].lb, R = nodes[node_id[v]].lb;
        const node& wroot = nodes[wbt_root[path_id[u]]];
        res += query(0, L, d - L, wroot, 0) + query(L, R + 1, d, wroot, 1) + query(R + 1, wroot.rb, d + R, wroot, 2);
        const int lca = depth[u];
        int prev_wbt_root = wbt_root[path_id[u]];
        for(u = pathup[u]; u >= 0 && depth[u] >= lca - d; prev_wbt_root = wbt_root[path_id[u]], u = pathup[u]){
            const int id = nodes[node_id[u]].lb;
            const node& wroot = nodes[wbt_root[path_id[u]]];
            res += query(0, id + 1, d - lca + depth[u] - id, wroot, 0) + query(id + 1, wroot.rb, d - lca + depth[u] + id, wroot, 2);
            const node& prev_wroot = nodes[prev_wbt_root];
            res -= query(0, prev_wroot.rb, d - lca + depth[u] - 1, prev_wroot, 2);
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
    void vertex_add(const int u, const T x){
        vertex_add_impl(u, x);
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