#include "../header.hpp"

//merge可能なheap(大きい方がTop(meldの不等号で変更可))
template<typename T> class node {
public:
    node<T>* l; node<T>* r; T val;
    node(T t) : l(nullptr), r(nullptr), val(t){}
    friend node* meld(node* a, node* b){
        if(!a) return b;
        if(!b) return a;
        if(a->val < b->val) swap(a, b);
        a->l = meld(a->l,b);
        swap(a->l, a->r);
        return a;
    }
};

template<typename T> class Heap {
public:
    node<T>* root;
    int sz;
    Heap() : root(nullptr), sz(0){}
    friend Heap<T>* meld(Heap<T>* hp1, Heap<T>* hp2){
        Heap<T>* res = new Heap<T>();
        res->sz = hp1->sz + hp2->sz;
        res->root = meld(hp1->root, hp2->root);
        return res;
    }
    bool empty(){
        return !root;
    }
    int size(){
        return sz;
    }
    void push(T val){
        sz++;
        node<T>* p = new node<T>(val);
        root = meld(root, p);
    }
    T top(){
        return root->val;
    }
    void pop(){
        sz--;
        node<T>* p = root;
        root = meld(root->r, root->l);
        delete p;
    }
};
