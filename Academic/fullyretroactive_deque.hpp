#include "header.hpp"

template<typename _Tp> class node {
public:
    const int key;
    int val, sum, pmin, pmax;
    _Tp data;
    node *left, *right, *par;
    node(const int _key, _Tp&& _data, int _val) noexcept
         : key(_key), val(_val), sum(val), pmin(min(val, 0)), pmax(max(val, 0)),
            data(move(_data)), left(nullptr), right(nullptr), par(nullptr){}
    inline bool isRoot() const { return !par; }
    void eval(){
        sum = pmin = pmax = 0;
        if(left) sum += left->sum, pmin = min(pmin, left->pmin), pmax = max(pmax, left->pmax);
        sum += val, pmin = min(pmin, sum), pmax = max(pmax, sum);
        if(right){
            pmin = min(pmin, sum + right->pmin), pmax = max(pmax, sum + right->pmax);
            sum += right->sum;
        }
    }
    void rotate(bool right_){
        node<_Tp> *p = par, *g = p->par;
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

template<typename _Tp>
node<_Tp>* splay(node<_Tp>* u) noexcept {
    if(!u) return nullptr;
    while(!(u->isRoot())){
        node<_Tp> *p = u->par, *gp = p->par;
        if(p->isRoot()){
            u->rotate((u == p->left));
        }else{
            bool flag = (u == p->left);
            if((u == p->left) == (p == gp->left)){
                p->rotate(flag), u->rotate(flag);
            }else{
                u->rotate(flag), u->rotate(!flag);
            }
        }
    }
    return u;
}

template<typename _Tp>
pair<node<_Tp>*, bool> get(const int _key, node<_Tp>* root){
    node<_Tp> *cur = nullptr, *nx = root, *res = nullptr;
    while(nx){
        cur = nx;
        if(cur->key <= _key) nx = cur->right, res = cur;
        else nx = cur->left;
    }
    node<_Tp>* tmp = splay(cur);
    return res ? make_pair(splay(res), true) : make_pair(tmp, false);
}

template<typename _Tp>
node<_Tp>* insert(node<_Tp>* ver, node<_Tp>* root) noexcept {
    if(!root) return ver;
    node<_Tp> *cur = nullptr, *nx = root;
    while(nx){
        cur = nx;
        if(cur->key > ver->key) nx = cur->left;
        else nx = cur->right;
    }
    if(cur->key > ver->key) cur->left = ver, ver->par = cur;
    else cur->right = ver, ver->par = cur;
    cur->eval();
    return splay(ver);
}

template<typename _Tp> class RetroactiveDeque {
private:
    node<_Tp> *lroot, *rroot;
    int left_pos(const int _time){
        pair<node<_Tp>*, bool> lres = get(_time, lroot);
        lroot = lres.first;
        int l = 0;
        if(lres.second) l = (lroot->left ? lroot->left->sum : 0) + lroot->val;
        return l;
    }
    int right_pos(const int _time){
        pair<node<_Tp>*, bool> rres = get(_time, rroot);
        rroot = rres.first;
        int r = 0;
        if(rres.second) r = (rroot->left ? rroot->left->sum : 0) + rroot->val;
        return r;
    }
    template<typename Data>
    void push_front_impl(Data&& data, const int _time){
        _Tp new_data = forward<Data>(data);
        node<_Tp> *new_node = new node<_Tp>(_time, move(new_data), 1);
        lroot = insert(new_node, lroot);
    }
    void pop_front_impl(const int _time){
        node<_Tp> *new_node = new node<_Tp>(_time, _Tp(), -1);
        lroot = insert(new_node, lroot);
    }
    template<typename Data>
    void push_back_impl(Data&& data, const int _time){
        _Tp new_data = forward<Data>(data);
        node<_Tp> *new_node = new node<_Tp>(_time, move(new_data), 1);
        rroot = insert(new_node, rroot);
    }
    void pop_back_impl(const int _time){
        node<_Tp> *new_node = new node<_Tp>(_time, _Tp(), -1);
        rroot = insert(new_node, rroot);
    }
    pair<node<_Tp>*, bool> find(int index, node<_Tp>* root){
        if(!root) return make_pair(nullptr, false);
        node<_Tp> *cur = root, *nx = root->left, *res = nullptr;
        if(!nx && index == 0) return make_pair(root, true);
        if(!nx || index < nx->pmin || nx->pmax < index) return make_pair(root, false);
        while(nx){
            cur = nx;
            int cur_sum = (cur->left ? cur->left->sum : 0) + cur->val;
            if(cur->right){
                if(cur_sum + cur->right->pmin <= index && index <= cur_sum + cur->right->pmax){
                    nx = cur->right, index -= cur_sum;
                    continue;
                }
            }
            if(cur_sum == index){
                res = cur;
                break;
            }else nx = cur->left;
        }
        node<_Tp> *tmp = splay(cur);
        if(!res) return make_pair(tmp, true);
        cur = splay(res), nx = cur->right;
        while(nx) cur = nx, nx = cur->left;
        return make_pair(splay(cur), true);
    }
    _Tp& query_impl(int index, const int _time, bool back){
        const int lpos = left_pos(_time), rpos = right_pos(_time);
        assert(index < lpos + rpos);
        if(back) index = lpos + rpos - 1;
        const int lid = lpos - index, rid = index + 1 - lpos;
        pair<node<_Tp>*, bool> lres = find(lid - 1, lroot);
        pair<node<_Tp>*, bool> rres = find(rid - 1, rroot);
        lroot = lres.first, rroot = rres.first;
        if(lres.second){
            if(rres.second) return (lroot->key < rroot->key) ? rroot->data : lroot->data;
            else return lroot->data;
        }else{
            return rroot->data;
        }
    }
public:
    RetroactiveDeque() : lroot(nullptr), rroot(nullptr){}
    size_t size(const int _time){ return left_pos(_time) + right_pos(_time); }
    _Tp& front(const int _time){ return query_impl(0, _time, false); }
    _Tp& back(const int _time){ return query_impl(0, _time, true); }
    void push_front(const _Tp& value, const int _time){ push_front_impl(value, _time); }
    void push_front(_Tp&& value, const int _time){ push_front_impl(move(value), _time); }
    void pop_front(const int _time){ pop_front_impl(_time); }
    void push_back(const _Tp& value, const int _time){ push_back_impl(value, _time); }
    void push_back(_Tp&& value, const int _time){ push_back_impl(move(value), _time); }
    void pop_back(const int _time){ pop_back_impl(_time); }
    _Tp& query(const size_t index, const int _time){ return query_impl(index, _time, false); }
};
