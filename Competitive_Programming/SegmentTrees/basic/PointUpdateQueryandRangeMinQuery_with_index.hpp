#include "../../header.hpp"

template<typename T> class segtree {
private:
    int n,sz;
    vector<pair<T, int> > node;
public:
    segtree(const vector<T>& v) : n(1), sz((int)v.size()){
        while(n < sz) n *= 2;
        node.resize(2*n, make_pair(numeric_limits<T>::max(), sz));
        for(int i = 0; i < sz; i++){
            node[i+n] = make_pair(v[i], i);
        }
        for(int i=n-1; i>=1; i--){
            node[i] = min(node[2*i], node[2*i+1]);
        }
    }
    void update(int k, T a)
    {
        node[k+n] = make_pair(a, k);
        k += n;
        while(k>>=1){
            node[k] = min(node[2*k], node[2*k+1]);
        }
    }
    pair<T, int> query(int a,int b,int k=0,int l=0,int r=-1)
    {
        pair<T, int> res1 = make_pair(numeric_limits<T>::max(), sz);
        pair<T, int> res2 = make_pair(numeric_limits<T>::max(), sz);
        a += n, b += n;
        while(a != b){
            if(a % 2) cmn(res1, node[a++]);
            if(b % 2) cmn(res2, node[--b]);
            a >>= 1, b>>= 1;
        }
        return min(res1, res2);
    }
    void print()
    {
        for(int i = 0; i < sz; i++){
            pair<T, int> p;
            p = query(i,i+1);
            cout << "st[" << i << "]: " << p.first << " " << p.second << endl;
        }
    }
};
