#include "../header.hpp"

// 関節点を求めるアルゴリズム(計算量は線形)
// 自己ループは存在しないことを仮定
class Articulation {
public:
    struct bcnode;
private:
	const int V;
    vector<int> ord, low;
    bool check(const int u){
        for(const int v : G[u]){
            if(ord[v] < 0) return true;
        }
        return false;
    }
    void identify_block(const int u, const int v, bool *used, 
                            stack<pair<int, int> >& st, const vector<int>& bcnode_id){
        vector<pair<int, int> > block;
        vector<int> cut_vertex;
        while(!st.empty()){
            const pair<int, int> p = st.top();
            st.pop(), block.push_back(p);
            if(art[p.first] && !used[p.first]){
                cut_vertex.push_back(p.first);
                used[p.first] = true;
            }
            if(art[p.second] && !used[p.second]){
                cut_vertex.push_back(p.second);
                used[p.second] = true;
            }
            if(p == (pair<int, int>){u, v}) break;
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
	void dfs(const int u, const int p, int& tm, bool *used,
                stack<pair<int, int> >& st, vector<int>& bcnode_id){
		ord[u] = low[u] = tm++;
        bool multi_edge = false;
		for(const int v : G[u]){
			if(ord[v] < 0){
                st.push({u, v});
				dfs(v, u, tm, used, st, bcnode_id);
				low[u] = min(low[u], low[v]);
				if(ord[u] <= low[v]){
                    if(!art[u] && (p >= 0 || check(u))){
                        art[u] = true;
                        bcnode_id[u] = (int)bctree.size();
                        bctree.push_back(vector<int>());
                        bcnodes.emplace_back(false, vector<pair<int, int> >{{u, u}});
                    }
                    identify_block(u, v, used, st, bcnode_id);
                }
			}else if(v == p){
                if(multi_edge){
                    st.push({u, v});
                    low[u] = min(low[u], ord[v]);
                }else{
                    multi_edge = true;
                }
			}else if(ord[v] < ord[u]){
                st.push({u, v});
                low[u] = min(low[u], ord[v]);
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
        : V(node_size), ord(V, -1), low(V), G(V), art(V, false){}
    void add_edge(const int a, const int b){
        G[a].push_back(b), G[b].push_back(a);
    }
	int solve(){
		int tm = 0;
        bool *used = new bool[V]();
        stack<pair<int, int> > st;
        vector<int> bcnode_id(V, -1); // bcnode_id[original vertex] = cut vertex;
        for(int i = 0; i < V; ++i){
			if(ord[i] < 0) dfs(i, -1, tm, used, st, bcnode_id);
		}
        delete[] used;
        return (int)bctree.size();
	}
};
