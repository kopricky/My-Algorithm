#include "../../header.hpp"

//更新 a_i → a_i & x (l <= i < r)
//更新 a_i → a_i | x (l <= i < r)
//クエリ max ([l]...a[r-1])
//計算量はならしO(log(MAX_VAL)log(n))

template<typename T> class segtree
{
private:
    vector<T> node, And, Or, lazy;
    int n,sz;
    void update(int id){
        node[id] = max(node[2*id+1],node[2*id+2]);
        And[id] = (And[2*id+1] & And[2*id+2]);
        Or[id] = (Or[2*id+1] | Or[2*id+2]);
    }
    void eval(int k, int l, int r){
        if(lazy[k] != 0){
            node[k] += lazy[k], And[k] += lazy[k], Or[k] += lazy[k];
            if(r - l > 1) {
                lazy[2*k+1] += lazy[k], lazy[2*k+2] += lazy[k];
            }
            lazy[k] = 0;
        }
    }
public:
    segtree(vector<T>& v){
        sz = (int)v.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.resize(2*n-1, 0), And.resize(2*n-1, 0);
        Or.resize(2*n-1, 0), lazy.resize(2*n-1, 0);
        rep(i,sz){
            node[i+n-1] = And[i+n-1] = Or[i+n-1] = v[i];
        }
        for(int i=n-2; i>=0; i--){
            update(i);
        }
    }
    void range_and(int a, int b, T x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a || !(~x & Or[k])){
            return;
        }
        if(a <= l && r <= b && (~x & And[k]) == (~x & Or[k])){
            lazy[k] -= (~x & And[k]);
            eval(k,l,r);
        }else{
            range_and(a, b, x, 2*k+1, l, (l+r)/2), range_and(a, b, x, 2*k+2, (l+r)/2, r);
            update(k);
        }
    }
    void range_or(int a, int b, T x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a || !(x & ~And[k])){
            return;
        }
        if(a <= l && r <= b && (x & ~Or[k]) == (x & ~And[k])){
            lazy[k] += (x & ~Or[k]);
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
        return max(vl,vr);
    }
    void print(){
        rep(i,sz){
            cout << query(i,i+1) << " ";
        }
        cout << endl;
    }
};
