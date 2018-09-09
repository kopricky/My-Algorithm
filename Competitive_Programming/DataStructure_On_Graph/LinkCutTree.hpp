#include "../header.hpp"

//根はそれぞれのSplay木の根と全体の木の根の２つの意味がある

struct node {
    int id;
    node *left, *right, *par;
    bool rev;
    node(int num) : id(num), left(nullptr), right(nullptr), par(nullptr), rev(false){}
    bool isRoot(){
        return (!par) || (par->left != this && par->right != this);
    }
    //スプレー木を反転させる
    void push(){
        if(!rev) return;
        rev = false;
        swap(left,right);
        if(left) left->rev = !(left->rev);
        if(right) right->rev = !(right->rev);
    }
};

class LinkCutTree{
private:
    //chとpを親子関係にする(lch:左の子かどうか)
    void connect(node* ch, node* p, bool lch){
        if(ch) ch->par = p;
        if(lch){
            p->left = ch;
        }else{
            p->right = ch;
        }
    }
    //zig, zig-zag, zig-zig の３種類の回転を表す
    void rotate(node* u){
        node *p = u->par, *gp  = p->par;
        bool root = p->isRoot(), lch = (u == p->left);
        connect(lch ? (u->right) : (u->left),p,lch);
        connect(p,u,!lch);
        if(root){
            if(u) u->par = gp;
        }else{
            connect(u,gp,!root && (p == gp->left));
        }
    }
    //uをスプレー木の根にする
    void splay(node* u){
        while(!(u->isRoot())){
            node* p = u->par;
            node* gp = p->par;
            if(!(p->isRoot())) gp->push();
            p->push(), u->push();
            if(!(p->isRoot())) rotate((u == p->left) == (p == gp->left) ? p : u);
            rotate(u);
        }
        u->push();
    }
    node* expose(node* u){
        node* last = nullptr;
        for(node* v = u; v; v=v->par){
            splay(v);
            v->left = last;
            last = v;
        }
        splay(u);
        return last;
    }
    //uを全体の根にする
    void evert(node* u){
        expose(u);
        u->rev = !(u->rev);
    }
    bool connected(node* u,node* v){
        expose(u), expose(v);
        return u->par;
    }
    void link(node* u,node* v){
        // u, v が同じ連結成分にないか
        // assert(!connected(u, v));
        evert(u);
        u->par = v;
    }
    void cut(node* u, node* v){
        evert(u);
        expose(v);
        // u, v の間に辺があるか
        // assert(v->right == u && !(u->left));
        v->right->par = nullptr, v->right = nullptr;
    }
    node* lca(node* u, node* v){
        // u,v が同じ連結成分内にあるか
        // assert(connected(u, v));
        expose(u);
        return expose(v);
    }

public:
    node** arr;
    LinkCutTree(int node_size){
        arr = new node* [node_size];
        for(int i = 0; i < node_size; i++){
            arr[i] = new node(i);
        }
    }
    // id1 と id2 が同じ木(連結成分)に属するか
    bool connected(int id1, int id2){ return connected(arr[id1],arr[id2]); }
    // 木(連結成分)の根 id1 の親を id2 にする
    void link(int id1, int id2){ link(arr[id1],arr[id2]); }
    // id1 と id2 の間の辺を削除する
    void cut(int id1, int id2){ cut(arr[id1], arr[id2]); }
    // id1 と id2 の LCA を求める
    int lca(int id1, int id2){ return lca(arr[id1],arr[id2])->id; }
};
