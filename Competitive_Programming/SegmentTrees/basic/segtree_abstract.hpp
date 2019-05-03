#include "../../header.hpp"

template<typename T> class segtree {
private:
    int n,sz;
	T id_element;
	function<T(const T&, const T&)> binary_opr;
    vector<T> node;

public:
    segtree(const vector<T>& init_vec, const function<T(const T&, const T&)> func, T ie) :
		n(1), sz((int)init_vec.size()), binary_opr(func), id_element(ie){
        while(n < sz){
            n *= 2;
        }
        node.assign(2*n, id_element);
        for(int i = 0; i < sz; i++){
            node[i+n] = init_vec[i];
        }
        for(int i=n-1; i>=1; i--){
            node[i] = binary_opr(node[2*i], node[2*i+1]);
        }
    }
    void update(int k, T a)
    {
    	node[k+=n] = a;
    	while(k>>=1){
    		node[k] = binary_opr(node[2*k],node[2*k+1]);
    	}
    }
    T query(int a,int b)
    {
        T res1 = id_element, res2 = id_element;
        a += n, b += n;
        while(a != b){
            if(a % 2) res1 = binary_opr(res1, node[a++]);
            if(b % 2) res2 = binary_opr(node[--b], res2);
            a >>= 1, b>>= 1;
        }
        return binary_opr(res1, res2);
    }
    void print()
    {
        for(int i = 0; i < sz; i++){
            cout << query(i,i+1) << " ";
        }
        cout << endl;
    }
};
