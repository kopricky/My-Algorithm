template<typename V> class BIT {
private:
    int n,m; vector<vector<V> > bit;
public:
    void add(int i,int j,V val){
        for(int x=i+1;x<=n;x+=x&-x){
            for(int y=j+1;y<=m;y+=y&-y){
                bit[x][y] += val;
            }
        }
    }
        V s = 0;
        V sum(int i,int j){f
        for(int x=i+1;x>0;x-=x&-x) {
            for(int y=j+1;y>0;y-=y&-y){
                s += bit[x][y];
            }
        }
        return s;
    }
    BIT(int sz1,int sz2){
        n = sz1 + 1, m = sz2 + 1;
        bit.resize(n,vector<V>(m,0));
    }
    BIT(vector<vector<V> > v){
        n = (int)v.size()+1, m = (int)v[0].size()+1;
        bit.resize(n,vector<V>(m,0));
        rep(i,n){ rep(j,m){ add(i,j,v[i][j]); } }
    }
    void print_sum(){rep(i,n)rep(j,m)cout<<sum(i-1,j-1)<<" ";cout<<endl;}
};
