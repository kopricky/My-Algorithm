#include "../header.hpp"

//値の範囲が大きくかつクエリをオンラインで処理するときに用いる(動的にセグメント木のノードを追加する)
//Binary Trieと同等の構造
//POOL_SIZE および MAX_SIZE を問題に応じて変更
//pointUpdateQuery and RangeMinQuery の実装(O(log(MAX_VAL)))

template<typename T> class dynamic_segtree{
private:
    struct node {
        T val;
        node *par, *left, *right;
        node() : val(identity), par(nullptr), left(nullptr), right(nullptr){}
    };
    void opr1(T& arg1,T arg2){
        arg1 = arg2;
    }
    T opr2(T arg1,T arg2){
        return min(arg1,arg2);
    }
    static int nw_size;
    static const int POOL_SIZE = 2000000;
    static const int MAX_SIZE = 1000000001;
    static node *root;
    static node pool[POOL_SIZE];
    static const T identity = numeric_limits<T>::max();
    node *alloc(){
        assert(nw_size < POOL_SIZE);
        return (&pool[nw_size++]);
    }
public:
    dynamic_segtree() : nw_size(0){ root = alloc(); }
    // a 番目に x を追加(すでに a 番目に値が存在すればそれを x に更新)
    void insert(int a, T x, node *k = root, int l = 0, int r = MAX_SIZE){
        k->val = opr2(k->val,x);
        if(r - l == 1){
            return;
        }
        if(a < (l + r) / 2){
            if(!(k->left)){
                k->left = alloc();
                k->left->par = k;
            }
            insert(a, x, k->left, l, (l+r)/2);
        }else{
            if(!(k->right)){
                k->right = alloc();
                k->right->par = k;
            }
            insert(a, x, k->right, (l+r)/2, r);
        }
    }
    void update(int a, T x, node *k = root, int l = 0, int r = MAX_SIZE){
        if(r - l == 1){
            opr1(k->val,x);
            while(k->par){
                k = k->par;
                k->val = identity;
                if(k->left){
                    k->val = opr2(k->val,k->left->val);
                }
                if(k->right){
                    k->val = opr2(k->val,k->right->val);
                }
            }
            return;
        }
        if(a < (l + r) / 2){
            update(a, x, k->left, l, (l+r)/2);
        }else{
            update(a, x, k->right, (l+r)/2, r);
        }
    }
    T query(int a, int b, node *k = root, int l=0, int r = MAX_SIZE){
        if(b <= l || r <= a){
            return identity;
        }
        if(a <= l && r <= b){
            return k->val;
        }
        T vl = identity,vr = identity;
        if(k->left){
            vl = query(a, b, k->left, l, (l+r)/2);
        }
        if(k->right){
            vr = query(a, b, k->right, (l+r)/2, r);
        }
        return opr2(vl, vr);
    }
};
template<typename T> int dynamic_segtree<T>::nw_size = 0;
template<typename T> class dynamic_segtree<T>::node* dynamic_segtree<T>::root;
template<typename T> class dynamic_segtree<T>::node dynamic_segtree<T>::pool[POOL_SIZE];
