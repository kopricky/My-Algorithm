#include "../header.hpp"

template<typename T> class CHT {
private:
    struct node {
        node *left, *right;
        static const T inf = numeric_limits<T>::max();
        T a, b;
        node() : node(0, inf){}
        node(const T _a, const T _b)
            : left(nullptr), right(nullptr), a(_a), b(_b){}
        T f(const T x) const { return a * x + b; }
    };
    static void swap(node *x, node *y){
        std::swap(x->a, y->a), std::swap(x->b, y->b);
    }
    void _add_line(node *cur, node *nw, T l, T r){
        while(true){
            if(nw->f(l) < cur->f(l)) swap(cur, nw);
            if(cur->f(r - 1) <= nw->f(r - 1)) break;
            const T mid = (l + r) / 2;
            if(cur->f(mid) <= nw->f(mid)){
                if(!cur->right){
                    cur->right = new node(*nw);
                    break;
                }else{
                    cur = cur->right, l = mid;
                }
            }else{
                swap(cur, nw);
                if(!cur->left){
                    cur->left = new node(*nw);
                    break;
                }else{
                    cur = cur->left, r = mid;
                }
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
    CHT(const T _lpos, const T _rpos) : lpos(_lpos), rpos(_rpos), root(new node()){
        assert(lpos < rpos);
    }
    // ~CHT(){ clear(root); }
    // f(x) = a * x + b を挿入
    void add_line(const T a, const T b){
        node nw(a, b);
        return _add_line(root, &nw, lpos, rpos);
    }
    // x = k での最小値
    T query(const T k) const {
        return query(root, k, lpos, rpos);
    }
};
