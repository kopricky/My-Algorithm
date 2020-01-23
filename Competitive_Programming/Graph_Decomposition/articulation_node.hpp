#include "../header.hpp"

//関節点を求めるアルゴリズム(計算量は線形)
class Articulation {
public:
    struct bcnode;
private:
    const int V;
    vector<vector<int> > dfs_tree;
    vector<int> ord, low, self_loops;
    void dfs(const int u, const int p, int& tm){
        ord[u] = low[u] = tm++;
        bool multi_edge = false;
        int ct = 0;
        for(const int v : G[u]){
            if(ord[v] < 0){
                dfs_tree[u].push_back(v);
                ++ct, dfs(v, u, tm);
                low[u] = min(low[u], low[v]);
                if(p >= 0 && ord[u] <= low[v]) art[u] = true;
            }else if(v == p){
                if(multi_edge){
                    dfs_tree[u].push_back(v);
                    low[u] = min(low[u], ord[v]);
                }else{
                    multi_edge = true;
                }
            }else if(ord[v] < ord[u]){
                dfs_tree[u].push_back(v);
                low[u] = min(low[u], ord[v]);
            }
        }
        if(p == -1 && ct > 1) art[u] = true;
    }
    bool check(const int cur, const pair<int, int> e) const {
        return ((e.first != cur && ord[cur] > low[e.first])
                || (e.second != cur && ord[cur] > low[e.second]));
    }
    void identify_block(const int u, bool *used, 
                        stack<pair<int, int> >& st, const vector<int>& bcnode_id){
        vector<pair<int, int> > block;
        vector<int> cut_vertex;
        while(!st.empty()){
            const pair<int, int> p = st.top();
            if(check(u, p)) break;
            st.pop(), block.push_back(p);
            if(art[p.first] && !used[p.first]){
                cut_vertex.push_back(p.first);
                used[p.first] = true;
            }
            if(art[p.second] && !used[p.second]){
                cut_vertex.push_back(p.second);
                used[p.second] = true;
            }
        }
        const int block_id = (int)bctree.size();
        bctree.push_back(vector<int>());
        bcnodes.emplace_back(true, move(block));
        for(const int ver : cut_vertex){
            bctree[block_id].push_back(bcnode_id[ver]);
            bctree[bcnode_id[ver]].push_back(block_id);
            used[ver] = false;
        }
    }
    void redfs(const int u, const int p, bool *visit, bool *used,
                stack<pair<int, int> >& st, vector<int>& bcnode_id){
        visit[u] = true;
        if(art[u]){
            bcnode_id[u] = (int)bctree.size();
            bctree.push_back(vector<int>());
            bcnodes.emplace_back(false, vector<pair<int, int> >{{u, u}});
        }
        for(int i = 0; i < self_loops[u]; ++i) st.push({u, u});
        if(art[u] || p == -1){
            for(const int v : dfs_tree[u]){
                if(!visit[v] && low[v] >= ord[u]){
                    st.push({u, v});
                    redfs(v, u, visit, used, st, bcnode_id);
                    // u might not be an articulation point when u is the root
                    if(art[u]) identify_block(u, used, st, bcnode_id);
                }
            }
        }
        for(const int v : dfs_tree[u]){
            if(!visit[v]){
                st.push({u, v});
                redfs(v, u, visit, used, st, bcnode_id);
            }else if(ord[v] < ord[u]){
                st.push({u, v});
            }
        }
    }
public:
    vector<vector<int> > G;
    vector<bool> art;
    struct bcnode {
        bool isBlock; // true: block, false: cut
        vector<pair<int, int> > component;
        bcnode(bool _isBlock, vector<pair<int, int> >&& _component)
            : isBlock(_isBlock), component(move(_component)){}
    };
    vector<vector<int> > bctree;
    vector<bcnode> bcnodes;
    Articulation(const int node_size)
        : V(node_size), dfs_tree(V), ord(V, -1), low(V), self_loops(V, 0), G(V), art(V, false){}
    void add_edge(const int a, const int b){
        if(a == b) ++self_loops[a];
        else G[a].push_back(b), G[b].push_back(a);
    }
    void solve(){
        int tm = 0;
        for(int i = 0; i < V; ++i){
            if(ord[i] < 0) dfs(i, -1, tm);
        }
    }
    int make_bctree(){
        bool *visit = new bool[V]();
        bool *used = new bool[V]();
        stack<pair<int, int> > st;
        vector<int> bcnode_id(V, -1); // bcnode_id[original vertex] = cut vertex;
        for(int i = 0; i < V; ++i){
            if(!visit[i]){
                redfs(i, -1, visit, used, st, bcnode_id);
                if(!st.empty()) identify_block(i, used, st, bcnode_id);
            }
        }
        delete[] visit;
        delete[] used;
        return (int)bctree.size();
    }
};
