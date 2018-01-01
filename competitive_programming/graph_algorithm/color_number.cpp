class color_number
{
public:
    int V;
    vi adj,I;
    color_number(int node_size){
        V = node_size;
        adj.resize(V,0);
        rep(i,V){
            adj[i] = (1 << i);
        }
        I.resize((1 << V),0);
    }
    void add_edge(int u, int v){
        adj[u] |= (1 << v),adj[v] |= (1 << u);
    }
    ll mod_pow(ll a,ll b)
    {
        a %= MOD;
        ll res = 1;
        while(b){
            if(b & 1){
                res = res * a % MOD;
            }
            a = a * a %MOD;
            b >>= 1;
        }
        return res;
    }
    //resは独立集合をk個選んでその和集合がSの部分集合になる場合の数
    //I[S]は独立集合でかつSの部分集合であるものの個数
    //重複して数え上げないように包除原理で求める
    bool possible(int k) {
        int res = 0;
        rep(S,(1 << V)){
            if(__builtin_popcount(S)%2){
                res -= mod_pow(I[S],k);
            }else{
                res += mod_pow(I[S],k);
            }
        }
        return (res%MOD+MOD)%MOD;
    }
    int solve(){
        I[0] = 1;
        for(int S = 1; S < (1 << V); ++S){
            int v = 0;
            while(!(S & (1 << v))){
                v++;
            }
            I[S] = I[S^(1 << v)] + I[S&(~adj[v])];
        }
        int l = 0,r = V,mid;
        while(r-l>1){
            mid = (l+r)/2;
            if(possible(mid)){
                r = mid;
            }else{
                l = mid;
            }
        }
        return r;
    }
};
