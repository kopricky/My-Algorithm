#include "header.hpp"

template<typename T> class LinkCutTree {
public:
    inline static T opr1(const T arg1, const T arg2){
        return max(arg1, arg2);
    }
    struct node {
        int id, al_id;
        T val, al;
        node *left, *right, *par;
        bool rev;
        node(){}
        node(int _id, T _val) : id(_id), al_id(_id), val(_val), al(_val),
            left(nullptr), right(nullptr), par(nullptr), rev(false){}
        inline bool isRoot() const {
            return (!par) || (par->left != this && par->right != this);
        }
        void push(){
            if(!rev) return;
            swap(left, right);
            if(left) left->rev = !(left->rev);
            if(right) right->rev = !(right->rev);
            rev = false;
        }
        void eval(){
            al_id = id, al = val;
            if(left){
                left->push();
                if(al < left->al){
                    al = left->al, al_id = left->al_id;
                }
            }
            if(right){
                right->push();
                if(al < right->al){
                    al = right->al, al_id = right->al_id;
                }
            }
        }
    };

private:
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
    void splay(node* u){
        while(!(u->isRoot())){
            node *p = u->par, *gp = p->par;
            if(p->isRoot()){ // zig
                p->push(), u->push();
                rotate(u, (u == p->left));
            }else{
                gp->push(), p->push(), u->push();
                bool flag = (u == p->left);
                if((u == p->left) == (p == gp->left)){
                    rotate(p, flag), rotate(u, flag);
                }else{
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
    void evert(node* u){
        access(u);
        u->rev = !(u->rev), u->push();
    }
    bool connected(node* u, node* v){
        access(u), access(v);
        return u->par;
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
    pair<T, int> query(node* u, node* v){
        evert(u), access(v);
        return make_pair(v->al, v->al_id);
    }
public:
    node** arr;
    LinkCutTree(int node_size){
        arr = new node*[node_size];
        for(int i = 0; i < node_size; i++){
            arr[i] = new node(-1, numeric_limits<T>::min());
        }
    }
    static node *make_node(){ return new node(); }
    bool connected(int id1, int id2){ return connected(arr[id1], arr[id2]); }
    void link(int ver_id, node *edge){ return link(arr[ver_id], edge); }
    void cut(int ver_id, node *edge){ return cut(arr[ver_id], edge); }
    pair<T, int> query(int id1, int id2){ return query(arr[id1], arr[id2]); }
};

class RetroactiveUnionFind {
private:
    const unsigned int V;
    LinkCutTree<int> lc;
    vector<LinkCutTree<int>::node> edge;
    vector<pair<int, int> > nodes;
    unsigned int cur_pos;

public:
    RetroactiveUnionFind(unsigned int node_size)
     : V(node_size), lc(V), edge(V - 1), nodes(V - 1), cur_pos(0u){}
    bool unite(const int id1, const int id2, int _time){
        unsigned int pos = cur_pos;
        if(cur_pos == V - 1 || lc.connected(id1, id2)){
            auto res = lc.query(id1, id2);
            if(res.first <= _time) return false;
            pair<int, int> p = nodes[pos = res.second];
            lc.cut(p.first, &edge[pos]), lc.cut(p.second, &edge[pos]);
        }else{
            ++cur_pos;
        }
        edge[pos] = LinkCutTree<int>::node(pos, _time);
        lc.link(id1, &edge[pos]), lc.link(id2, &edge[pos]);
        nodes[pos] = {id1, id2};
        return true;
    }
    bool same(const int id1, const int id2, int _time){
        if(!lc.connected(id1, id2)) return false;
        return lc.query(id1, id2).first <= _time;
    }
};
