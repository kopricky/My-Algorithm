#include "../../header.hpp"

template<typename T> class segtree{
private:
    int n,sz;
    vector<T> node;
public:
    segtree(vector<T>& v) : n(1), sz((int)v.size()){
        while(n < sz) n *= 2;
        node.resize(2*n-1, numeric_limits<T>::max());
        for(int i = 0; i < sz; i++){
            node[i+n-1] = v[i];
        }
        for(int i=n-2; i>=0; i--){
            node[i] = min(node[i*2+1], node[i*2+2]);
        }
    }
    void update(int k,T a){
    	k += n-1;
    	node[k] = a;
    	while(k>0){
            k = (k-1)/2;
            node[k] = min(node[2*k+1],node[2*k+2]);
    	}
    }
    T query(int a,int b,int k=0,int l=0,int r=-1){
        if(r < 0) r = n;
    	if(r <= a || b <= l){
            return numeric_limits<T>::max();
    	}
    	if(a <= l && r <= b){
            return node[k];
    	}else{
            T vl = query(a,b,2*k+1,l,(l+r)/2);
            T vr = query(a,b,2*k+2,(l+r)/2,r);
            return min(vl,vr);
    	}
    }
    void print(){
        for(int i = 0; i < sz; i++){
            cout<<query(i,i+1)<< " ";
        }
        cout<<endl;
    }
};
