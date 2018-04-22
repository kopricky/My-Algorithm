#include "../header.hpp"

template<typename T> class BIT {
private:
    int n,m; vector<vector<T> > bit;
public:
    void add(int x, int y, T val){
        for(int i = x+1; i < n; i += i&-i){
            for(int j = y+1; j < m; j += j&-j){
                bit[i][j] += val;
            }
        }
    }
    T sum(int x, int y){
        T s = 0;
        for(int i = x+1; i > 0; i -= i&-i){
            for(int j = y+1; j > 0; j -= j&-j){
                s += bit[i][j];
            }
        }
        return s;
    }
    T sum(int lx, int ly, int rx, int ry){
        return sum(rx-1, ry-1) - sum(rx-1, ly-1) - sum(lx-1, ry-1) + sum(lx-1, ly-1);
    }
    BIT(int sz1,int sz2){
        n = sz1 + 1, m = sz2 + 1;
        bit.resize(n,vector<T>(m,0));
    }
    BIT(vector<vector<T> >& v){
        n = (int)v.size()+1, m = (int)v[0].size()+1;
        bit.resize(n,vector<T>(m,0));
        rep(i,n) rep(j,m) add(i,j,v[i][j]);
    }
    void print(){ rep(i,n-1){ rep(j,m-1) cout<<sum(i,j,i+1,j+1)<<" "; cout<<endl;} }
};
