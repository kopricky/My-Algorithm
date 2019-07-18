#include "../../header.hpp"

//更新 a_i → a_i & x (l <= i < r)
//更新 a_i → a_i | x (l <= i < r)
//クエリ min (a[l]...a[r-1])
//計算量はならし O(max(log n, log MAX_VAL))

template<typename T> class segtree
{
private:
    int n,sz;
    vector<T> node, _and, _or, lazy;
    void update(int id){
        node[id] = min(node[2*id+1],node[2*id+2]);
        _and[id] = (_and[2*id+1] & _and[2*id+2]);
        _or[id] = (_or[2*id+1] | _or[2*id+2]);
    }
    void eval(int k, int l, int r){
        if(lazy[k] != 0){
            node[k] += lazy[k], _and[k] += lazy[k], _or[k] += lazy[k];
            if(r - l > 1) {
                lazy[2*k+1] += lazy[k], lazy[2*k+2] += lazy[k];
            }
            lazy[k] = 0;
        }
    }
public:
    segtree(const vector<T>& v) : n(1), sz((int)v.size()){
        while(n < sz) n *= 2;
        node.resize(2*n-1, numeric_limits<T>::max()), _and.resize(2*n-1, 0);
        _or.resize(2*n-1, 0), lazy.resize(2*n-1, 0);
        for(int i = 0; i < sz; i++){
            node[i+n-1] = _and[i+n-1] = _or[i+n-1] = v[i];
        }
        for(int i=n-2; i>=0; i--){
            update(i);
        }
    }
    void range_and(int a, int b, T x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a || !(~x & _or[k])){
            return;
        }
        if(a <= l && r <= b && (~x & _and[k]) == (~x & _or[k])){
            lazy[k] -= (~x & _and[k]);
            eval(k,l,r);
        }else{
            range_and(a, b, x, 2*k+1, l, (l+r)/2), range_and(a, b, x, 2*k+2, (l+r)/2, r);
            update(k);
        }
    }
    void range_or(int a, int b, T x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a || !(x & ~_and[k])){
            return;
        }
        if(a <= l && r <= b && (x & ~_or[k]) == (x & ~_and[k])){
            lazy[k] += (x & ~_or[k]);
            eval(k,l,r);
        }else{
            range_or(a, b, x, 2*k+1, l, (l+r)/2), range_or(a, b, x, 2*k+2, (l+r)/2, r);
            update(k);
        }
    }
    T query(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a){
            return 0;
        }
        if(a <= l && r <= b){
            return node[k];
        }
        T vl = query(a, b, 2*k+1, l, (l+r)/2), vr = query(a, b, 2*k+2, (l+r)/2, r);
        return min(vl,vr);
    }
    void print(){
        for(int i = 0; i < sz; i++){
            cout << query(i,i+1) << " ";
        }
        cout << endl;
    }
};
