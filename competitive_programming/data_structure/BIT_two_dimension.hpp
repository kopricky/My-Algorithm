#include "../header.hpp"

template<typename V> class BIT {
private:
    int n,m; vector<vector<V> > bit;
public:
    void add(int i,int j,V val){
        for(int x=i+1;x<n;x+=x&-x){
            for(int y=j+1;y<m;y+=y&-y){
                bit[x][y] += val;
            }
        }
    }
    //[0,i)×[0,j)
    V sum(int i,int j){
        V s = 0;
        for(int x=i+1;x>0;x-=x&-x) {
            for(int y=j+1;y>0;y-=y&-y){
                s += bit[x][y];
            }
        }
        return s;
    }
    //[i,k)×[j,l)
    V sum(int i,int j,int k,int l){
        return sum(k,l) - sum(i-1,l) - sum(k,j-1) + sum(i-1,j-1);
    }
    BIT(int sz1,int sz2){
        n = sz1 + 1, m = sz2 + 1;
        bit.resize(n,vector<V>(m,0));
    }
    BIT(vector<vector<V> >& v){
        n = (int)v.size()+1, m = (int)v[0].size()+1;
        bit.resize(n,vector<V>(m,0));
        rep(i,n){ rep(j,m){ add(i,j,v[i][j]); } }
    }
    void print_sum(){rep(i,n)rep(j,m)cout<<sum(i-1,j-1)<<" ";cout<<endl;}
};
