#include "header.hpp"

//多次元領域の範囲加算および範囲総和のクエリに答える(実装は2次元)
//計算量はn*nの正方形について
//加算クエリ:16*log^2(n)
//総和クエリ:16*log^2(n)
//最悪計算量が線形であるquadtreeと比べて高速
//具体的にはd次元について計算量はO(4^d*log^d(n))
//Mishra 2013 を参照

template<typename T> class BIT_2D_RangeAdd_RangeSum {
private:
    const int n, m;
    vector<vector<T> > bitxy, bitx, bity, bitc;
    void add(const int i, const int j, const T valxy, const T valx, const T valy, const T valc){
        for(int i_ = i+1; i_ < n; i_ += i_ & -i_)
            for(int j_ = j+1; j_ < m; j_ += j_ & -j_)
                bitxy[i_][j_] += valxy, bitx[i_][j_] += valx, bity[i_][j_] += valy, bitc[i_][j_] += valc;
    }
    // [0, i] x [0, j]
    T sum(const int i, const int j){
        T s = 0;
        for(int i_ = i+1; i_ > 0; i_ -= i_ & -i_)
            for(int j_ = j+1; j_ > 0; j_ -= j_ & -j_)
                s += bitxy[i_][j_] * i * j + bitx[i_][j_] * i + bity[i_][j_] * j + bitc[i_][j_];
        return s;
    }
public:
    BIT_2D_RangeAdd_RangeSum(const int sz1, const int sz2) : n(sz1 + 1), m(sz2 + 1),
            bitxy(n, vector<T>(m, 0)), bitx(n, vector<T>(m, 0)), bity(n, vector<T>(m, 0)), bitc(n, vector<T>(m, 0)){}
    // [lx, rx)×[ly, ry) に val を足す
    void add(const int lx, const int ly, const int rx, const int ry, const T val){
        add(lx, ly, val, -val * (ly - 1), -val * (lx - 1), val * (lx - 1) * (ly - 1));
        add(rx, ly, -val, val * (ly - 1), val * (rx - 1), -val * (rx - 1) * (ly - 1));
        add(lx, ry, -val, val * (ry - 1), val * (lx - 1), -val * (lx - 1) * (ry - 1));
        add(rx, ry, val, -val * (ry - 1), -val * (rx - 1), val * (rx - 1) * (ry - 1));
    }
    // [lx, rx)×[ly, ry) の和を求める
    T sum(const int lx, const int ly, const int rx, const int ry){
        return sum(rx - 1, ry - 1) - sum(lx - 1, ry - 1) - sum(rx - 1, ly - 1) + sum(lx - 1, ly - 1);
    }
    void print(){
        for(int i = 0; i < n-1; ++i){
            for(int j = 0; j < m-1; ++j){
                cout << sum(i, j, i+1, j+1) << " ";
            }
            cout << endl;
        }
    }
};
