#include <bits/stdc++.h>
#define ll long long
#define INF 1000000005
#define MOD 1000000007
#define EPS 1e-10
#define rep(i,n) for(int i=0;i<(int)(n);++i)
#define rrep(i,n) for(int i=(int)(n)-1;i>=0;--i)
#define srep(i,s,t) for(int i=(int)(s);i<(int)(t);++i)
#define each(a,b) for(auto& (a): (b))
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
#define sar(a,n) cout<<#a<<":";rep(pachico,n)cout<<" "<<a[pachico];cout<<endl
#define svec(v) cout<<#v<<":";rep(pachico,v.size())cout<<" "<<v[pachico];cout<<endl
#define svecp(v) cout<<#v<<":";each(pachico,v)cout<<" {"<<pachico.first<<":"<<pachico.second<<"}";cout<<endl
#define sset(s) cout<<#s<<":";each(pachico,s)cout<<" "<<pachico;cout<<endl
#define smap(m) cout<<#m<<":";each(pachico,m)cout<<" {"<<pachico.first<<":"<<pachico.second<<"}";cout<<endl

using namespace std;

const int MAX_N = 1005;

int c[MAX_N][MAX_N];

int main()
{
	string X,Y;
	cin >> X >> Y;
	int n = (int)X.length(), m = (int)Y.length();
	for(int i = 0; i <= n; i++){
		c[i][0] = i;
	}
	for(int j = 0; j <= m; j++){
		c[0][j] = j;
	}
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= m; j++){
			int p,q,r;
			if(X[i-1] == Y[j-1]){
				p = c[i-1][j-1];
			}else{
				p = c[i-1][j-1] + 1;
			}
			q = c[i][j-1] + 1;
			r = c[i-1][j] + 1;
			c[i][j] = min({p,q,r});
		}
	}
	cout << c[n][m] << "\n";
}
