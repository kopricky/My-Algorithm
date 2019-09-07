#include "../header.hpp"

class biconnected {
private:
    void dfs(int u, int p, int &k, int q)
    {
        visit[u] = true;
        ord[u] = k++;
        low[u] = ord[u];
        for(int i = 0; i < (int)G[u].size(); i++){
            int v = G[u][i];
            if(!visit[v]){
                dfs(v, u, k, i);
                low[u] = min(low[u], low[v]);
                if(ord[u] < low[v]){
                    bridge.emplace_back(u, v);
                    check[u][i].second = 1;
                    check[v][check[u][i].first].second = 1;
                }
            }else if(v != p || q != check[u][i].first){
                low[u] = min(low[u], ord[v]);
            }
        }
    }
    void restrict_dfs(int u, int p, int kind, queue<int>& que)
    {
        visit[u] = true;
        cmp[u] = kind;
        for(int i = 0; i < (int)G[u].size(); i++){
            int v = G[u][i];
            if(check[u][i].second){
                if(!visit[v]){
                    que.push(v);
                }
            }else if(!visit[v]){
                restrict_dfs(v, u, kind, que);
            }
        }
    }
    void rebuild()
    {
        kind = 0;
        fill(visit, visit + V, false);
        for(int i = 0; i < V; i++){
            if(!visit[i]){
                queue<int> que;
                que.push(i);
                while(!que.empty()){
                    int p = que.front();
                    que.pop();
                    restrict_dfs(p, -1, kind, que);
                    kind++;
                }
            }
        }
    }
public:
	int V, kind;
	vector<vector<int> > G, tree;
	vector<vector<pair<int, int> > > check;
    vector<pair<int, int> > bridge;
	vector<int> ord, low, cmp;
    bool *visit;
	biconnected(int node_size)
        : V(node_size), kind(0), G(V), check(V), ord(V), low(V)
			,cmp(V), visit(new bool[V]()){}
    // ~biconnected(){ delete[] visit; }
    void add_edge(int u, int v)
    {
        G[u].push_back(v), G[v].push_back(u);
        check[u].emplace_back((int)check[v].size(), 0);
        check[v].emplace_back((int)check[u].size()-1, 0);
    }
    // 橋を検出する.
	void detect_bridge(){
		int id = 0;
		for(int i = 0; i < V; i++){
			if(!visit[i]){
				dfs(i, -1, id, -1);
			}
		}
	}
	// 2(辺)連結成分を頂点とする木(tree) を作る(事前に detect_bridge() を呼んでおく必要がある).
    // 木の頂点数を返す.
    // cmp[i] は元の頂点 i の含まれる頂点(2(辺)連結成分) を表す.
	int make_bctree()
	{
		rebuild();
        tree.resize(kind);
		for(int i = 0; i < (int)bridge.size(); i++){
			int a = cmp[bridge[i].first], b = cmp[bridge[i].second];
			tree[a].push_back(b), tree[b].push_back(a);
		}
        return kind;
	}
};
