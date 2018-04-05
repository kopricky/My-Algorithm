ll mod_pow(ll a, ll b, ll m)
{
    a %= m;
    ll res = 1;
    while(b){
        if(b & 1){
            res = res * a % m;
        }
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

void extgcd(ll a, ll b, ll& x, ll& y)
{
	if(b != 0){
		extgcd(b,a%b,y,x);
		y -= (a/b)*x;
	}else{
		x = 1;
		y = 0;
	}
}

ll mod_inverse(ll a, ll m)
{
    ll x,y;
	extgcd(a,m,x,y);
	return (m + x % m) % m;
}

//g^x ≡ y (mod p) の解xを求める (p^(1/2)logp)
ll BabyStep_GiantStep(ll g, ll y, ll p)
{
    ll m = floor(sqrt(p));
    map<ll,ll> mp;
    ll val = 1;
    rep(i,m){
        mp[val] = i;
        val = val * g % p;
    }
    ll invgm = mod_pow(mod_inverse(g,p), m, p);
    ll giant = y;
    rep(i,m){
        if(mp.find(giant) != mp.end()){
            return i*m + mp[giant];
        }else{
            giant = giant * invgm % p;
        }
    }
    return -1;
}
