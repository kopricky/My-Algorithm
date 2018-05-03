#include "../header.hpp"

template<typename T> class SparseTable {
private:
    vector<T> Arr;
    vector<int> LogTable;
    vector<vector<int> > Table; //最小値のインデックスを保持
    int sz;
public:
    void resize(vector<T>& v){
        sz = (int)v.size();
        Arr = v;
        LogTable.resize(sz+1);
        for(int i = 2; i < sz + 1; i++){
            LogTable[i] = LogTable[i >> 1] + 1;
        }
        Table.resize(sz,vector<T>(LogTable[sz]+1));
        rep(i,sz){
            Table[i][0] = i;
        }
        for(int k = 1; (1 << k) <= sz; k++){
            for(int i = 0; i + (1 << k) <= sz; i++){
                int s = Table[i][k-1];
                int t = Table[i + (1 << (k-1))][k-1];
                if(Arr[s] < Arr[t]){
                    Table[i][k] = s;
                }else{
                    Table[i][k] = t;
                }
            }
        }
    }
    pair<T,int> query(int l,int r){
        int k = LogTable[r-l];
        if(Arr[Table[l][k]] < Arr[Table[r-(1<<k)][k]]){
            return make_pair(Arr[Table[l][k]],Table[l][k]);
        }else{
            return make_pair(Arr[Table[r-(1<<k)][k]],Table[r-(1<<k)][k]);
        }
    }
};


class LCA{
public:
    vector<int> ord,depth,id;
    vector<vector<int> > G;
    SparseTable<int> st;
    int V;
    LCA(int node_size){
        V = node_size;
        G.resize(V),depth.resize(V),id.resize(V,-1);
    }
    void add_edge(int from,int to){
        G[from].push_back(to),G[to].push_back(from);
    }
    void dfs(int u,int p,int k){
        id[u] = (int)ord.size();
        ord.push_back(u);
        depth[u] = k;
        rep(i,G[u].size()){
            if(G[u][i] != p){
                dfs(G[u][i],u,k+1);
                ord.push_back(u);
            }
        }
    }
    void build(){
        rep(i,V){
            if(id[i] < 0){
                dfs(i,-1,0);
            }
        }
        vector<int> stvec((int)ord.size());
    	rep(i,ord.size()){
    		stvec[i] = depth[ord[i]];
    	}
        st.resize(stvec);
    }
    int solve(int u,int v){
        return ord[st.query(min(id[u],id[v]),max(id[u],id[v])+1).second];
    }
    int dist(int u,int v){
        int lca = solve(u,v);
        return depth[u] + depth[v] - 2*depth[lca];
    }
};
