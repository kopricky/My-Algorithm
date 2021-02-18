#include "./header.hpp"

template<typename T> class segtree {
private:
    int n, sz;
    vector<pair<T, int> > node;
public:
    void resize(const vector<T>& v){
        n = 1, sz = (int)v.size();
        while(n < sz) n *= 2;
        node.resize(2 * n);
        for(int i = 0; i < sz; ++i){
            node[i+n] = make_pair(v[i], i);
        }
        for(int i = n - 1; i >= 1; --i){
            node[i] = min(node[2 * i], node[2 * i + 1]);
        }
    }
    pair<T, int> query(int a, int b) const {
        pair<T, int> res1 = make_pair(numeric_limits<T>::max(), -1);
        pair<T, int> res2 = make_pair(numeric_limits<T>::max(), -1);
        a += n, b += n;
        while(a != b){
            if(a % 2) res1 = min(res1, node[a++]);
            if(b % 2) res2 = min(res2, node[--b]);
            a >>= 1, b >>= 1;
        }
        return min(res1, res2);
    }
};

template<typename T> class PathAdd_PathSum_with_CD {
private:
    int V, root;
    vector<vector<T> > psm;
    vector<vector<int> > G, tree, coef, chid;
    vector<T> sm, plus;
    segtree<int> st;
    vector<int> sz, rch, par, depth, id, ord;
    vector<bool> used;
    void calcSize(const int u, const int p){
        sz[u] = 1;
        for(const int v : G[u]){
            if(!used[v] && v != p){
                calcSize(v, u);
                sz[u] += sz[v];
            }
        }
    }
    void rdfs(const int u, const int p, const int d, vector<int>& vec){
        vec[u] = d;
        for(const int v : G[u]){
            if(!used[v] && v != p){
                rdfs(v, u, d, vec);
            }
        }
    }
    void dfs(const int u, const int p, const int num, vector<int>& vec){
        vec[u] = num;
        for(const int v : G[u]){
            if(!used[v] && v != p){
                dfs(v, u, num, vec);
            }
        }
    }
    void cdBuild(int u, const int p, const int num, const int dep){
        calcSize(u, -1);
        int tot = sz[u], pp = -1;
        bool ok = false;
        while(!ok){
            ok = true;
            for(const int v : G[u]){
                if(!used[v] && v != pp && 2 * sz[v] > tot){
                    pp = u, u = v, ok = false;
                    break;
                }
            }
        }
        if((int)coef.size() == dep){
            coef.push_back(vector<int>(V)), chid.push_back(vector<int>(V, -1));
        }
        if(p >= 0) tree[p][num] = u;
        else root = u;
        int cnt = 0;
        for(int v : G[u]){
            if(used[v]) continue;
            if(depth[v] < depth[u]){
                rch[u] = cnt;
                int prv = u, nx;
                for(; v >= 0; prv = v, v = nx){
                    nx = -1, coef[dep][v] = depth[v];
                    for(const int w : G[v]){
                        if(used[w] || w == prv) continue;
                        if(depth[w] < depth[v]) nx = w;
                        else rdfs(w, v, depth[v], coef[dep]);
                    }
                }
            }else{
                dfs(v, u, cnt, chid[dep]);
            }
            ++cnt;
        }
        psm[u].resize(cnt, 0), tree[u].resize(cnt), used[u] = true;
        int bue = 0;
        for(const int v : G[u]){
            if(!used[v]) cdBuild(v, u, bue++, dep + 1);
        }
    }
    void path_add_impl(const int u, const T x){
        for(int cur = root, d = 0;; ++d){
            if(cur == u){
                sm[cur] += x;
                break;
            }
            if(chid[d][u] >= 0) psm[cur][chid[d][u]] += x, sm[cur] += x, cur = tree[cur][chid[d][u]];
            else plus[cur] += x * coef[d][u], cur = tree[cur][rch[cur]];
        }
    }
    T path_sum_impl(const int u) const {
        T res = 0;
        for(int cur = root, d = 0;; ++d){
            if(cur == u){
                res += plus[cur] + sm[cur] * depth[cur];
                break;
            }
            if(chid[d][u] >= 0)  res += plus[cur] + (sm[cur] - psm[cur][chid[d][u]]) * depth[cur], cur = tree[cur][chid[d][u]];
            else res += sm[cur] * coef[d][u], cur = tree[cur][rch[cur]];
        }
        return res;
    }
    void lca_dfs(const int u, const int p, const int k){
        id[u] = (int)ord.size(), ord.push_back(u), par[u] = p, depth[u] = k;
        for(const int v : G[u]){
            if(v != p){
                lca_dfs(v, u, k + 1);
                ord.push_back(u);
            }
        }
    }
    void lcaBuild(){
        ord.reserve(2 * V - 2);
        lca_dfs(0, -1, 1);
        vector<int> stvec(2 * V - 2);
        for(int i = 0; i < 2 * V - 2; ++i){
            stvec[i] = depth[ord[i]];
        }
        st.resize(stvec);
    }
    int get_lca(const int u, const int v) const {
        return ord[st.query(min(id[u], id[v]), max(id[u], id[v]) + 1).second];
    }
public:
    PathAdd_PathSum_with_CD(const int node_size)
         : V(node_size), psm(V), G(V), tree(V), sm(V, 0), plus(V, 0), 
                sz(V), rch(V), par(V, -1), depth(V), id(V, -1), used(V, false){}
    void add_edge(const int u, const int v){
        G[u].push_back(v), G[v].push_back(u);
    }
    void build(){
        lcaBuild();
        cdBuild(0, -1, -1, 0);
    }
    void path_add(const int u, const int v, const T x){
        const int lca = get_lca(u, v);
        path_add_impl(u, x), path_add_impl(v, x), path_add_impl(lca, -x);
        if(par[lca] >= 0) path_add_impl(par[lca], -x);
    }
    T path_sum(const int u, const int v) const {
        T res = 0;
        const int lca = get_lca(u, v);
        res += path_sum_impl(u) + path_sum_impl(v) - path_sum_impl(lca);
        if(par[lca] >= 0) res -= path_sum_impl(par[lca]);
        return res;
    }
};