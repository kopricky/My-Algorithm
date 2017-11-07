// MODが小さい値のときのみ有効
const int MAX_P = 10007;

int fact[MAX_P];

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

int mod_fact(int n,int p,int& e)
{
    e = 0;
    if(n == 0){
        return 1;
    }
    int res = mod_fact(n/p,p,e);
    e += n / p;
    if(n / p % 2 != 0){
        return res * (p - fact[n%p]) % p;
    }
    return res * fact[n%p] % p;
}

int mod_comb(int n,int k,int p)
{
    int e1,e2,e3;
    int a1 = mod_fact(n,p,e1),a2 = mod_fact(k,p,e2),a3 = mod_fact(n-k,p,e3);
    if(e1 > e2 + e3){
        return 0;
    }
    return a1 * mod_inverse(a2*a3%p,p) % p;
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
