#include "../header.hpp"

// 多重辺が存在しないことを仮定
class FindingAllCycles {
private:
    struct node {
        int prev, next, to;
        node(const int _prev, const int _next, const int _to)
            : prev(_prev), next(_next), to(_to){}
    };
    const int V;
    vector<vector<node> > G;
    vector<stack<int> > block_stack;
    stack<int> st;
    bool *fix, *blocked, *used;
    bool enumerate;
    void erase_edge(const int u, const int id){
        G[u][G[u][id].next].prev = G[u][id].prev;
        G[u][G[u][id].prev].next = G[u][id].next;
    }
    void scc_dfs(const int u, int& tm, int& cnt,
            vector<int>& ord, vector<int>& low, vector<int>& cmp, stack<int>& st){
        ord[u] = low[u] = tm++, st.push(u);
        for(int id = G[u][0].next; id != 0; id = G[u][id].next){
            const int v = G[u][id].to;
            if(ord[v] < 0){
                scc_dfs(v, tm, cnt, ord, low, cmp, st);
                low[u] = min(low[u], low[v]);
            }else if(cmp[v] < 0){
                low[u] = min(low[u], ord[v]);
            }
            if(cmp[v] >= 0) erase_edge(u, id);
        }
        if(ord[u] == low[u]){
            while(true){
                const int v = st.top();
                st.pop(), cmp[v] = cnt;
                if(v == u) break;
            }
            ++cnt;
        }
    }
    void scc(){
        vector<int> ord(V, -1), low(V), cmp(V, -1);
        stack<int> st;
        int tm = 0, cnt = 0;
        for(int i = 0; i < V; ++i){
            if(ord[i] < 0) scc_dfs(i, tm, cnt, ord, low, cmp, st);
        }
    }
    void unblock(const int u){
        blocked[u] = false;
        while(!block_stack[u].empty()){
            const int v = block_stack[u].top();
            block_stack[u].pop();
            if(blocked[v]) unblock(v);
        }
    }
    bool dfs(const int u, const int s, vector<int>& path, vector<int>& ver_list){
        bool flag = false;
        path.push_back(u);
        blocked[u] = true;
        if(!used[u]) used[u] = true, ver_list.push_back(u);
        for(int id = G[u][0].next; id != 0; id = G[u][id].next){
            const int v = G[u][id].to;
            if(fix[v]){
                erase_edge(u, id);
                continue;
            }
            if(v == s){
                if(enumerate) ans.push_back(path);
                ++count, flag = true;
            }else if(!blocked[v]){
                if(dfs(v, s, path, ver_list)) flag = true;
            }
        }
        if(flag) unblock(u);
        else{
            for(int id = G[u][0].next; id != 0; id = G[u][id].next){
                block_stack[G[u][id].to].push(u);
            }
        }
        path.pop_back();
        return flag;
    }

public:
    vector<vector<int> > ans;
    int count;
    FindingAllCycles(const int node_size)
        : V(node_size), G(V, vector<node>{{0, 0, -1}}), block_stack(V),
            fix(new bool[V]()), blocked(new bool[V]()), used(new bool[V]()), count(0){}
    ~FindingAllCycles(){
        delete[] fix, delete[] blocked, delete[] used;
    }
    void add_edge(const int u, const int v){
        if(u == v){
            ans.push_back({u});
            return;
        }
        G[u][0].prev = G[u].back().next = (int)G[u].size();
        G[u].push_back((node){(int)G[u].size() - 1, 0, v});
    }
    int solve(bool _enumerate=true){
        scc();
        enumerate = _enumerate;
        for(int i = 0; i < V; ++i){
            vector<int> path, ver_list;
            dfs(i, i, path, ver_list);
            fix[i] = true;
            for(int j : ver_list){
                used[j] = blocked[j] = false, block_stack[j] = stack<int>();
            }
        }
        return count;
    }
};