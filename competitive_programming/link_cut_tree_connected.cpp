struct node {
    node* left; node* right; node* par;
    bool rev;
    node(int t) : left(nullptr), right(nullptr), par(nullptr), rev(false){}
    bool isRoot(){
        return (!par) || (par->left != this && par->right != this);
    }
    void push(){
        if(rev){
            rev = false;
            swap(left,right);
            if(left){
                left->rev = !(left->rev);
            }
            if(right){
                right->rev = !(right->rev);
            }
        }
    }
};

class LinkCutTree{
private:
    void connect(node* ch, node* p, bool lch){
        if(ch){
            ch->par = p;
        }
        if(lch){
            p->left = ch;
        }else{
            p->right = ch;
        }
    }
    void rotate(node* u){
        node* p = u->par;
        node* gp  = p->par;
        bool isRootP = p->isRoot();
        bool lch = (u == p->left);
        connect(lch ? (u->right) : (u->left),p,lch);
        connect(p,u,!lch);
        if(isRootP){
            u->par = gp;
        }else{
            connect(u,gp,(p == gp->left));
        }
    }
    void splay(node* u){
        while(!(u->isRoot())){
            node* p = u->par;
            node* gp = p->par;
            if(!(p->isRoot())){
                gp->push();
            }
            p->push();
            u->push();
            if(!(p->isRoot())){
                rotate((u == p->left) == (p == gp->left) ? p : u);
            }
            rotate(u);
        }
        u->push();
    }
    node* expose(node* u){
        node* last = nullptr;
        for(node* v = u; v; v=v->par){
            splay(v);
            v->left = last;
            v->size += getSize(last);
            last = v;
        }
        splay(u);
        return last;
    }
    void evert(node* u){
        expose(u);
        u->rev = !(u->rev);
    }
public:
    node** arr;
    LinkCutTree(){}
    void build(int node_size){
        arr = new node* [node_size];
        for(int i = 0; i < node_size; i++){
            arr[i] = new node;
        }
    }
    bool connected(node* u,node* v){
        if(u == v){
            return true;
        }
        expose(u);
        expose(v);
        return u->par;
    }
    bool connected(int unum,int vnum){ return connected(arr[unum],arr[vnum]); }
    void link(node* u,node* v){
        if(connected(u,v))  return;
        evert(u);
        u->par = v;
    }
    void link(int unum,int vnum){ link(arr[unum],arr[vnum]); }
    void cut(node* u,node* v){
        evert(u);
        expose(v);
        v->right->par = nullptr;
        v->right = nullptr;
    }
    void cut(int unum,int vnum){ cut(arr[unum],arr[vnum]); }
};
