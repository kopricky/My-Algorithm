#include "../../header.hpp"

template<typename T> class segtree {
private:
    int n,sz,h; vector<T> node, lazy, lazyFlag;
    void eval(int k) {
        if(lazyFlag[k]){
            node[k] = lazy[k];
            if(k < n) {
                lazy[k*2] = lazy[k*2+1] = lazy[k]; lazyFlag[k*2] = lazyFlag[k*2+1] = true;
            }
            lazyFlag[k] = false;
        }
    }

public:
    segtree(vector<T>& v) : sz((int)v.size()), h(0){
        n = 1;
        while(n < sz) n *= 2, h++;
        node.resize(2*n, numeric_limits<T>::max());
        lazy.resize(2*n); lazyFlag.resize(2*n, false);
        for(int i = 0; i < sz; i++) node[i+n] = v[i];
        for(int i = n - 1; i >= 1; i--) node[i] = min(node[i*2],node[i*2+1]);
    }
    void range(int a, int b, T x) {
        a += n, b += n - 1;
        for(int i = h; i > 0; i--){
            eval(a >> i), eval(b >> i);
        }
        int ta = a, tb = b++;
        while(a < b){
            if(a & 1) lazy[a] = x, lazyFlag[a++] = true;
            if(b & 1) lazy[--b] = x, lazyFlag[b] = true;
            a >>= 1, b >>= 1;
        }
        while(ta >>= 1, tb >>= 1, ta){
            if(!lazyFlag[ta]){
                eval(2*ta), eval(2*ta+1), node[ta] = min(node[2*ta], node[2*ta+1]);
            }
            if(!lazyFlag[tb]){
                eval(2*tb), eval(2*tb+1), node[tb] = min(node[2*tb], node[2*tb+1]);
            }
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
    void print(){for(int i = 0; i < sz; i++) cout<<query(i,i+1)<< " ";cout<<endl;}
};
