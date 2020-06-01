#include "../header.hpp"

// RAQ and RSQ の場合
template<typename _Tp> class RBSTNode {
public:
    unsigned sz;
    _Tp val, lazy, al;
    RBSTNode* left, *right;
    RBSTNode(){};
    RBSTNode(_Tp v) : sz(1), val(v), lazy(id1), al(val), left(nullptr), right(nullptr){}
    static const _Tp id1 = (_Tp)0;
    static const _Tp id2 = (_Tp)0;
    static void opr1(_Tp& arg1, const _Tp arg2){ arg1 = arg1 + arg2; }
    static _Tp opr2(const _Tp arg1, const _Tp arg2){ return arg1 + arg2; }
    void push(){
        if(lazy == id1) return;
        opr1(val, lazy), al += lazy * sz;
        if(left) opr1(left->lazy, lazy);
        if(right) opr1(right->lazy, lazy);
        lazy = id1;
    }
    void eval(){
        sz = 1, al = val;
        if(left) left->push(), sz += left->sz, al = opr2(left->al, al);
        if(right) right->push(), sz += right->sz, al = opr2(al, right->al);
    }
    static unsigned rnd(){
        static unsigned x = 123456789, y = 362436069, z = 521288629, w = 86675123;
        unsigned t = (x^(x<<11));
        x = y, y = z, z = w;
        return (w = (w^(w>>19))^(t^(t>>8)));
    }
};

template<typename _Tp>
inline unsigned size(RBSTNode<_Tp> *t){ return t ? t->sz : 0u; }

template<typename _Tp>
RBSTNode<_Tp> *join(RBSTNode<_Tp> *l, RBSTNode<_Tp> *r){
    if(!l) return r;
    if(!r) return l;
    l->push(), r->push();
    if(RBSTNode<_Tp>::rnd() % (l->sz + r->sz) < l->sz){
        return l->right = join(l->right, r), l->eval(), l;
    }else{
        return r->left = join(l, r->left), r->eval(), r;
    }
}

template<typename _Tp>
pair<RBSTNode<_Tp>*, RBSTNode<_Tp>*> split(RBSTNode<_Tp> *t, unsigned k){
    if(!t) return make_pair(nullptr, nullptr);
    t->push();
    if(!k) return make_pair(nullptr, t);
    if(k <= size(t->left)){
        pair<RBSTNode<_Tp>*, RBSTNode<_Tp>*> s = split(t->left, k);
        return t->left = s.second, t->eval(), make_pair(s.first, t);
    }else{
        pair<RBSTNode<_Tp>*, RBSTNode<_Tp>*> s = split(t->right, k-size(t->left)-1);
        return t->right = s.first, t->eval(), make_pair(t, s.second);
    }
}

template<typename _Tp>
RBSTNode<_Tp>* insert(RBSTNode<_Tp> *root, RBSTNode<_Tp>* u, const unsigned k){
    auto s = split(root, k);
    return join(join(s.first, u), s.second);
}

template<typename _Tp>
RBSTNode<_Tp>* erase(RBSTNode<_Tp> *root, unsigned k){
    auto sl = split(root, k);
    auto sr = split(sl.second, 1);
    delete sr.first;
    return join(sl.first, sr.second);
}

template<typename _Tp>
pair<RBSTNode<_Tp>*, _Tp> get(RBSTNode<_Tp> *root, unsigned k){
    auto sr = split(root, k+1);
    auto sl = split(sr.first, k);
    auto lr = sl.second;
    _Tp res = lr->val;
    return make_pair(join(join(sl.first, lr), sr.second), res);
}

template<typename _Tp>
RBSTNode<_Tp> *range(RBSTNode<_Tp> *root, const unsigned l, const unsigned r, const _Tp x){
    if(l >= r) return nullptr;
    auto sl = split(root, l);
    auto sr = split(sl.second, r-l);
    RBSTNode<_Tp>::opr1(sr.first->lazy, x);
    return join(join(sl.first, sr.first), sr.second);
}

template<typename _Tp>
pair<RBSTNode<_Tp>*, _Tp> query(RBSTNode<_Tp> *root, const unsigned l, const unsigned r){
    if(l >= r) return make_pair(nullptr, (_Tp)RBSTNode<_Tp>::id2);
    auto sl = split(root, l);
    auto sr = split(sl.second, r-l);
    _Tp res = sr.first->al;
    return make_pair(join(join(sl.first, sr.first), sr.second), res);
}

// ヘルパー関数
template<typename _Tp>
RBSTNode<_Tp> *bst_build(vector<RBSTNode<_Tp>*>& vec){
    RBSTNode<_Tp> *root = vec[0];
    int i, n = (int)vec.size(), st = 2, isolate = ((n % 4 == 1) ? (n-1) : -1);
    while(st <= n){
        root = vec[st-1];
        for(i = st-1; i < n; i += 2*st){
            vec[i]->left = vec[i-st/2];
            if(i+st/2 < n) vec[i]->right = vec[i+st/2];
            else if(isolate >= 0) vec[i]->right = vec[isolate];
            vec[i]->eval();
        }
        isolate = ((n % (4*st) >= st && (n % (4*st) < 2*st)) ? (i-2*st): isolate);
        st <<= 1;
    }
    return root;
}
