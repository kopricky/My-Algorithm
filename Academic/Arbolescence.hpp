#include "header.hpp"

//最小全域有向木(O(mlogn))
//joisinoさんのブログを参照
//http://joisino.hatenablog.com/entry/2017/01/11/230141
//Tarjan 1977
class UnionFind {
private:
    int sz; vector<int> par;
public:
    UnionFind(){}
    UnionFind(int node_size) : sz(node_size), par(sz){
        iota(par.begin(),par.end(),0);
    }
    int find(int x){
        if(par[x] == x) return x;
        else return par[x] = find(par[x]);
    }
    void unite(int x,int y){
        x = find(x), y = find(y);
        if(x == y) return;
    	par[x] = y;
    }
    bool same(int x,int y){
        return find(x) == find(y);
    }
};

//マージ可能なヒープ(skew heap)
template<typename T> class LazyHeap {
public:
    struct node{
        node *l, *r;
        T val, lazy;
        int id;
        node(T t, int i) : l(nullptr), r(nullptr), val(t), lazy(0), id(i){}
    };
    void push(node* a){
        if(a->l) a->l->lazy += a->lazy;
        if(a->r) a->r->lazy += a->lazy;
        a->val += a->lazy;
        a->lazy = 0;
    }
    node* meld(node* a, node* b){
        if(!a) return b;
        if(!b) return a;
        if(a->val+a->lazy > b->val+b->lazy) swap(a, b);
        push(a);
        a->r = meld(a->r,b);
        swap(a->l, a->r);
        return a;
    }
    node* insert(node* org, T val, int i){
        node* p = new node(val, i);
        return meld(org, p);
    }
    node* erase(node* p){
        push(p);
        return meld(p->l, p->r);
    }
};

template<typename T> class Arborescence {
public:
    // ここでの edge の cost は枝の重みというよりは枝を取り替えたときの差分を持っている
    struct edge{
        int from, to;
        T cost;
    };
    int V;
    LazyHeap<T> heap;
    vector<edge> es;
    vector<int> used, from, from_cost;
    // 各頂点ごとに入ってくる辺を skew heap で管理
    vector<typename LazyHeap<T>::node*> come;
    Arborescence(int node_size) : V(node_size), used(V, 0),
        from(V), from_cost(V), come(V, nullptr){}
    void add_edge(int u, int v, T cost){
        es.push_back((edge){u,v,cost});
    }
    //rを根とする最小全域有向木のコストを計算
    T solve(int r){
        used[r] = 2;
        UnionFind uf(V);
        for(int i = 0; i < (int)es.size(); i++){
            edge& e = es[i];
            come[e.to] = heap.insert(come[e.to], e.cost, i);
        }
        T ans = 0;
        for(int i = 0; i < V; i++){
            // i に入ってくる辺がすでに存在する場合
            if(used[i] != 0) continue;
            int cur = i;
            vector<int> path;
            while(used[cur] != 2){
                used[cur] = 1;
                path.push_back(cur);
                //全域有向木が存在しない場合
                if(!come[cur]) return numeric_limits<T>::max();
                from[cur] = uf.find(es[come[cur]->id].from);
                from_cost[cur] = come[cur]->val + come[cur]->lazy;
                come[cur] = heap.erase(come[cur]);
                // 自己ループの場合
                if(from[cur] == cur) continue;
                ans += from_cost[cur];
                // サイクルをたどって１つの頂点に圧縮する
                if(used[from[cur]] == 1){
                    int p = cur;
                    do{
                        if(come[p]){
                            come[p]->lazy -= from_cost[p];
                        }
                        if(p != cur){
                            uf.unite(p,cur);
                            come[cur] = heap.meld(come[cur],come[p]);
                        }
                        p = uf.find(from[p]);
                    }while(p != cur);
                }else{
                    cur = from[cur];
                }
            }
            // 頂点を確定済みにする
            for(int v : path){
                used[v] = 2;
            }
        }
        return ans;
    }
};
