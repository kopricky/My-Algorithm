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
typedef vector<vl> vvl;
typedef vector<double> vd;
typedef vector<P> vp;
typedef vector<string> vs;

const int MAX_N = 100005;

template<typename T> class segtree {
public:
	segtree(int n, T e) : E(e), N(1 << (int)log2(n * 2 - 1)) {}

	void update(int k, T val) {
		root = update(k, val, root, 0, N);
	}

	T query(int a, int b) {
		return query(a, b, root, 0, N);
	}

private:
	struct node {
		node *left = nullptr, *right = nullptr;
		T val;
	};
	const T E;
	const int N;

	node* root = nullptr;

	T get(node* x) {
		return x ? x->val : E;
	}
	T func(T arg1, T arg2)
	{
		return min(arg1,arg2);
	}
	node* update(int k, T val, node* x, int l, int r) {
		x = x ? new node(*x) : new node();
		if (r - l == 1) {
			x->val = val;
			return x;
		}
		int m = (l + r) / 2;
		if(k < m) {
			x->left = update(k, val, x->left, l, m);
		}else{
			x->right = update(k, val, x->right, m, r);
		}
		x->val = func(get(x->left), get(x->right));
		return x;
	}

	T query(int a, int b, node* x, int l, int r) {
		if (!x) return E;
		if (r <= a || b <= l) return E;
		if (a <= l && r <= b) return x->val;
		int m = (l + r) / 2;
		T vl = query(a, b, x->left, l, m);
		T vr = query(a, b, x->right, m, r);
		return func(vl, vr);
	}
};

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n;
    cin >> n;

    return 0;
}
