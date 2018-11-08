#include "../header.hpp"

// RUQ(一点) and RSQ の場合

template<typename T> class RBST {
private:
    struct node{
        T val, al;
        node* left; node* right;
        int st_size;   // 部分木のサイズ
        node(){};
        node(T v) : val(v), al(v), left(nullptr), right(nullptr), st_size(1){}
        ~node() { delete left; delete right; }
    };
    using pnn = pair<node*,node*>;
    node* root;
    T id1;  //opr1の単位元
    //クエリに答える演算(sumに対応)
    inline T opr1(T l, T r) {
    	return l + r;
    }
    int size(node* t) { return t ? t->st_size : 0; }
    int que(node* t) { return t ? t->al : id1; }
    node* update(node *t){
        node* l = t->left; node* r = t->right;
        t->st_size = size(l) + size(r) + 1;
        t->al = opr1(opr1(que(l),que(r)),t->val);
        return t;
    }
    unsigned rnd(){
        static unsigned x = 123456789, y = 362436069, z = 521288629, w = 86675123;
        unsigned t = (x^(x<<11));
        x = y,y = z,z = w;
        return (w = (w^(w>>19))^(t^(t>>8)));
    }
    node* merge(node* l, node* r){
        if (!l || !r) return (!l) ? r : l;
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
    node* insert(node* t, int k, node* u){
        auto s = split(t, k);
        return merge(merge(s.first, u), s.second);
    }
    node* erase(node* t, int k){
        auto sl = split(t, k);
        auto sr = split(sl.second, 1);
        delete sr.first;
        return merge(sl.first, sr.second);
    }

public:
    RBST() : root(nullptr), id1(0){}
    RBST(vector<T>& vec) : root(nullptr), id1(0){
        for(int i = 0; i < (int)vec.size(); i++){
    		insert(i,vec[i]x  );
    	}
    }
    // 0_indexedでkにinsert
    void insert(int k, T val){
        root = insert(root, k, new node(val));
    }
    // 0_indexedでkをerase
    void erase(int k){
        root = erase(root, k);
    }
    // k番目の要素を参照
    T get(int k){
        auto sr = split(root, k+1);
        auto sl = split(sr.first, k);
        T res = sl.second->val;
        root = merge(merge(sl.first, sl.second), sr.second);
        return res;
    }
    void update(int k, T val){
        auto sr = split(root, k+1);
        auto sl = split(sr.first, k);
        sl.second->val = val;
        root = merge(merge(sl.first,sl.second),sr.second);
    }
    T query(int l,int r){
        if(l >= r)  return 0;
        auto sr = split(root,r);
        auto sl = split(sr.first,l);
        T res = que(sl.second);
        root = merge(merge(sl.first, sl.second), sr.second);
        return res;
    }
};
