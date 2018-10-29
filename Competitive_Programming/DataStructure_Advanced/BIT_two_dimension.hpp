#include "../header.hpp"

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
