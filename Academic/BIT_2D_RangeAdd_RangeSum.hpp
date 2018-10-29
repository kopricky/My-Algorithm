#include "header.hpp"

//多次元領域の範囲加算および範囲総和のクエリに答える(実装は2次元)
//計算量はn*nの正方形について
//加算クエリ:36*log^2(n)
//総和クエリ:16*log^2(n)
//最悪計算量が線形であるquadtreeと比べて高速
//具体的にはd次元について計算量はO(4^d*log^d(n))
//Mishra 2013 を参照

template<typename T> class BIT {
private:
    int n,m; vector<vector<T> > bit;
public:
    void add(int i, int j, T val){
        for(int i_ = i+1; i_ < n; i_ += i_ & -i_)
            for(int j_ = j+1; j_ < m; j_ += j_ & -j_)
                bit[i_][j_] += val;
    }
    T sum(int i, int j){
        T s = 0;
            for(int i_ = i+1; i_ > 0; i_ -= i_ & -i_)
                for(int j_ = j+1; j_ > 0; j_ -= j_ & -j_)
                    s += bit[i_][j_];
            return s;
    }
    T sum(int lx, int uy, int rx, int dy){
        return sum(rx, dy) - sum(lx-1, dy) - sum(rx, uy-1) + sum(lx-1, uy-1);   
    }
    BIT(int sz1, int sz2){
        n = sz1 + 1, m = sz2 + 1;
        bit.resize(n, vector<T>(m, 0));
    }
    BIT(vector<vector<T> >& v){
        n = (int)v.size()+1, m = (int)v[0].size()+1;
        bit.resize(n, vector<T>(m, 0));
        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
                add(i, j, v[i][j]);
    }
    void print_sum(){
        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
                cout<< sum(i-1, j-1) << " ";
        cout << "\n";
    }
};

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
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                cout<<sum(i,j,i+1,j+1)<<" ";
            }
            cout<<endl;
        }
    }
};
