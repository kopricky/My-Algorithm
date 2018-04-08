#define ll long long

template<typename V> class BIT {
private:
    int n; vector<V> bit;
public:
    void add(int i,V x){ i++; while(i < n) bit[i] += x, i += i & -i;}
    V sum(int i){ i++; V s = 0; while(i>0) s += bit[i], i -= i & -i; return s;}
    BIT(){} BIT(int sz){ n = sz + 1, bit.resize(n,0);} //初期値がすべて0の場合
    BIT(vector<V> v){ n = (int)v.size()+1; bit.resize(n); rep(i,n-1) add(i,v[i]);}
    void print(){ rep(i,n-1)cout<<sum(i)-sum(i-1)<< " ";cout<<endl;}
    void print_sum(){ rep(i,n)cout<<sum(i-1)<<" ";cout<<endl;}	//-1スタート
};

//nu,nvにu,vを[0,n)に座圧した結果をいれる
pair<vector<int>,vector<int> > zaatu(vector<int>& u,vector<int>& v)
{
    int n = (int)u.size();
    vector<int> sa = u;
    sort(all(sa));
    vector<int> nu(n),nv(n);
    rep(i,n){
        nu[i] = lower_bound(all(sa),u[i]) - sa.begin();
        nv[i] = lower_bound(all(sa),v[i]) - sa.begin();
    }
    return make_pair(nu,nv);
}

//uをvに変換するのに必要な交換回数(転倒数)
ll inv_count(vector<int>& u,vector<int>& v)
{
    int n = len(u);
    vector<int> p(n);
    BIT<int> bt(n);
    ll ans = 0;
    rep(i,n){
        p[v[i]] = i;
    }
    rep(i,n){
        ans += i-bt.sum(p[u[i]]);
        bt.add(p[u[i]],1);
    }
    return ans;
}
