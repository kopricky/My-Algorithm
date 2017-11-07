//どこかでmake()することを忘れずに
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

ll comb(int a,int b)
{
	if(a<b){
		return 0;
	}
	return fac[a] * (finv[b] * finv[a-b] % MOD) % MOD;
}

ll add(ll x,ll y)
{
    return (x + y)%MOD;
}

ll sub(ll x,ll y)
{
    return (x+MOD-y)%MOD;
}

ll mul(ll x,ll y)
{
    return x*y%MOD;
}
