#include <bits/stdc++.h>
#define ll long long
#define INF 1000000005
#define MOD 1000000007
#define EPS 1e-10
#define rep(i,n) for(int i=0;i<(int)(n);++i)
#define rrep(i,n) for(int i=(int)(n)-1;i>=0;--i)
#define srep(i,s,t) for(int i=(int)(s);i<(int)(t);++i)
#define each(a,b) for(auto (a): (b))
#define all(v) (v).begin(),(v).end()
#define len(v) (int)(v).size()
#define zip(v) sort(all(v)),v.erase(unique(all(v)),v.end())
#define cmx(x,y) x=max(x,y)
#define cmn(x,y) x=min(x,y)
#define fi first
#define se second
#define pb push_back
#define show(x) cout<<#x<<" = "<<(x)<<endl
#define spair(p) cout<<#p<<": "<<p.fi<<" "<<p.se<<endl
#define svec(v) cout<<#v<<":";rep(kbrni,v.size())cout<<" "<<v[kbrni];cout<<endl
#define sset(s) cout<<#s<<":";each(kbrni,s)cout<<" "<<kbrni;cout<<endl
#define smap(m) cout<<#m<<":";each(kbrni,m)cout<<" {"<<kbrni.first<<":"<<kbrni.second<<"}";cout<<endl

using namespace std;

typedef pair<int,int> P;
typedef pair<ll,ll> pll;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<ll> vl;
typedef vector<double> vd;
typedef vector<P> vp;
typedef vector<string> vs;

const int MAX_N = 100005;

struct Miller{
    const vector<ll> v = {2,3,5,7,11,13,17,19,23,29,31,37} //2^64までいける
    // x^k (mod m)
    ll modpow(ll x, ll k, ll m){
        ll res = 1;
        while(k){
          if(k & 1){
            res = res * x % m;
          }
          k /= 2;
          x = x * x % m;
        }
        return res;
    }
    // check if n is prime
    bool check(ll n){
        if(n < 2){
          return false;
        }
        ll d = n - 1;
        ll s = 0;
        while(d % 2 == 0){
            d /= 2;
            s++;
        }
        for(ll a : v){
            if(a == n){
              return true;
            }
            if(modpow(a,d,n) != 1){
              bool ok = true;
              for(ll r=0;r<s;r++){
                if( modpow(a,d*(1LL << r),n) == n-1){
                  ok = false;
                  break;
                }
              }
              if(ok){
                return false;
              }
            }
        }
        return true;
    }
};

ll count_devisor(ll N){
    ll res = 1;
	for(ll i=2;i*i*i<=N;i++){
		int cnt = 0;
		while(N%i == 0){
			cnt++;
			N /= i;
		}
        res *= (cnt+1);
	}
    Miller miller;
    if(N != 1){
        if(miller.check(N)){
            res *= 2;
        }else{
            int val = floor(sqrt(N));
            if(val*val == N){
                res *= 3;
            }else{
                res *= 4;
            }
        }
    }
	return res;
}

int main()
{
    ll n;
    cin >> n;
}
