#include "../header.hpp"

template<typename _Key, typename _Tp> class node {
public:
    int sz;
    _Key key;
    _Tp val, al, lazy;
    node *left, *right, *par;
    node(_Key _key, _Tp _val) noexcept : sz(1), key(_key), val(_val), al(val), lazy(id1),
        left(nullptr), right(nullptr), par(nullptr){}
    static const _Tp id1 = (_Tp)0;
    static const _Tp id2 = (_Tp)0;
    static void opr1(_Tp& arg1, const _Tp arg2){ arg1 += arg2; }
    static _Tp opr2(const _Tp arg1, const _Tp arg2){ return arg1 + arg2; }
    inline bool isRoot() const { return !par; }
    void push(){
        if(lazy != id1){
            opr1(val, lazy), al += lazy * sz;
            if(left) opr1(left->lazy, lazy);
            if(right) opr1(right->lazy, lazy);
            lazy = id1;
        }
    }
    void eval(){
        sz = 1, al = val;
        if(left) left->push(), sz += left->sz, al = opr2(left->al, al);
        if(right) right->push(), sz += right->sz, al = opr2(al, right->al);
    }
    void rotate(bool right_){
        node<_Key, _Tp> *p = par, *g = p->par;
        if(right_){
            if((p->left = right)) right->par = p;
            right = p, p->par = this;
        }else{
            if((p->right = left)) left->par = p;
            left = p, p->par = this;
        }
        p->eval(), eval();
        if(!(par = g)) return;
        if(g->left == p) g->left = this;
        if(g->right == p) g->right = this;
        g->eval();
    }
};

template<typename _Key, typename _Tp>
int size(node<_Key, _Tp>* u){ return u ? u->sz : 0; }

// 頂点 u を木の根にする
template<typename _Key, typename _Tp>
node<_Key, _Tp>* splay(node<_Key, _Tp>* u) noexcept {
    if(!u) return nullptr;
    while(!(u->isRoot())){
        node<_Key, _Tp> *p = u->par, *gp = p->par;
        if(p->isRoot()){ // zig
            p->push(), u->push();
            u->rotate((u == p->left));
        }else{
            gp->push(), p->push(), u->push();
            bool flag = (u == p->left);
            if((u == p->left) == (p == gp->left)){ // zig-zig
                p->rotate(flag), u->rotate(flag);
            }else{ // zig-zag
                u->rotate(flag), u->rotate(!flag);
            }
        }
    }
    u->push();
    return u;
}

// root を根とする木の頂点で k 番目に小さいキー値のものを探索する
// 返り値は (木の根, k 番目が存在するかしないか(0/1))
template<typename _Key, typename _Tp>
pair<node<_Key, _Tp>*, bool> get(const int k, node<_Key, _Tp>* root) noexcept {
    if(size(root) < k) return make_pair(root, false);
    int sum = 0;
    node<_Key, _Tp> *cur = nullptr, *nx = root;
    while(nx){
        cur = nx, cur->push();
        if(size(cur->left) < sum - 1){
            nx = cur->right, sum -= size(cur->left) + 1;
        }else if(size(cur->left) >= sum){
            nx = cur->left;
        }else{
            return make_pair(splay(cur), true);
        }
    }
}

// root を根とする木の頂点でキーが _key であるものを探索する
// 返り値は (木の根, 存在するかしないか(0/1))
template<typename _Key, typename _Tp>
pair<node<_Key, _Tp>*, bool> find(const _Key _key, node<_Key, _Tp>* root) noexcept {
    node<_Key, _Tp> *cur = nullptr, *nx = root;
    while(nx){
        cur = nx, cur->push();
        if(cur->key == _key) return make_pair(splay(cur), true);
        if(cur->key > _key) nx = cur->left;
        else nx = cur->right;
    }
    return make_pair(splay(cur), false);
}

// root を根とする木の頂点でキーが _key 以上で最小のものを返す(bool は _key 以上のものがあるかないか)
// _key 以上のものがなくても返り値のノードは nullptr とは限らないことに注意(_key 以上がなくても木の根は変わるので)
template<typename _Key, typename _Tp>
pair<node<_Key, _Tp>*, bool> lower_bound(const _Key _key, node<_Key, _Tp>* root) noexcept {
    node<_Key, _Tp> *cur = nullptr, *nx = root, *res = nullptr;
    while(nx){
        cur = nx, cur->push();
        if(cur->key >= _key){
            nx = cur->left;
            if(!res || cur->key <= res->key) res = cur;
        }else nx = cur->right;
    }
    node<_Key, _Tp>* tmp = splay(cur);
    return res ? make_pair(splay(res), true) : make_pair(tmp, false);
}

