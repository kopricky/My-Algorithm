#include "../../header.hpp"

template<typename T> class segtree{
private:
    int n,sz;
    vector<T> node,node_id,lazy;
    vector<bool> lazyFlag;

public:
    segtree(vector<T>& v){
        sz = (int)v.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.resize(2*n-1,numeric_limits<T>::max());
        node_id.resize(2*n-1);
        lazy.resize(2*n-1, 0);
        lazyFlag.resize(2*n-1,false);
        rep(i,sz){
            node[i+n-1] = v[i];
            node_id[i+n-1] = i;
        }
        for(int i=n-2; i>=0; i--){
            if(node[2*i+1] > node[2*i+2]){
                node[i] = node[2*i+2];
                node_id[i] = node_id[2*i+2];
            }else{
                node[i] = node[2*i+1];
                node_id[i] = node_id[2*i+1];
            }
        }
    }
    void eval(int k, int l, int r){
        if(lazyFlag[k]){
            node[k] = lazy[k];
            if(r - l > 1){
                lazy[k*2+1] = lazy[k*2+2] = lazy[k];
                lazyFlag[k*2+1] = lazyFlag[k*2+2] = true;
            }
            lazyFlag[k] = false;
        }
    }
    void range(int a, int b, T x, int k=0, int l=0, int r=-1){
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
            range(a, b, x, 2*k+1, l, (l+r)/2);
            range(a, b, x, 2*k+2, (l+r)/2, r);
            if(node[k*2+1] < node[k*2+2]){
                node[k] = node[k*2+1];
                node_id[k] = node_id[k*2+1];
            }else{
                node[k] = node[k*2+2];
                node_id[k] = node_id[k*2+2];
            }
        }
    }
    pair<T, int> query(int a, int b, int k=0, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a){
            return pair<T, int>(numeric_limits<T>::max(),-1);
        }
        if(a <= l && r <= b){
            return pair<T, int>(node[k],node_id[k]);
        }
        pair<T, int> vl = query(a, b, 2*k+1, l, (l+r)/2);
        pair<T, int> vr = query(a, b, 2*k+2, (l+r)/2, r);
        return min(vl,vr);
    }
    void print(){
        rep(i,sz){
            pair<T, int> p;
            p = query(i,i+1);
            cout << "st[" << i << "]: " << p.fi << " " << p.se << endl;
        }
    }
};
