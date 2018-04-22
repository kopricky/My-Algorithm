#include "../header.hpp"
#include "BIT_two_dimension.hpp"

//多次元領域の範囲加算および範囲総和のクエリに答える(実装は2次元)
//計算量はn*nの正方形について
//加算クエリ:36*log^2(n)
//総和クエリ:16*log^2(n)
//最悪計算量が線形であるquadtreeと比べて高速
//具体的にはd次元について計算量はO(4^d*log^d(n))
//Mishra 2013 を参照

template<typename T> class BIT_2D_RangeAdd_RangeSum {
private:
    int n,m;
    void add(BIT<T>& bit,int lx, int ly, int rx, int ry, T val){
        bit.add(lx, ly, val), bit.add(lx, ry, -val);
        bit.add(rx, ly, -val), bit.add(rx, ry, val);
    }
public:
    BIT<T> bitxy, bitx, bity, biti;
    BIT_2D_RangeAdd_RangeSum(int sz1, int sz2) : n(sz1), m(sz2),
            bitxy(sz1, sz2), bitx(sz1, sz2), bity(sz1, sz2), biti(sz1, sz2){}
    void add(int lx, int ly, int rx, int ry, T val){
        add(bitxy, lx, ly, rx, ry, val);
        add(bitx, lx, ly, rx, ry, -val*(ly-1));
        add(bitx, lx, ry, rx, m, val*(ry-ly));
        add(bity, lx, ly, rx, ry, -val*(lx-1));
        add(bity, rx, ly, n, ry, val*(rx-lx));
        add(biti, lx, ly, rx, ry, val*(lx-1)*(ly-1));
        add(biti, rx, ly, n, ry, -val*(ly-1)*(rx-lx));
        add(biti, lx, ry, rx, m, -val*(lx-1)*(ry-ly));
        add(biti, rx, ry, n, m, val*(rx-lx)*(ry-ly));
    }
    T sum(int x, int y){
        return bitxy.sum(x, y)*x*y + bitx.sum(x, y)*x + bity.sum(x, y)*y + biti.sum(x, y);
    }
    T sum(int lx, int ly, int rx, int ry){
        return sum(rx-1, ry-1) - sum(lx-1, ry-1) - sum(rx-1, ly-1) + sum(lx-1, ly-1);
    }
    void print(){
        rep(i,n){
            rep(j,m)
                cout<<sum(i,j,i+1,j+1)<<" ";
            cout<<endl;
        }
    }
};
