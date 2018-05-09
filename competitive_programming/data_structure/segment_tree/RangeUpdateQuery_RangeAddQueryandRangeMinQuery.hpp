#include "../../header.hpp"

//区間更新と区間加算の2種類の遅延伝播を処理できる(ならしO(log(n)))
//verifyはしていません(verify問題を知らない)

template<typename T> class segtree {
private:
    int n,sz; vector<T> node, lazy_update, lazy_add; vector<bool> lazyFlag;
public:
    segtree(vector<T> v) {
        sz = (int)v.size(); n = 1;
        while(n < sz) n *= 2;
        node.resize(2*n-1,numeric_limits<T>::max());
        lazy_update.resize(2*n-1, 0); lazyFlag.resize(2*n-1,false);
        lazy_add.resize(2*n-1, 0);
        rep(i,sz) node[i+n-1] = v[i];
        for(int i=n-2; i>=0; i--) node[i] = min(node[2*i+1],node[2*i+2]);
    }
    void eval(int k, int l, int r) {
        if(lazyFlag[k]){
            lazy_update[k] += lazy_add[k];
            node[k] = lazy_update[k];
            if(r - l > 1) {
                lazy_add[2*k+1] = lazy_add[2*k+2] = 0;
                lazy_update[2*k+1] = lazy_update[2*k+2] = lazy_update[k];
                lazyFlag[2*k+1] = lazyFlag[2*k+2] = true;
            }
            lazy_add[k] = 0;
            lazyFlag[k] = false;
        }else if(lazy_add[k] != 0){
            node[k] += lazy_add[k];
            if(r - l > 1){
                lazy_add[2*k+1] += lazy_add[k]; lazy_add[2*k+2] += lazy_add[k];
            }
            lazy_add[k] = 0;
        }
    }
    void update(int a, int b, T x, int k=0, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a) return;
        if(a <= l && r <= b){
            lazy_add[k] = 0; lazy_update[k] = x; lazyFlag[k] = true;
            eval(k, l, r);
        }else{
            update(a, b, x, 2*k+1, l, (l+r)/2); update(a, b, x, 2*k+2, (l+r)/2, r);
            node[k] = min(node[2*k+1],node[2*k+2]);
        }
    }
    void add(int a, int b, T x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a) return;
        if(a <= l && r <= b){
            lazy_add[k] += x; eval(k, l, r);
        }else{
            add(a, b, x, 2*k+1, l, (l+r)/2); add(a, b, x, 2*k+2, (l+r)/2, r);
            node[k] = min(node[2*k+1],node[2*k+2]);
        }
    }
    T query(int a, int b, int k=0, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a) return numeric_limits<T>::max();
        if(a <= l && r <= b) return node[k];
        return min(query(a, b, 2*k+1, l, (l+r)/2), query(a, b, 2*k+2, (l+r)/2, r));
    }
    void print(){rep(i,sz)cout<<query(i,i+1)<< " ";cout<<endl;}
};
