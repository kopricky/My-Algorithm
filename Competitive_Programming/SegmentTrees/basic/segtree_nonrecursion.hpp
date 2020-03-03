#include "../../header.hpp"

template<typename T> class segtree {
private:
    int n,sz;
    vector<T> node;
public:
    segtree(const vector<T>& v) : n(1), sz((int)v.size()){
        while(n < sz) n *= 2;
        node.assign(2*n,numeric_limits<T>::max());
        for(int i = 0; i < sz; i++){
            node[i+n] = v[i];
        }
        for(int i=n-1; i>=1; i--){
            node[i] = min(node[2*i],node[2*i+1]);
        }
    }
    void update(int k,T a){
        node[k+=n] = a;
        while(k>>=1){
            node[k] = min(node[2*k],node[2*k+1]);
        }
    }
    T query(int a,int b){
        T res1 = numeric_limits<T>::max();
        T res2 = numeric_limits<T>::max();
        a += n, b += n;
        while(a != b){
            if(a & 1) res1 = min(res1, node[a++]);
            if(b & 1) res2 = min(node[--b], res2);
            a >>= 1, b >>= 1;
        }
        return min(res1, res2);
    }
    void print(){
        for(int i = 0; i < sz; i++){
            cout << query(i,i+1) << " ";
        }
        cout << endl;
    }
};
