#include "../../header.hpp"

//更新 a_i → a_i (l <= i < r)
//modulo更新 a_i → a_i % mod (l <= i < r)
//クエリ sum ([l]...a[r-1])
//計算量はならしO(log^2(n))

template<typename T> class segtree {
private:
    int n,sz;
    vector<T> node, min_val, max_val, lazy;
    vector<bool> lazyFlag;
    void opr(int id){
        node[id] = node[2*id+1] + node[2*id+2];
        min_val[id] = min(min_val[2*id+1],min_val[2*id+2]);
        max_val[id] = max(max_val[2*id+1],max_val[2*id+2]);
    }
public:
    segtree(vector<T>& v) {
        sz = (int)v.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.resize(2*n-1,0);
        lazy.resize(2*n-1,0);
        lazyFlag.resize(2*n-1,false);
        min_val.resize(2*n-1,numeric_limits<T>::max());
        max_val.resize(2*n-1,numeric_limits<T>::min());
        rep(i,sz){
            node[i+n-1] = min_val[i+n-1] = max_val[i+n-1] = v[i];
        }
        for(int i=n-2; i>=0; i--){
            opr(i);
        }
    }
    void eval(int k, int l, int r) {
        if(lazyFlag[k]){
            node[k] = (r - l) * lazy[k];
            min_val[k] = max_val[k] = lazy[k];
            if(r - l > 1){
                lazy[k*2+1] = lazy[k*2+2] = lazy[k];
                lazyFlag[k*2+1] = lazyFlag[k*2+2] = true;
            }
            lazyFlag[k] = false;
        }
    }
    void update(int a, int b, T x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a){
            return;
        }
        if(a <= l && r <= b){
            lazy[k] = x;
            lazyFlag[k] = true;
            eval(k, l, r);
        }else{
            update(a, b, x, 2*k+1, l, (l+r)/2);
            update(a, b, x, 2*k+2, (l+r)/2, r);
            opr(k);
        }
    }
    void modulo_update(int a, int b, T x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a || max_val[k] < x){
            return;
        }
        if(a <= l && r <= b && max_val[k] == min_val[k]) {
            lazy[k] = max_val[k] % x;
            lazyFlag[k] = true;
            eval(k, l, r);
        }else{
            modulo_update(a, b, x, 2*k+1, l, (l+r)/2);
            modulo_update(a, b, x, 2*k+2, (l+r)/2, r);
            opr(k);
        }
    }
    T query(int a, int b, int k=0, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a){
            return 0;
        }
        if(a <= l && r <= b){
            return node[k];
        }
        T vl = query(a, b, 2*k+1, l, (l+r)/2);
        T vr = query(a, b, 2*k+2, (l+r)/2, r);
        return vl + vr;
    }
    void print()
    {
        rep(i,sz){
            cout << query(i,i+1) << " ";
        }
        cout << endl;
    }
};
