#include "../header.hpp"

//遅延処理(加算)を備えたmerge可能なheap(大きい方がTop(meldの不等号で変更可))
template<typename T> class LazyHeap {
public:
    struct node{
        node *l, *r;
        T val, add;
        node(T t) : l(nullptr), r(nullptr), val(t), add(0){}
    };
    node* root;
    LazyHeap() : root(nullptr){}
    void lazy(node* a){
        if(a->l) a->l->add += a->add;
        if(a->r) a->r->add += a->add;
        a->val += a->add;
        a->add = 0;
    }
    node* meld(node* a, node* b){
        if(!a) return b; if(!b) return a;
        if(a->val+a->add < b->val+b->add) swap(a, b);
        lazy(a);
        a->r = meld(a->r,b);
        swap(a->l, a->r);
        return a;
    }
    bool empty(){
        return !root;
    }
    void push(T val){
        node* p = new node(val);
        root = meld(root, p);
    }
    T top(){
        return root->val;
    }
    void pop(){
        node* p = root;
        lazy(p);
        root = meld(root->r, root->l);
        delete p;
    }
};
