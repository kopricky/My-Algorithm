//merge可能なheap(大きい方がTop(meldの不等号で変更可))
template<typename T> class Heap {
public:
    struct node {
        node* l; node* r; T val;
        node(T t) : l(nullptr), r(nullptr), val(t){}
    };
    node* root;
    Heap() : root(nullptr){}
    node* meld(node* a, node* b){
        if(!a) return b; if(!b) return a;
        if(a->val < b->val) swap(a, b);
        a->r = meld(a->r,b); swap(a->l, a->r);
        return a;
    }
    void push(T val){ node* p = new node(val); root = meld(root, p); }
    T top(){ return root->val; }
    void pop(){ node* p = root; root = meld(root->r, root->l); delete p; }
};
