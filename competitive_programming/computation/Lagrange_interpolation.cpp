ll inv[MAX_N],fac[MAX_N],finv[MAX_N];

void make()
{
	fac[0] = fac[1] = 1;
	finv[0] = finv[1] = 1;
	inv[1] = 1;
	for(int i=2;i<MAX_N;i++){
		inv[i] = MOD - inv[MOD%i] * (MOD/i) % MOD;
		fac[i] = fac[i-1] * (ll) i % MOD;
		finv[i] = finv[i-1] * inv[i] % MOD;
	}
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

void extgcd(int a,int b, int& x,int& y)
{
	if(b != 0){
		extgcd(b,a%b,y,x);
		y -= (a/b)*x;
	}else{
		x = 1;
		y = 0;
	}
}

int mod_inverse(int a,int m)
{
	int x,y;
	extgcd(a,m,x,y);
	return (m + x % m) % m;
}

ll add(ll x,ll y)
{
    return (x + y)%MOD;
}

ll sub(ll x,ll y)
{
    return (x+MOD-y)%MOD;
}

ll mult(ll x,ll y)
{
    return x*y%MOD;
}
//degは多項式の次数、argは関数の引数(長さdeg+1),retは関数の返り値(長さdeg+1),numは求めたい返り値に対する引数
ll comp(int deg,ll num,vector<int>& arg,vector<ll>& ret){

    ll ue = 1;
    rep(i,deg+1){
        ue = mult(ue,num-arg[i]);
    }
    ll ans = 0;
    rep(i,deg+1){
        ll r1 = mult(ue,mult(mod_inverse(num-arg[i],MOD),ret[i]));
        ll r2 = mult(finv[deg-arg[i]],finv[arg[i]]);
        if((deg-arg[i])%2){
            r2 = MOD-r2;
        }
        ans = add(ans,mult(r1,r2));
    }
    return ans;
}

int main(){
    cin.tie(0);
    ios::sync_with_stdio(false);
    ll n,k;
    cin >> n >> k;
    if(n <= k+1){
        ll ans = 0;
        for(int i=1;i<=n;i++){
            ans = add(ans,mod_pow(i,k));
        }
        cout << ans << endl;
    }else{
        vector<ll> res(k+2,0);
        res[0] = 0;
        for(int i=1;i<=k+1;i++){
            res[i] = add(res[i-1],mod_pow(i,k));
        }
        make();
        vector<int> arg(k+2);
        rep(i,k+2){
            arg[i] = i;
        }
        cout << comp(k+1,n,arg,res) << endl;
    }
}
