#include "../../header.hpp"

//POOL_SIZE および MAX_SIZE を問題に応じて変更
//RAQ and RSQ の実装
template<typename T> class dynamic_segtree{
private:
    struct node {
    	T val, lazy;
        int st_size;
    	node *par, *left, *right;
    	node() : val(id2), lazy(id1), st_size(0),
            par(nullptr), left(nullptr), right(nullptr){}
    };
    void opr1(T& arg1,T arg2){
        arg1 += arg2;
    }
    T opr2(T arg1,T arg2){
        return arg1 + arg2;
    }
    static const int POOL_SIZE = 4000000;
    static const int MAX_SIZE = 100001;
    static node *root;
    static node pool[POOL_SIZE];
    static const T id1 = (T)0;
    static const T id2 = (T)0;
    int nw_size;
    node *alloc(){
        assert(nw_size < POOL_SIZE);
        return (&pool[nw_size++]);
    }
    void eval(node* k, int l, int r){
        if(k->lazy != id1){
            opr1(k->val,k->lazy);
            if(r-l>1){
                if(k->left){
                    opr1(k->left->lazy, k->lazy / k->st_size * k->left->st_size);
                }
                if(k->right){
                    opr1(k->right->lazy, k->lazy / k->st_size * k->right->st_size);
                }
            }
            k->lazy = id1;
        }
    }
public:
    dynamic_segtree(){ nw_size = 0; root = alloc(); }
    //a番目にxを追加
    void insert(int a, T x, node *k = root, int l = 0, int r = MAX_SIZE){
        eval(k, l, r);
        k->st_size++;
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
    void range(int a, int b, const T x, node *k = root, int l = 0, int r = MAX_SIZE){
        eval(k, l, r);
    	if(b <= l || r <= a) return;
    	if(a <= l && r <= b){
            opr1(k->lazy, k->st_size * x);
    		eval(k, l, r);
    		return;
    	}
        k->val = id2;
    	if(k->left){
            range(a, b, x, k->left, l, (l+r)/2);
            k->val = opr2(k->val,k->left->val);
    	}
        if(k->right){
            range(a, b, x, k->right, (l+r)/2, r);
            k->val = opr2(k->val,k->right->val);
        }
    }
    T query(int a, int b, node *k = root, int l=0, int r = MAX_SIZE){
        eval(k, l, r);
        if(b <= l || r <= a){
            return id2;
        }
        if(a <= l && r <= b){
            return k->val;
        }
        T vl = id2,vr = id2;
        if(k->left){
            vl = query(a, b, k->left, l, (l+r)/2);
        }
        if(k->right){
            vr = query(a, b, k->right, (l+r)/2, r);
        }
        return opr2(vl, vr);
    }
};
template<typename T> class dynamic_segtree<T>::node* dynamic_segtree<T>::root;
template<typename T> class dynamic_segtree<T>::node dynamic_segtree<T>::pool[POOL_SIZE];
