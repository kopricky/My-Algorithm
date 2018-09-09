#include "header.hpp"

// sigmaさんのブログを参照
// http://sigma425.hatenablog.com/entry/2015/12/25/224053
// グラフGが連結であることを仮定
// 結果はtreeに格納
// Lengauer, Tarjan 1979
class DominatorTree{
public:
    struct UF{
        int V; vector<int> par,label;
        void init(int node_size){
            V = node_size; par.resize(V),label.resize(V);
            for(int i = 0; i < V; i++) par[i] = label[i] = i;
        }
        int find(int x,vector<int>& sdom){
            if(par[x] == x){
                return x;
            }else{
                int r = find(par[x],sdom);
                if(sdom[label[x]] > sdom[label[par[x]]]){
                    label[x] = label[par[x]];
                }
                return par[x] = r;
            }
        }
        void unite(int x,int y){
            par[y] = x;
        }
        int get(int x,vector<int>& sdom){
            find(x,sdom);
            return label[x];
        }
    };
    vector<vector<int> > G,rG,bucket,tree;
	vector<int> sdom,et,par,idom,dom;
    int V; UF uf;
    DominatorTree(int node_size){
        V = node_size;
        G.resize(V),rG.resize(V),bucket.resize(V),tree.resize(V);
        sdom.resize(V,-1),et.resize(V),idom.resize(V),par.resize(V),dom.resize(V);
    }
    void add_edge(int u,int v){
        G[u].push_back(v),rG[v].push_back(u);
    }
    void dfs(int v,int p,int& id){
        sdom[v] = id;
        et[id++] = v;
        par[v] = p;
        for(int w : G[v]){
            if(sdom[w] < 0){
                dfs(w,v,id);
            }
        }
    }
    void build(int root){
        uf.init(V);
        int id = 0;
        dfs(root,-1,id);
        for(int i=V-1;i>0;i--){
            int v = et[i];
            for(int w : rG[v]){
                int u = uf.get(w,sdom);
                sdom[v] = min(sdom[v],sdom[u]);
            }
            bucket[et[sdom[v]]].push_back(v);
            for(int w : bucket[par[v]]){
                dom[w] = uf.get(w,sdom);
            }
            bucket[par[v]].clear();
            uf.unite(par[v],v);
        }
        for(int i=1;i<V;i++){
            int v = et[i];
            int w = dom[v];
            if(sdom[v] == sdom[w]){
                idom[v] = sdom[v];
            }else{
                idom[v] = idom[w];
            }
        }
        for(int i=1;i<V;i++){
            int v = et[i];
            idom[v] = et[idom[v]];
        }
        idom[root] = -1;
		for(int i=0;i<V;i++){
			if(i != root){
				if(i != root){
					tree[idom[i]].push_back(i);
				}
			}
		}
    }
};