// root を根とする木の頂点でキーが _key を超える最小のものを返す(bool は _key を超えるものがあるかないか)
// _key を超えるものがなくても返り値のノードは nullptr とは限らないことに注意(_key を超えるものがなくても木の根は変わるので)
template<typename _Key, typename _Tp>
pair<node<_Key, _Tp>*, bool> upper_bound(const _Key _key, node<_Key, _Tp>* root) noexcept {
    node<_Key, _Tp> *cur = nullptr, *nx = root, *res = nullptr;
    while(nx){
        cur = nx, cur->push();
        if(cur->key > _key){
            nx = cur->left;
            if(!res || cur->key <= res->key) res = cur;
        }else nx = cur->right;
    }
    node<_Key, _Tp>* tmp = splay(cur);
    return res ? make_pair(splay(res), true) : make_pair(tmp, false);
}

// root1 を根とする木のすべての要素が root 2 を根とする木のすべての要素より小さいときの merge
template<typename _Key, typename _Tp>
node<_Key, _Tp>* join(node<_Key, _Tp>* root1, node<_Key, _Tp>* root2) noexcept {
    if(!root1 || !root2) return root1 ? root1 : root2;
    node<_Key, _Tp> *cur = nullptr, *nx = root1;
    while(nx) cur = nx, cur->push(), nx = cur->right;
    node<_Key, _Tp>* ver = splay(cur);
    ver->right = root2, ver->eval(), root2->par = ver;
    return ver;
}

// root を根とする木に ver を insert する
template<typename _Key, typename _Tp>
node<_Key, _Tp>* insert(node<_Key, _Tp>* ver, node<_Key, _Tp>* root) noexcept {
    if(!root) return ver;
    node<_Key, _Tp> *cur = nullptr, *nx = root;
    while(nx){
        cur = nx, cur->push();
        if(cur->key > ver->key) nx = cur->left;
        else nx = cur->right;
    }
    if(cur->key > ver->key) cur->left = ver, ver->par = cur;
    else cur->right = ver, ver->par = cur;
    cur->eval();
    return splay(ver);
}

// root を根とする木から _key をキーとする要素を erase する(同一キーの要素が複数ある場合はどれかひとつだけ消す)
// _key をキーとする要素がない場合は第 2 引数として false を返す.
template<typename _Key, typename _Tp>
pair<node<_Key, _Tp>*, bool> erase(const _Key _key, node<_Key, _Tp>* root){
    pair<node<_Key, _Tp>*, bool> ver = find(_key, root);
    assert(ver.second);
    node<_Key, _Tp> *l = ver.first->left, *r = ver.first->right;
    if(l) l->par = nullptr;
    if(r) r->par = nullptr;
    delete ver.first;
    return make_pair(join(l, r), true);
}

// root を根とする木を (_key 未満, _key 以上) の木に split する.
// lower_bound ⇒ get に変えるとキー値ベースではなく個数ベースで split できる.
template<typename _Key, typename _Tp>
pair<node<_Key, _Tp>*, node<_Key, _Tp>*> split_key(const _Key _key, node<_Key, _Tp>* root) noexcept {
    pair<node<_Key, _Tp>*, bool> res = lower_bound(_key, root);
    if(!res.second) return make_pair(res.first, nullptr);
    node<_Key, _Tp> *res1 = nullptr, *res2 = res.first;
    res1 = res2->left, res2->left = nullptr, res2->eval();
    if(res1) res1->par = nullptr;
    return make_pair(res1, res2);
}

// root を根とする木のうちキー値が [lkey, rkey) のものに対して x の更新を行う.
template<typename _Key, typename _Tp>
node<_Key, _Tp>* range(_Key lkey, _Key rkey, _Tp x, node<_Key, _Tp>* root) noexcept {
    auto pnn1 = split_key(lkey, root);
    auto pnn2 = split_key(rkey, pnn1.second);
    if(pnn2.first) node<_Key, _Tp>::opr1(pnn2.first->lazy, x);
    return join(pnn1.first, join(pnn2.first, pnn2.second));
}

// root を根とする木のうちキー値が [lkey, rkey) のものに対するクエリに答える.
// 返り値は(木の根, 値)
template<typename _Key, typename _Tp>
pair<node<_Key, _Tp>*, _Tp> query(_Key lkey, _Key rkey, node<_Key, _Tp>* root) noexcept {
    auto pnn1 = split_key(lkey, root);
    auto pnn2 = split_key(rkey, pnn1.second);
    _Tp res = (pnn2.first ? pnn2.first->al : node<_Key, _Tp>::id2);
    return make_pair(join(pnn1.first, join(pnn2.first, pnn2.second)), res);
}
