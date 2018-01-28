template<typename T> class Weighted_UnionFind
{
public:
    vector<int> par,nrank;
    vector<T> wd;
    int V;
    Weighted_UnionFind(int node_size){
        V = node_size;
        par.resize(V),wd.resize(V,0),nrank.resize(V,0);
        rep(i,V){
            par[i] = i;
        }
    }
    int find(int x){
        if(par[x] == x){
            return x;
        }else{
            int parent = find(par[x]);
            wd[x] += wd[par[x]];
            return par[x] = parent;
        }
    }
    T weight(int x){
        find(x);
        return wd[x];
    }
    void unite(int x,int y,T w){
        w += weight(x), w -= weight(y);
        x = find(x),y = find(y);
        if(x == y) return;
    	if(nrank[x] < nrank[y]){
            swap(x,y);
            w = -w;
        }
        nrank[x] += (nrank[x] == nrank[y]);
        par[y] = x;
        wd[y] = w;
    }
    bool same(int x,int y){
        return find(x)==find(y);
    }
    T diff(int x,int y){
        return weight(y)-weight(x);
    }
};
