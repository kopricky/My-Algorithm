#include "../header.hpp"

// 単調でない傾きについての convex hull trick
// Li Chao Segment Tree を用いて O(nlog(MAX_A))
template<typename T> class CHT {
private:
    struct node {
        node *left, *right;
        T a, b;
        node() : left(nullptr), right(nullptr){}
        node(T arg1, T arg2)
            : left(nullptr), right(nullptr), a(arg1), b(arg2){}
        T f(T x, T a, T b){ return a * x + b; }
        void add(T l, T r, T _a, T _b) {
            if(f(l, _a, _b) < f(l, a, b)) {
                swap(a, _a), swap(b, _b);
            }
            if(f(r, a, b) <= f(r, _a, _b)) return;
            T mid = (l + r) / 2;
            if(f(mid, a, b) < f(mid, _a, _b)){
                if(!right){
                    right = new node(_a, _b);
                }else{
                    right->add(mid + 1, r, _a, _b);
                }
            }else{
                swap(a, _a), swap(b, _b);
                if(!left){
                    left = new node(_a,_b);
                }else{
                    left->add(l, mid, _a, _b);
                }
            }
        }
        T query(T l, T r, T k) {
            if(l == r) return f(k, a, b);
            T ans = f(k, a, b);
            T mid = (l + r) / 2;
            if(k <= mid && left){
                ans = min(ans, left->query(l, mid, k));
            }
            if(k > mid && right){
                ans = min(ans, right->query(mid + 1, r, k));
            }
            return ans;
        }
        void clear() {
            if(left) left->clear();
            if(right) right->clear();
            delete this;
        }
    };
    node *root;
    T min_val, max_val;
public:
    // arg1:クエリになげる最小の値, arg2:クエリになげる最大の値
    CHT(T arg1, T arg2)
        : min_val(arg1), max_val(arg2){ root = new node(0, numeric_limits<T>::max() / 10); }
    // f(x) = a * x + b を挿入
    void add(T a, T b) {
        return root->add(min_val, max_val, a, b);
    }
    // x = kでの最小値
    T query(T k) {
        return root->query(min_val, max_val, k);
    }
    ~CHT(){ root->clear(); }
};

