#include "../header.hpp"

template<typename T> class CHT {
private:
    struct node {
        node *left, *right;
        T a, b;
        node(const T arg1, const T arg2)
            : left(nullptr), right(nullptr), a(arg1), b(arg2){}
        T f(const T x) const { return a * x + b; }
    };
    void swap(node *x, node *y){
        std::swap(x->a, y->a), std::swap(x->b, y->b);
    }
    void add(node *cur, node *nw, const T l, const T r){
        if(nw->f(l) < cur->f(l)) swap(cur, nw);
        if(cur->f(r - 1) <= nw->f(r - 1)) return;
        const T mid = (l + r) / 2;
        if(cur->f(mid) <= nw->f(mid)){
            if(!cur->right){
                cur->right = new node(nw->a, nw->b);
            }else{
                add(cur->right, nw, mid, r);
            }
        }else{
            swap(cur, nw);
            if(!cur->left){
                cur->left = new node(nw->a, nw->b);
            }else{
                add(cur->left, nw, l, mid);
            }
        }
    }
    T query(node *cur, T k, T l, T r){
        if(r - l == 1) return cur->f(k);
        T ans = cur->f(k);
        const T mid = (l + r) / 2;
        if(k < mid && cur->left){
            ans = min(ans, query(cur->left, k, l, mid));
        }
        if(mid <= k && cur->right){
            ans = min(ans, query(cur->right, k, mid, r));
        }
        return ans;
    }
    void clear(node *cur){
        if(cur->left) clear(cur->left);
        if(cur->right) clear(cur->right);
        delete cur;
    }
    const T lpos, rpos;
    node *root;
public:
    CHT(const T _lpos, const T _rpos)
        : lpos(_lpos), rpos(_rpos), root(new node(0, numeric_limits<T>::max() / 2)){
        assert(lpos < rpos);
    }
    // ~CHT(){ clear(root); }
    // f(x) = a * x + b を挿入
    void add_line(const T a, const T b){
        node *nw = new node(a, b);
        return add(root, nw, lpos, rpos);
    }
    // f(x) = a * x + b を [c, d) の範囲に挿入
    void add_line(const T a, const T b, const T c, const T d){
        assert(c < d);
        node *nw = new node(a, b);
        return add(root, nw, c, d);
    }
    // x = k での最小値
    T query(const T k){
        return query(root, k, lpos, rpos);
    }
};

