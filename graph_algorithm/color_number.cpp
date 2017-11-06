const int MAX_V=20;
int N[MAX_V], I[1<<MAX_V], n;
//N[v]はvに隣接する頂点集合
//I[S]はSの部分集合で独立なものの個数

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

//resはSの部分集合で独立なものをk個選ぶ方法の総数
bool possible(int k) {
    int res = 0;
    rep(S,(1 << n)){
        if(__builtin_popcount(S)%2){
            res -= mod_pow(I[S],k);
        }
        else{
            res += mod_pow(I[S],k);
        }
    }
    return (res%MOD+MOD)%MOD;
}

//I[S] = I[S\{v}] + I[S\N[v]])が成り立つ
int color_number() {
    memset(I,0,sizeof(I));
    I[0] = 1;
    for(int S=1;S<(1<<n);++S){
        int v = 0;
        while(!(S&(1<<v))){
            v++;
        }
        I[S] = I[S-(1<<v)] + I[S&(~N[v])];
    }
    int lb = 0,ub = n,mid;
    while(ub-lb>1){
        mid = (lb+ub)/2;
        if(possible(mid)){
            ub = mid;
        }
        else{
            lb = mid;
        }
    }
    return ub;
}
