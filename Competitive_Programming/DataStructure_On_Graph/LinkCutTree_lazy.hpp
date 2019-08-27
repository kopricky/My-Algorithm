#include "../header.hpp"

// 以下の実装では link, range, query などで reroot が起こる.
template<typename T> class LinkCutTree {
public:
    struct node {
        int sz;
        T val, al, lazy;
        node *left, *right, *par;
        bool rev;
        node() : sz(1), val(id2), al(id2), lazy(id1),
            left(nullptr), right(nullptr), par(nullptr), rev(false){}
        node(T _val) : sz(1), val(_val), al(_val), lazy(id1),
            left(nullptr), right(nullptr), par(nullptr), rev(false){}
        inline bool isRoot() const {
            return (!par) || (par->left != this && par->right != this);
        }
        void push(){
            if(lazy != id1){
                opr1(val, lazy), al += lazy * sz;
                if(left) opr1(left->lazy, lazy);
                if(right) opr1(right->lazy, lazy);
                lazy = id1;
            }
            if(!rev) return;
            swap(left, right);
            if(left) left->rev = !(left->rev);
            if(right) right->rev = !(right->rev);
            rev = false;
        }
        void eval(){
            sz = 1, al = val;
            if(left) left->push(), sz += left->sz, al = (rev ? opr2(al, left->al) : opr2(left->al, al));
            if(right) right->push(), sz += right->sz, al = (rev ? opr2(right->al, al) : opr2(al, right->al));
        }
    };
    static const T id1 = (T)0;
    static const T id2 = (T)0;
    inline static void opr1(T& arg1, const T arg2){
        arg1 += arg2;
    }
    inline static T opr2(const T arg1, const T arg2){
        return arg1 + arg2;
    }

private:
    // 回転
    void rotate(node* u, bool right){
        node *p = u->par, *g = p->par;
        if(right){
            if((p->left = u->right)) u->right->par = p;
            u->right = p, p->par = u;
        }else{
            if((p->right = u->left)) u->left->par = p;
            u->left = p, p->par = u;
        }
        p->eval(), u->eval(), u->par = g;
        if(!g) return;
        if(g->left == p) g->left = u;
        if(g->right == p) g->right = u;
        g->eval();
    }
    // u を splay 木の根にする
    void splay(node* u){
        while(!(u->isRoot())){
            node *p = u->par, *gp = p->par;
            if(p->isRoot()){ // zig
                p->push(), u->push();
                rotate(u, (u == p->left));
            }else{
                gp->push(), p->push(), u->push();
                bool flag = (u == p->left);
                if((u == p->left) == (p == gp->left)){ // zig-zig
                    rotate(p, flag), rotate(u, flag);
                }else{ // zig-zag
                    rotate(u, flag), rotate(u, !flag);
                }
            }
        }
        u->push();
    }
    node* access(node* u){
        node* last = nullptr;
        for(node* v = u; v; v = v->par){
            splay(v);
            v->right = last;
            v->eval();
            last = v;
        }
        splay(u);
        return last;
    }
    bool connected(node* u, node* v){
        access(u), access(v);
        return u == v || u->par;
    }
    void link(node* u, node* v){
        evert(u), u->par = v;
    }
    void cut(node* u){
        access(u);
        u->left->par = nullptr, u->left = nullptr, u->eval();
    }
    void cut(node* u, node* v){
        access(u), access(v);
        if(u->isRoot()) u->par = nullptr;
        else v->left->par = nullptr, v->left = nullptr, v->eval();
    }
    node* lca(node* u, node* v){
        access(u);
        return access(v);
    }
    void evert(node* u){
        access(u);
        u->rev = !(u->rev), u->push();
    }
    int depth(node* u){
        access(u);
        return u->sz - 1;
    }
    void toRoot_range(node* u, const T x){
        access(u);
        opr1(u->lazy, x), u->push();
    }
    void toRoot_query(node* u){
        access(u);
        return u->al;
    }
    void range(node* u, node* v, const T x){
        evert(u), access(v);
        opr1(v->lazy, x), v->push();
    }
    T query(node* u, node* v){
        evert(u), access(v);
        return v->al;
    }

public:
    const int V;
    node** arr;
    LinkCutTree(const vector<T>& init_node_value)
     : V((int)init_node_value.size()){
        arr = new node*[V];
        for(int i = 0; i < V; ++i){
            arr[i] = new node(init_node_value[i]);
        }
    }
    // ~LinkCutTree(){
    //     for(int i = 0; i < V; ++i){
    //         delete arr[i];
    //     }
    //     delete[] arr;
    // }
    // id1 と id2 が同じ木(連結成分)に属するか
    bool connected(int id1, int id2){ return connected(arr[id1], arr[id2]); }
    // id1 を id2 の non-preferred edge にする
    void link(int id1, int id2){ return link(arr[id1], arr[id2]); }
    // id とその親の間の辺を削除する
    void cut(int id){ return cut(arr[id]); }
    // id と id2 の間の辺を削除する
    void cut(int id1, int id2){ return cut(arr[id1], arr[id2]); }
    // id1 と id2 の LCA を求める
    int lca(int id1, int id2){ return static_cast<size_t>(lca(arr[id1], arr[id2]) - arr[0]); }
    // 元の木の根を id にする
    void evert(int id){ return evert(arr[id]); }
    // id の深さを求める
    int depth(int id){ return depth(arr[id]); }
    // id から根までの間にある頂点すべてに x を足す
    void toRoot_range(int id, const T x){ return toRoot_range(arr[id], x); }
    // id から根までの間にある頂点すべてのコストの総和を求める
    void toRoot_query(int id){ return toRoot_query(arr[id]); }
    // id1 と id2 の間にある頂点すべてに x を足す
    void range(int id1, int id2, const T x){ return range(arr[id1], arr[id2], x); }
    // id1 と id2 の間にある頂点すべてのコストの総和を求める
    T query(int id1, int id2){ return query(arr[id1], arr[id2]); }
};
