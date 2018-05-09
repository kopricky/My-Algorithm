#include "../header.hpp"

template<typename T> class segtree{
private:
    int n,sz;
    vector<T> node;
    vector<int> node_id;
public:
    segtree(vector<T>& v){
        sz = (int)v.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.resize(2*n-1,numeric_limits<T>::max()),node_id.resize(2*n-1);
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
    void update(int k,T a){
    	k += n-1;
    	node[k] = a,node_id[k] = k-(n-1);
    	while(k>0){
    		k = (k-1)/2;
    		if(node[2*k+1] < node[2*k+2]){
                node[k] = node[2*k+1],node_id[k] = node_id[2*k+1];
            }else{
                node[k] = node[2*k+2],node_id[k] = node_id[2*k+2];
            }
    	}
    }
    pair<T, int> query(int a,int b,int k=0,int l=0,int r=-1){
        if(r < 0) r = n;
    	if(r <= a || b <= l){
    		return pair<T, int>(numeric_limits<T>::max(),-1);
    	}
    	if(a <= l && r <= b){
    		return pair<T, int>(node[k],node_id[k]);
    	}else{
    		pair<T, int> vl = query(a,b,2*k+1,l,(l+r)/2);
    		pair<T, int> vr = query(a,b,2*k+2,(l+r)/2,r);
    		return min(vl,vr);
    	}
    }
    void print(){
        rep(i,sz){
            pair<T, int> p;
            p = query(i,i+1);
            cout << "st[" << i << "]: " << p.fi << " " << p.se << endl;
        }
    }
};
