#include "header.hpp"

//最小全域有向木(O(mlogn))
//joisinoさんのブログを参照
//http://joisino.hatenablog.com/entry/2017/01/11/230141
//Tarjan 1977
class UF {
private:
    int sz; vector<int> par;
public:
    UF(){}
    UF(int node_size){
        sz = node_size;
        par.resize(sz);
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

template<typename T> class LazyHeap {
public:
    struct node{
        node *l, *r;
        T val, add;
        int id;
        node(T t, int i) : l(nullptr), r(nullptr), val(t), add(0), id(i){}
    };
    void lazy(node* a){
        if(a->l) a->l->add += a->add;
        if(a->r) a->r->add += a->add;
        a->val += a->add;
        a->add = 0;
    }
    node* meld(node* a, node* b){
        if(!a) return b; if(!b) return a;
        if(a->val+a->add > b->val+b->add) swap(a, b);
        lazy(a);
        a->r = meld(a->r,b);
        swap(a->l, a->r);
        return a;
    }
    node* push(node* org, T val, int i){
        node* p = new node(val, i);
        return meld(org, p);
    }
    node* pop(node* p){
        lazy(p);
        return meld(p->l, p->r);
    }
};

template<typename T> class Arborescence {
public:
    struct edge{
        int from, to;
        T cost;
    };
    LazyHeap<T> heap;
    vector<edge> es;
    vector<int> used, from, from_cost;
    vector<typename LazyHeap<T>::node*> come;
    int V;
    Arborescence(int node_size){
        V = node_size;
        used.resize(V, 0), from.resize(V), from_cost.resize(V);
        come.resize(V, nullptr);
    }
    void add_edge(int u, int v, T cost){
        es.push_back((edge){u,v,cost});
    }
    //rを根とする最小全域有向木のコストを計算
    T solve(int r){
        used[r] = 2;
        UF uf(V);
        rep(i,(int)es.size()){
            edge& e = es[i];
            come[e.to] = heap.push(come[e.to], e.cost, i);
        }
        T ans = 0;
        rep(i,V){
            if(used[i] != 0) continue;
            int cur = i;
            vector<int> path;
            while(used[cur] != 2){
                used[cur] = 1;
                path.push_back(cur);
                //全域有向木が存在しない場合
                if(!come[cur]) return numeric_limits<T>::max();
                from[cur] = uf.find(es[come[cur]->id].from);
                from_cost[cur] = come[cur]->val + come[cur]->add;
                come[cur] = heap.pop(come[cur]);
                if(from[cur] == cur) continue;
                ans += from_cost[cur];
                if(used[from[cur]] == 1){
                    int p = cur;
                    do{
                        if(come[p]){
                            come[p]->add -= from_cost[p];
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
            for(int v : path){
                used[v] = 2;
            }
        }
        return ans;
    }
};
