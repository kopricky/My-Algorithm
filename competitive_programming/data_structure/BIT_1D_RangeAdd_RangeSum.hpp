#include "../header.hpp"
#include "BIT.hpp"

//区間加算,区間総和をBIT2個で求める手法(一次元だとあまりうれしさがない)

template<typename T> class BIT_1D_RangeAdd_RangeSum {
private:
    int n;
    BIT<T> bitc, biti;
    void add(BIT<T>& bit, int l, int r, T val){
        bit.add(l, val), bit.add(r, -val);
    }
public:
    BIT_1D_RangeAdd_RangeSum(int sz) : n(sz), bitc(sz), biti(sz){}
    void add(int l, int r, T val){
        add(bitc, l, r, val), add(biti, l, r, -val*(l-1)), add(biti, r, n, val*(r-l));
    }
    T sum(int x){
        return bitc.sum(x) * x + biti.sum(x);
    }
    T sum(int l, int r){
        return sum(r-1) - sum(l-1);
    }
    void print(){ rep(i,n)cout<<sum(i,i+1)<< " ";cout<<endl;}
};
