#include "../header.hpp"

// root ポインタは nullptr 初期化することを忘れない
template<typename KeyType, typename ValueType> class node {
public:
    using KT =  KeyType;
    using VT = ValueType;
    KT key;
    int sz;
    VT value, al, lazy;
    node *left, *right, *par;
    node(KT _key, VT _value) : key(_key), sz(1), value(_value), al(id2), lazy(id1),
        left(nullptr), right(nullptr), par(nullptr){}
    static const VT id1 = (VT)0;
    static const VT id2 = (VT)0;
    void opr1(VT& arg1, const VT arg2) const {
        arg1 += arg2;
    }
    VT opr2(const VT arg1, const VT arg2) const {
        return arg1 + arg2;
    }
    bool isRoot() const {
        return !par;
    }
    void push(){
        if(lazy != id1){
            opr1(value, lazy), al += lazy * sz;
            if(left) opr1(left->lazy, lazy);
            if(right) opr1(right->lazy, lazy);
            lazy = id1;
        }
    }
    void eval(){
        sz = 1, al = value;
        if(left) left->push(), sz += left->sz, al = opr2(left->al, al);
        if(right) right->push(), sz += right->sz, al = opr2(al, right->al);
    }
    void rotate(bool right_){
        node<KT, VT> *p = par, *g = p->par;
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

template<typename KT, typename VT>
static int size(node<KT, VT>* u){ return u ? u->sz : 0; }

// 頂点 u を木の根にする
template<typename KT, typename VT>
node<KT, VT>* splay(node<KT, VT>* u){
    if(!u) return nullptr;
    while(!(u->isRoot())){
        node<KT, VT> *p = u->par, *gp = p->par;
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

// root を根とする木の頂点で k 番目(0 indexed)に小さいキー値のものを探索する
// 返り値は (木の根, k 番目が存在するかしないか(0/1))
template<typename KT, typename VT>
pair<node<KT, VT>*, bool> get(int k, node<KT, VT>* root){
    if(size(root) < k+1) return make_pair(root, false);
    node<KT, VT> *cur = nullptr, *nx = root;
    while(true){
        cur = nx, cur->push();
        if(size(cur->left) < k){
            nx = cur->right, k -= size(cur->left) + 1;
        }else if(size(cur->left) > k){
            nx = cur->left;
        }else break;
    }
    return make_pair(splay(cur), true);
}

// root を根とする木の頂点でキーが _key であるものを探索する
// 返り値は (木の根, 存在するかしないか(0/1))
template<typename KT, typename VT>
pair<node<KT, VT>*, bool> find(const KT _key, node<KT, VT>* root){
    node<KT, VT> *cur = nullptr, *nx = root;
    while(nx){
        cur = nx, cur->push();
        if(cur->key == _key) break;
        if(cur->key > _key) nx = cur->left;
        else nx = cur->right;
    }
    return make_pair(splay(cur), cur && cur->key == _key);
}

// root を根とする木の頂点でキーが _key 以上で最小のものを返す(bool は _key 以上のものがあるかないか)
// _key 以上のものがなくても返り値のノードは nullptr とは限らないことに注意(_key 以上がなくても木の根は変わるので)
template<typename KT, typename VT>
pair<node<KT, VT>*, bool> lower_bound(const KT _key, node<KT, VT>* root){
    node<KT, VT> *cur = nullptr, *nx = root, *res = nullptr;
    while(nx){
        cur = nx, cur->push();
        if(cur->key >= _key){
            nx = cur->left;
            if(!res || cur->key <= res->key) res = cur;
        }else nx = cur->right;
    }
    node<KT, VT>* tmp = splay(cur);
    return res ? make_pair(splay(res), res) : make_pair(tmp, res);
}

// root を根とする木の頂点でキーが _key を超える最小のものを返す(bool は _key を超えるものがあるかないか)
// _key を超えるものがなくても返り値のノードは nullptr とは限らないことに注意(_key を超えるものがなくても木の根は変わるので)
template<typename KT, typename VT>
pair<node<KT, VT>*, bool> upper_bound(const KT _key, node<KT, VT>* root){
    node<KT, VT> *cur = nullptr, *nx = root, *res = nullptr;
    while(nx){
        cur = nx, cur->push();
        if(cur->key > _key){
            nx = cur->left;
            if(!res || cur->key <= res->key) res = cur;
        }else nx = cur->right;
    }
    node<KT, VT>* tmp = splay(cur);
    return res ? make_pair(splay(res), res) : make_pair(tmp, res);
}

// root1 を根とする木のすべての要素が root 2 を根とする木のすべての要素より小さいときの merge
template<typename KT, typename VT>
node<KT, VT>* join(node<KT, VT>* root1, node<KT, VT>* root2){
    if(!root1 || !root2) return root1 ? root1 : root2;
    node<KT, VT> *cur = nullptr, *nx = root1;
    while(nx){
        cur = nx, cur->push(), nx = cur->right;
    }
    node<KT, VT>* ver = splay(cur);
    ver->right = root2, ver->eval(), root2->par = ver;
    return ver;
}

// root を根とする木に ver を insert する
template<typename KT, typename VT>
node<KT, VT>* insert(node<KT, VT>* ver, node<KT, VT>* root){
    if(!root) return ver;
    node<KT, VT> *cur = nullptr, *nx = root;
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
template<typename KT, typename VT>
pair<node<KT, VT>*, bool> erase(const KT _key, node<KT, VT>* root){
    pair<node<KT, VT>*, bool> ver = find(_key, root);
    if(!ver.second) return ver;
    node<KT, VT> *l = ver.first->left, *r = ver.first->right;
    if(l) l->par = nullptr;
    if(r) r->par = nullptr;
    delete ver.first;
    node<KT, VT>* res = join(l, r);
    return make_pair(res, true);
}

// root1 を根とする木と root 2 を根とする木の merge
template<typename KT, typename VT>
node<KT, VT>* merge(node<KT, VT>* root1, node<KT, VT>* root2){
    if(!root2) return root1;
    return insert(root2, root1);
}

// root を根とする木を (_key 未満, _key 以上) の木に split する.
// lower_bound ⇒ get に変えるとキー値ベースではなく個数ベースで split できる.
template<typename KT, typename VT>
pair<node<KT, VT>*, node<KT, VT>*> split(const KT _key, node<KT, VT>* root){
    pair<node<KT, VT>*, bool> res = lower_bound(_key, root);
    if(!res.second) return make_pair(res.first, nullptr);
    node<KT, VT> *res1 = nullptr, *res2 = res.first;
    res1 = res2->left, res2->left = nullptr, res2->eval();
    if(res1) res1->par = nullptr;
    return make_pair(res1, res2);
}

// root を根とする木のうちキー値が [lvalue, rvalue) のものに対して x の更新を行う.
template<typename KT, typename VT>
node<KT, VT>* range(KT lvalue, KT rvalue, const VT x, node<KT, VT>* root){
    auto pnn1 = split(lvalue, root);
    auto pnn2 = split(rvalue, pnn1.second);
    if(pnn2.first) opr1(pnn2.first->lazy, x), pnn2.first->push();
    return join(pnn1.first, join(pnn2.first, pnn2.second));
}

// root を根とする木のうちキー値が [lvalue, rvalue) のものに対するクエリに答える.
// 返り値は(木の根, 値)
template<typename KT, typename VT>
pair<node<KT, VT>*, VT> query(KT lvalue, KT rvalue, node<KT, VT>* root){
    auto pnn1 = split(lvalue, root);
    auto pnn2 = split(rvalue, pnn1.second);
    VT res = (pnn2.first ? pnn2.first->al : node<KT, VT>::id2);
    return make_pair(join(pnn1.first, join(pnn2.first, pnn2.second)), res);
}
