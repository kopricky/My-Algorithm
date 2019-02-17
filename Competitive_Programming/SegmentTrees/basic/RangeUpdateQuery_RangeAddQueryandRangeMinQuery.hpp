#include "../../header.hpp"

//区間更新と区間加算の2種類の遅延伝播を処理できる(ならしO(log(n)))
//verifyはしていません(verify問題を知らない)

template<typename T> class segtree {
private:
    int n,sz,h; vector<T> node, lazy_update, lazy_add; vector<bool> lazyFlag;
public:
    segtree(vector<T>& v) : n(1), sz((int)v.size()), h(0){
        while(n < sz) n *= 2, h++;
        node.resize(2*n, numeric_limits<T>::max());
        lazy_update.resize(2*n, 0); lazyFlag.resize(2*n, false);
        lazy_add.resize(2*n, 0);
        for(int i = 0; i < sz; i++) node[i+n] = v[i];
        for(int i=n-1; i>=1; i--) node[i] = min(node[2*i],node[2*i+1]);
    }
    void eval(int k) {
        if(lazyFlag[k]){
            lazy_update[k] += lazy_add[k];
            node[k] = lazy_update[k];
            if(k < n) {
                lazy_add[2*k] = lazy_add[2*k+1] = 0;
                lazy_update[2*k] = lazy_update[2*k+1] = lazy_update[k];
                lazyFlag[2*k] = lazyFlag[2*k+1] = true;
            }
            lazy_add[k] = 0, lazyFlag[k] = false;
        }else if(lazy_add[k] != 0){
            node[k] += lazy_add[k];
            if(k < n){
                lazy_add[2*k] += lazy_add[k]; lazy_add[2*k+1] += lazy_add[k];
            }
            lazy_add[k] = 0;
        }
    }
    void update(int a, int b, T x, int k=1, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k);
        if(b <= l || r <= a) return;
        if(a <= l && r <= b){
            lazy_add[k] = 0; lazy_update[k] = x; lazyFlag[k] = true; eval(k);
        }else{
            update(a, b, x, 2*k, l, (l+r)/2); update(a, b, x, 2*k+1, (l+r)/2, r);
            node[k] = min(node[2*k],node[2*k+1]);
        }
    }
    void add(int a, int b, T x, int k=1, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k);
        if(b <= l || r <= a) return;
        if(a <= l && r <= b){
            lazy_add[k] += x; eval(k);
        }else{
            add(a, b, x, 2*k, l, (l+r)/2); add(a, b, x, 2*k+1, (l+r)/2, r);
            node[k] = min(node[2*k],node[2*k+1]);
        }
    }
    T query(int a, int b) {
        a += n, b += n - 1;
        for(int i = h; i > 0; i--) eval(a >> i), eval(b >> i);
        b++;
        T res1 = numeric_limits<T>::max(), res2 = numeric_limits<T>::max();
        while(a < b) {
            if(a & 1) eval(a), res1 = min(res1, node[a++]);
            if(b & 1) eval(--b), res2 = min(res2, node[b]);
            a >>= 1, b >>= 1;
        }
        return min(res1, res2);
    }
    void print(){
        for(int i = 0; i < sz; i++){
            cout<<query(i,i+1)<< " ";
        }
        cout<<endl;
    }
};
