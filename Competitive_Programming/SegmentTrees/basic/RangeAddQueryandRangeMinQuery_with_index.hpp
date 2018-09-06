#include "../../header.hpp"

template<typename T> class segtree{
private:
    int n, sz;
    vector<pair<T, int> > node;
    vector<T> lazy;

public:
    segtree(vector<T>& v) : sz((int)v.size()){
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.resize(2*n-1, make_pair(numeric_limits<T>::max(), -1));
        lazy.resize(2*n-1, 0);
        for(int i = 0; i < sz; i++){
            node[i+n-1] = make_pair(v[i], i);
        }
        for(int i=n-2; i>=0; i--){
            node[i] = min(node[i*2+1], node[i*2+2]);
        }
    }
    void eval(int k, int l, int r){
        if(lazy[k] != 0){
            node[k].first += lazy[k];
            if(r - l > 1) {
                lazy[k*2+1] += lazy[k], lazy[k*2+2] += lazy[k];
            }
            lazy[k] = 0;
        }
    }
    void range(int a, int b, T x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a){
            return;
        }
        if(a <= l && r <= b){
            lazy[k] += x;
            eval(k, l, r);
        }else{
            range(a, b, x, 2*k+1, l, (l+r)/2), range(a, b, x, 2*k+2, (l+r)/2, r);
            node[k] = min(node[k*2+1], node[k*2+2]);
        }
    }
    pair<T, int> query(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a){
            return make_pair(numeric_limits<T>::max(), -1);
        }
        if(a <= l && r <= b){
            return node[k];
        }
        pair<T, int> vl = query(a, b, 2*k+1, l, (l+r)/2), vr = query(a, b, 2*k+2, (l+r)/2, r);
        return min(vl, vr);
    }
    void print(){
        for(int i = 0; i < sz; i++){
            pair<T,int> p;
            p = query(i,i+1);
            cout << "st[" << i << "]: " << p.fi << " " << p.se << endl;
        }
    }
};
