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

typedef pair<int,int>P;

int main()
{
	cin.tie(0);
	ios::sync_with_stdio(false);
	int h,w;
	cin >> h >> w;
    vector<vector<int> > a(h,vector<int>(w,0));
    vector<vector<int> > t(h,vector<int>(w+1,0));
	rep(i,h){
		rep(j,w){
			cin >> a[i][j];
		}
	}
	rep(i,w){
		t[0][i] = 1^a[0][i];
	}
	for(int i = 1; i < h; i++){
		rep(j,w){
			if(a[i][j] == 0){
				t[i][j] = t[i-1][j] + 1;
			}else{
				t[i][j] = 0;
			}
		}
	}
    deque<P> deq;
	int ans = 0;
	rep(i,h){
		rep(j,w+1){
			if(deq.empty()){
				deq.push_back(P(t[i][j],j));
				continue;
			}
			P p = deq.back();
			if(p.first < t[i][j]){
				deq.push_back(P(t[i][j],j));
			}else if(p.first > t[i][j]){
				int pos;
				while(!deq.empty() && p.first > t[i][j]){
					ans = max(ans, p.first * (j - p.second));
                    pos = p.second;
					deq.pop_back();
					p = deq.back();
				}
				deq.push_back(P(t[i][j],pos));
			}
		}
	}
	cout << ans << "\n";
	return 0;
}
