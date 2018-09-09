#include "../header.hpp"

// RAQ and RSQ の場合
template<typename T> class RBST {
public:
    struct node{
        T val, lazy, al;
        node* left; node* right;
        int st_size;   // 部分木のサイズ
        bool lazy_flag;
        node(){};
        node(T v) : val(v), lazy((T)0), al(v), left(nullptr), right(nullptr), st_size(1), lazy_flag(false){}
        ~node() { delete left; delete right; }
    };
    using pnn = pair<node*,node*>;
    node* root;
    T id1,id2;  //opr1,opr2の単位元
    //クエリに答える演算(sumに対応)
    inline T opr1(T l, T r) {
    	return l + r;
    }
    //遅延処理の演算(addに対応)
    inline T opr2(T& l, T r) {
    	return l = l + r;
    }
    RBST() : root(nullptr), id1(0), id2(0){}
    RBST(vector<T>& vec) : root(nullptr), id1(0), id2(0){
    	for(int i = 0; i < (int)vec.size(); i++){
    		insert(i,vec[i]);
    	}
    }
    inline int size(node* t) { return t ? t->st_size : 0; }
    inline T que(node* t) { push(t); return t ? t->al : id1; }
    //遅延伝播
    inline node* push(node* t){
        if(!t) return t;
        if(t->lazy_flag){
            if(t->left){
                t->left->lazy_flag = true;
                opr2(t->left->lazy, t->lazy);
            }
            if(t->right){
                t->right->lazy_flag = true;
                opr2(t->right->lazy, t->lazy);
            }
            opr2(t->val, t->lazy);
            t->al += t->lazy;
            t->lazy = id2;
            t->lazy_flag = false;
        }
        return t;
    }
    inline node* update(node *t){
        push(t);
        node* l = t->left; node* r = t->right;
        t->st_size = size(l) + size(r) + 1;
        t->al = opr1(opr1(que(l),que(r)),t->val);
        return t;
    }
    inline unsigned rnd(){
        static unsigned x = 123456789, y = 362436069, z = 521288629, w = 86675123;
        unsigned t = (x^(x<<11));
        x = y,y = z,z = w;
        return (w = (w^(w>>19))^(t^(t>>8)));
    }
    inline node* merge(node* l, node* r){
        if (!l || !r) return (!l) ? r : l;
        push(l),push(r);
        if(rnd() % (size(l) + size(r)) < (unsigned)size(l)){
            l->right = merge(l->right, r);
            return update(l);
        }else{
            r->left = merge(l, r->left);
            return update(r);
        }
    }
    pnn split(node* t, int k){   //木のサイズが(k,n-k)となるように分割する
        if(!t) return pnn(nullptr, nullptr);
        push(t);
        if(k <= size(t->left)){
            pnn s = split(t->left, k);
            t->left = s.second;
            return pnn(s.first,update(t));
        }else{
            pnn s = split(t->right, k-size(t->left)-1);
            t->right = s.first;
            return pnn(update(t), s.second);
        }
    }
    inline node* insert(node* t, int k, node* u){
        auto s = split(t, k);
        return merge(merge(s.first, u), s.second);
    }
    //0_indexedでkにinsert
    inline void insert(int k, T val){
        root = insert(root, k, new node(val));
    }
    inline node* erase(node* t, int k){
        auto sl = split(t, k);
        auto sr = split(sl.second, 1);
        delete sr.first;
        return merge(sl.first, sr.second);
    }
    //0_indexedでkをerase
    inline void erase(int k){
        root = erase(root, k);
    }
    inline T get(int k){
        auto sr = split(root, k+1);
        auto sl = split(sr.first, k);
        auto lr = sl.second;
        T res = lr->val;
        root = merge(merge(sl.first, lr), sr.second);
        return res;
    }
    inline void range(int l, int r, T val){
        if(l >= r)  return;
        auto sr = split(root, r);
        auto sl = split(sr.first, l);
        auto lr = sl.second;
        lr->lazy = val;
        lr->lazy_flag = true;
        root = merge(merge(sl.first,lr),sr.second);
    }
    inline T query(int l,int r){
        if(l >= r)  return 0;
        auto sr = split(root,r);
        auto sl = split(sr.first,l);
        auto lr = sl.second;
        T res = que(lr);
        root = merge(merge(sl.first, lr), sr.second);
        return res;
    }
};
