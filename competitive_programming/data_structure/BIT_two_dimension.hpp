#include "../header.hpp"

template<typename V> class BIT {
private:
    int n,m; vector<vector<V> > bit;
public:
    void add(int i,int j,V val){
        i++, j++;
        while(i < n){
            while(j < m){
                bit[i][j] += val;
                j += j & -j;
            }
            i += i & -i;
        }
    }
    V sum(int i,int j){
        i++, j++;
        V s = 0;
        while(i > 0){
            while(j > 0){
                s += bit[i][j];
                j -= j & -j;
            }
            i -= i & -i;
        }
        return s;
    }
    BIT(int sz1,int sz2){
        n = sz1 + 1, m = sz2 + 1;
        bit.resize(n,vector<V>(m,0));
    }
    BIT(vector<vector<V> >& v){
        n = (int)v.size()+1, m = (int)v[0].size()+1;
        bit.resize(n,vector<V>(m,0));
        rep(i,n) rep(j,m) add(i,j,v[i][j]);
    }
    void print_sum(){rep(i,n){ rep(j,m)cout<<sum(i-1,j-1)<<" ";cout<<endl;} }
};
