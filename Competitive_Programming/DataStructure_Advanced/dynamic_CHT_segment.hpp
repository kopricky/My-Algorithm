#include <../header.hpp>

template<typename T> class CHT {
private:
    struct node {
        node *left, *right;
        static const T inf = numeric_limits<T>::max();
        T a, b, l, r;
        node(const T _a, const T _b, const T _l, const T _r)
            : left(nullptr), right(nullptr), a(_a), b(_b), l(_l), r(_r){}
        T f(const T x) const {
            return (l <= x && x < r) ? (a * x + b) : inf;
        }
        T tf(const T x) const {
            return a * x + b;
        }
        bool isLine(const T _l, const T _r) const {
            return (l == _l) && (r == _r);
        }
    };
    static void swap(node *x, node *y){
        std::swap(x->a, y->a), std::swap(x->b, y->b), std::swap(x->l, y->l), std::swap(x->r, y->r);
    }
    void _add_segment(node *cur, node *nw, T l, T r){
        while(true){
            const T mid = (l + r) / 2;
            if(cur->f(nw->l) <= nw->tf(nw->l) && cur->f(nw->r - 1) <= nw->tf(nw->r - 1)){
                break;
            }
            if(cur->tf(cur->l) >= nw->f(cur->l) && cur->tf(cur->r - 1) >= nw->f(cur->r - 1)){
                swap(cur, nw);
                break;
            }
            if(cur->isLine(l, r) && nw->isLine(l, r)){
                if(nw->tf(l) < cur->tf(l)) swap(cur, nw);
                if(cur->tf(mid) <= nw->tf(mid)){
                    if(!cur->right){
                        cur->right = new node(nw->a, nw->b, mid, r);
                        break;
                    }else{
                        cur = cur->right, l = mid, nw->l = mid;
                    }
                }else{
                    swap(cur, nw);
                    if(!cur->left){
                        cur->left = new node(nw->a, nw->b, l, mid);
                        break;
                    }else{
                        cur = cur->left, r = mid, nw->r = mid;
                    }
                }
                continue;
            }
            if(nw->isLine(l, r)) swap(cur, nw);
            if(nw->r <= mid){
                if(!cur->left){
                    cur->left = new node(*nw);
                    break;
                }else{
                    cur = cur->left, r = mid;
                }
            }else if(mid <= nw->l){
                if(!cur->right){
                    cur->right = new node(*nw);
                    break;
                }else{
                    cur = cur->right, l = mid;
                }
            }else{
                node _nw(nw->a, nw->b, mid, nw->r);
                nw->r = mid;
                if(!cur->left){
                    cur->left = new node(*nw);
                }else{
                    _add_segment(cur->left, nw, l, mid);
                }
                if(!cur->right){
                    cur->right = new node(_nw);
                }else{
                    _add_segment(cur->right, &_nw, mid, r);
                }
                break;
            }
        }
    }
    T query(node *cur, const T k, T l, T r) const {
        T ans = numeric_limits<T>::max();
        while(cur){
            ans = min(ans, cur->f(k));
            const T mid = (l + r) / 2;
            if(k < mid){
                cur = cur->left, r = mid;
            }else{
                cur = cur->right, l = mid;
            }
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
        : lpos(_lpos), rpos(_rpos),
            root(new node(0, numeric_limits<T>::max(), lpos, rpos)){
        assert(lpos < rpos);
    }
    // ~CHT(){ clear(root); }
    // f(x) = a * x + b を挿入
    void add_line(const T a, const T b){
        node nw(a, b, lpos, rpos);
        return _add_line(root, &nw, lpos, rpos);
    }
    // f(x) = a * x + b (x ∈ [l, r)) を挿入
    void add_segment(const T a, const T b, const T l, const T r){
        assert(l < r);
        node nw(a, b, l, r);
        return _add_segment(root, &nw, lpos, rpos);
    }
    // x = k での最小値
    T query(const T k) const {
        return query(root, k, lpos, rpos);
    }
};