#include "../header.hpp"

template<typename T> class segtree {
private:
    int n,sz;
    vector<pair<T, int> > node;
public:
    void resize(vector<T>& v){
        sz = (int)v.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.resize(2*n);
        for(int i = 0; i < sz; i++){
            node[i+n] = make_pair(v[i], i);
        }
        for(int i=n-1; i>=1; i--){
            node[i] = min(node[2*i], node[2*i+1]);
        }
    }
    pair<T, int> query(int a,int b) const {
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

class LCA{
public:
    int V;
    vector<vector<int> > G;
    vector<int> ord,depth,id;
    segtree<int> st;
    LCA(int node_size) : V(node_size), G(V), depth(V), id(V, -1){}
    void add_edge(int from,int to){
        G[from].push_back(to),G[to].push_back(from);
    }
    void dfs(int u,int p,int k){
        id[u] = (int)ord.size();
        ord.push_back(u);
        depth[u] = k;
        for(int v : G[u]){
            if(v != p){
                dfs(v,u,k+1);
                ord.push_back(u);
            }
        }
    }
    void build(){
        ord.reserve(2*V-2);
        for(int i = 0; i < V; i++){
            if(id[i] < 0){
                dfs(i,-1,0);
            }
        }
        vector<int> stvec(2*V-2);
    	for(int i = 0; i < 2*V-2; i++){
    		stvec[i] = depth[ord[i]];
    	}
        st.resize(stvec);
    }
    int solve(int u,int v) const {
        return ord[st.query(min(id[u],id[v]),max(id[u],id[v])+1).second];
    }
    int dist(int u,int v) const {
        int lca = solve(u,v);
        return depth[u] + depth[v] - 2*depth[lca];
    }
};

// 初めにコンストラクトして以降クエリとして呼ぶ
 class VirtualTree {
public:
    // ver_list 内の頂点から構成されるグラフの virtual tree を virtual_tree に格納し, その 根(virtual_tree 上の頂点番号) を返す.
    // mp: "元の木上の頂点番号" から "virtual_tree 上の頂点番号" へのマッピング
    // imp: "virtual_tree 上の頂点番号" から "元の木上の頂点番号" へのマッピング
    // virtual_tree は根から子に向かう一方向の辺しかない
    const LCA& lca;
    vector<vector<int> > virtual_tree;
    vector<int> mp, imp;
    VirtualTree(const LCA& _lca) : lca(_lca), mp(_lca.V){}
    int construct_virtual_tree(const vector<int>& ver_list){
        const int n = (int)ver_list.size();
        virtual_tree.assign(n, vector<int>());
        imp = ver_list;
        const auto& index = lca.id;
        const auto& depth = lca.depth;
        sort(imp.begin(), imp.end(), [&](const int a, const int b){ return index[a] < index[b]; });
        stack<int> st;
        st.push(imp[0]), mp[imp[0]] = 0;
        for(int i = 0; i < n-1; ++i){
            const int u = lca.solve(imp[i], imp[i+1]);
            if(u != imp[i]){
                int mapped_ver = mp[st.top()];
                while(true){
                    st.pop();
                    if(st.empty() || depth[u] >= lca.depth[st.top()]) break;
                    const int tmp = mp[st.top()];
                    virtual_tree[tmp].push_back(mapped_ver), mapped_ver = tmp;
                }
                if(st.empty() || st.top() != u){
                    st.push(u), imp.push_back(u);
                    mp[u] = (int)virtual_tree.size();
                    virtual_tree.push_back({mapped_ver});
                }else{
                    virtual_tree[mp[u]].push_back(mapped_ver);
                }
            }
            st.push(imp[i+1]), mp[imp[i+1]] = i+1;
        }
        int mapped_ver = ((st.size() > 1) ? mp[st.top()] : -1);
        while(st.size() > 1){
            st.pop();
            const int tmp = mp[st.top()];
            virtual_tree[tmp].push_back(mapped_ver), mapped_ver = tmp;
        }
        return mp[st.top()];
    }
};