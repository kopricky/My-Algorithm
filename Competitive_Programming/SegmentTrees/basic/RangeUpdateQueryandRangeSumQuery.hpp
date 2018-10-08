#include "../../header.hpp"

template<typename T> class segtree {
private:
    int n,sz,h;
    vector<T> node, lazy;

public:
    segtree(vector<T>& v) : sz((int)v.size()), h(0){
        n = 1;
        while(n < sz){
            n *= 2, h++;
        }
        node.resize(2*n, 0), lazy.resize(2*n, 0);
        for(int i = 0; i < sz; i++){
            node[i+n] = v[i];
        }
        for(int i=n-1; i>=1; i--){
            node[i] = node[i*2] + node[i*2+1];
        }
    }
    void eval(int k) {
        if(lazy[k]) {
            node[k] = lazy[k];
            if(k < n) {
                lazy[k*2] = lazy[k*2+1] = lazy[k]/2;
            }
            lazy[k] = 0;
        }
    }
    void range(int a, int b, T x, int k=1, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k);
        if(b <= l || r <= a) return;
        if(a <= l && r <= b) {
            lazy[k] = (r - l) * x, eval(k);
        }
        range(a, b, x, 2*k, l, (l+r)/2), range(a, b, x, 2*k+1, (l+r)/2, r);
        node[k] = node[2*k] + node[2*k+1];
    }
    T query(int a, int b) {
        a += n, b += n - 1;
        for(int i = h; i > 0; i--) eval(a >> i), eval(b >> i);
        b++;
        T res1 = 0, res2 = 0;
        while(a < b) {
            if(a & 1) eval(a), res1 += node[a++];
            if(b & 1) eval(--b), res2 += node[b];
            a >>= 1, b >>= 1;
        }
        return res1 + res2;
    }
    void print()
    {
        for(int i = 0; i < sz; i++){
            cout << query(i,i+1) << " ";
        }
        cout << "\n";
    }
};
