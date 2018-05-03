#include "../header.hpp"

template<typename T> class segtree {
private:
    int n,sz;
    vector<T> node;
public:
    segtree(vector<T>& v){
        sz = (int)v.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.assign(2*n,numeric_limits<T>::max());
        rep(i,sz){
            node[i+n] = v[i];
        }
        for(int i=n-1; i>=1; i--){
            node[i] = min(node[2*i],node[2*i+1]);
        }
    }
    void update(int k,T a)
    {
    	node[k+=n] = a;
    	while(k>>=1){
    		node[k] = min(node[2*k],node[2*k+1]);
    	}
    }
    T query(int a,int b)
    {
        T res1 = numeric_limits<T>::max();
        T res2 = numeric_limits<T>::max();
        a += n, b += n;
        while(a != b){
            if(a % 2) res1 = min(res1, node[a++]);
            if(b % 2) res2 = min(res2, node[--b]);
            a >>= 1, b>>= 1;
        }
        return min(res1, res2);
    }
    void print(){ rep(i,sz) cout << query(i,i+1) << " "; cout << endl; }
};
