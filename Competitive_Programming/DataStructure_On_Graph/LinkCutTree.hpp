#include "../header.hpp"

//根はそれぞれのパスの根と全体の木の根の２つの意味がある
//親はsplay木の中の親とパスの親の２つの意味がある
struct node {
    node *left, *right, *par;
    bool rev;
    node() : left(nullptr), right(nullptr), par(nullptr), rev(false){}
    bool isRoot(){
        return (!par) || (par->left != this && par->right != this);
    }
    //スプレー木を反転させる
    void push(){
        if(rev){
            rev = false;
            swap(left,right);
            if(left) left->rev = !(left->rev);
            if(right) right->rev = !(right->rev);
        }
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
        if(u == v) return true;
        expose(u), expose(v);
        return u->par;
    }
    void link(node* u,node* v){
        //already connected
        if(connected(u, v)) return;
        evert(u);
        u->par = v;
    }
    void cut(node* u,node* v){
        evert(u);
        expose(v);
        //no edge
        if(v->right != u || u->left || u->right) return;
        v->right->par = nullptr, v->right = nullptr;
    }
public:
    node** arr;
    LinkCutTree(){}
    void build(int node_size){
        arr = new node* [node_size];
        for(int i = 0; i < node_size; i++){
            arr[i] = new node();
        }
    }
    bool connected(int id1, int id2){ return connected(arr[id1],arr[id2]); }
    void link(int id1, int id2){ link(arr[id1],arr[id2]); }
    void cut(int id1,int id2){ cut(arr[id1],arr[id2]); }
};
