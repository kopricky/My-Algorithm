#include "../../header.hpp"

template<typename T> class segtree {
private:
    int n,sz,h; vector<T> node, lazy;
public:
    segtree(vector<T>& v) : sz((int)v.size()), h(0){
        n = 1;
        while(n < sz) n *= 2, h++;
        node.resize(2*n, numeric_limits<T>::max()); lazy.resize(2*n, 0);
        for(int i = 0; i < sz; i++) node[i+n] = v[i];
        for(int i=n-1; i>=1; i--) node[i] = min(node[i*2],node[i*2+1]);
    }
    void eval(int k){
        if(lazy[k]) {
            node[k] += lazy[k];
            if(k < n) {
                lazy[2*k] += lazy[k]; lazy[2*k+1] += lazy[k];
            }
            lazy[k] = 0;
        }
    }
    void range(int a, int b, T x, int k=1, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k);
        if(b <= l || r <= a) return;
        if(a <= l && r <= b){
            lazy[k] += x; eval(k);
        }else{
            range(a, b, x, 2*k, l, (l+r)/2); range(a, b, x, 2*k+1, (l+r)/2, r);
            node[k] = min(node[2*k],node[2*k+1]);
        }
    }
    T query(int a, int b) {
        a += n, b += n - 1;
        for(int i = h; i > 0; i--) eval(a >> i), eval(b >> i);
        b++;
        T res1 = numeric_limits<T>::max(), res2 = numeric_limits<T>::max();
        while(a < b) {
            if(a & 1) eval(a), res1 += node[a++];
            if(b & 1) eval(--b), res2 += node[b];
            a >>= 1, b >>= 1;
        }
        return min(res1, res2);
    }
    void print(){for(int i=0;i<sz;i++)cout<<query(i,i+1)<< " ";cout<<endl;}
};
