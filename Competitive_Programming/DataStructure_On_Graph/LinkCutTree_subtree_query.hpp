#include "../header.hpp"

// パス加算総和 + 部分木サイズ/総和
template<typename T> class LinkCutTree {
public:
    struct node {
        int sz, sub1, sub2;
        T val, al, lazy, sum1, sum2;
        node *left, *right, *par;
        bool rev;
        node(const T _val) : sz(1), sub1(1), sub2(0), val(_val), al(_val), lazy(0), sum1(_val), sum2(0),
            left(nullptr), right(nullptr), par(nullptr), rev(false){}
        bool isRoot() const {
            return (!par) || (par->left != this && par->right != this);
        }
        void push(){
            if(lazy != 0){
                val += lazy, al += lazy * sz;
                if(left) left->lazy += lazy;
                if(right) right->lazy += lazy;
                lazy = 0;
            }
            if(!rev) return;
            swap(left, right);
            if(left) left->rev = !(left->rev);
            if(right) right->rev = !(right->rev);
            rev = false;
        }
        void eval(){
            sz = 1, sub1 = sub2 + 1, al = val, sum1 = sum2 + val;
            if(left) left->push(), sz += left->sz, sub1 += left->sub1, al += left->al, sum1 += left->sum1;
            if(right) right->push(), sz += right->sz, sub1 += right->sub1, al += right->al, sum1 += right->sum1;
        }
    };

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
            if(v->right) v->sub2 += v->right->sub1, v->sum2 += v->right->sum1;
            v->right = last;
            if(v->right) v->sub2 -= v->right->sub1, v->sum2 -= v->right->sum1;
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
        evert(u), access(v);
        u->par = v, v->sub2 += u->sub1, v->sum2 += u->sum1;
    }
    void cut(node* u){
        access(u);
        u->left->par = nullptr, u->left = nullptr, u->eval();
    }
    void cut(node* u, node* v){
        access(u), access(v);
        if(u->isRoot()) v->sub2 -= u->sub1, v->sum2 -= u->sum1, u->par = nullptr;
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
        u->lazy += x, u->push();
    }
    T toRoot_query(node* u){
        access(u);
        return u->al;
    }
    void range(node* u, node* v, const T x){
        evert(u), access(v);
        v->lazy += x, v->push();
    }
    T query(node* u, node* v){
        evert(u), access(v);
        return v->al;
    }
    int component_size(node* u){
        evert(u);
        return u->sub1;
    }
    int subtree_size(node* u){
        access(u);
        return u->sub2 + 1;
    }
    T component_query(node* u){
        evert(u);
        return u->sum1;
    }
    T subtree_query(node* u){
        access(u);
        return u->sum2 + u->val;
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
    T toRoot_query(int id){ return toRoot_query(arr[id]); }
    // id1 と id2 の間にある頂点すべてに x を足す
    void range(int id1, int id2, const T x){ return range(arr[id1], arr[id2], x); }
    // id1 と id2 の間にある頂点すべてのコストの総和を求める
    T query(int id1, int id2){ return query(arr[id1], arr[id2]); }
    // id の属する連結成分のサイズを返す
    int component_size(int id){ return component_size(arr[id]); }
    // id を根とする部分木のサイズを返す
    int subtree_size(int id){ return subtree_size(arr[id]); }
    // id の属する連結成分内のコストの総和を返す
    T component_query(int id){ return component_query(arr[id]); }
    // id を根とする部分木内のコストの総和を返す
    T subtree_query(int id){ return subtree_query(arr[id]); }
};