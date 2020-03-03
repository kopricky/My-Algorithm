#include <bits/stdc++.h>
#define ll long long
#define INF 1000000005
#define MOD 1000000007
#define EPS 1e-10
#define rep(i,n) for(int i=0;i<(int)(n);++i)
#define rrep(i,n) for(int i=(int)(n)-1;i>=0;--i)
#define srep(i,s,t) for(int i=(int)(s);i<(int)(t);++i)
#define each(a,b) for(auto& (a): (b))
#define len(v) (int)(v).size()
#define zip(v) sort(all(v)),v.erase(unique(all(v)),v.end())
#define cmx(x,y) x=max(x,y)
#define cmn(x,y) x=min(x,y)
#define fi first
#define se second
#define pb push_back
#define show(x) cout<<#x<<" = "<<(x)<<endl
#define sar(a,n) {cout<<#a<<":";rep(pachico,n)cout<<" "<<a[pachico];cout<<endl;}

using namespace std;

template<typename S,typename T>auto&operator<<(ostream&o,pair<S,T>p){return o<<"{"<<p.fi<<","<<p.se<<"}";}
template<typename T>auto&operator<<(ostream&o,set<T>s){for(auto&e:s)o<<e<<" ";return o;}
template<typename S,typename T,typename U>
auto&operator<<(ostream&o,priority_queue<S,T,U>q){while(!q.empty())o<<q.top()<<" ",q.pop();return o;}
template<typename K,typename T>auto&operator<<(ostream&o,map<K,T>&m){for(auto&e:m)o<<e<<" ";return o;}
template<typename T>auto&operator<<(ostream&o,vector<T>v){for(auto&e:v)o<<e<<" ";return o;}
void ashow(){cout<<endl;}template<typename T,typename...A>void ashow(T t,A...a){cout<<t<<" ";ashow(a...);}
template<typename S,typename T,typename U>
struct TRI{S fi;T se;U th;TRI(){}TRI(S f,T s,U t):fi(f),se(s),th(t){}
bool operator<(const TRI&_)const{return(fi==_.fi)?((se==_.se)?(th<_.th):(se<_.se)):(fi<_.fi);}};
template<typename S,typename T,typename U>
auto&operator<<(ostream&o,TRI<S,T,U>&t){return o<<"{"<<t.fi<<","<<t.se<<","<<t.th<<"}";}

typedef pair<int, int> P;
typedef pair<ll, ll> pll;
typedef TRI<int, int, int> tri;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<vi> vvi;
typedef vector<vl> vvl;
typedef vector<P> vp;
typedef vector<double> vd;
typedef vector<string> vs;

const int MAX_N = 100005;

#define all(v) (v).begin(), (v).end()

template<typename CandidateType> class OrthogonalRangeReport
{
private:
    using CT = CandidateType;
    using pcc = pair<CT, CT>;
    using pci = pair<CT, int>;
    int n, sz;
    // x座標
    vector<CT> xs;
    // y座標, インデックス
    vector<vector<pci> > ys;
    // 位置
    vector<vector<vector<int> > > pos;
    static bool less(const pci& a, const pci& b, const vector<pcc>& cand){
        return (a.first == b.first) ? (cand[a.second].first < cand[b.second].first) : (a.first < b.first);
    }
    void query(const int lxid, const int rxid, const int lyid, const int ryid,
                vector<int>& report, const int k, const int l, const int r){
        if(lyid >= ryid || r <= lxid || rxid <= l) return;
        if(lxid <= l && r <= rxid){
            for(int i = lyid; i < ryid; ++i){
                report.push_back(ys[k][i].second);
            }
        }else{
            query(lxid, rxid, pos[k][lyid][0], pos[k][ryid][0], report, 2*k+1, l, (l+r)/2);
            query(lxid, rxid, pos[k][lyid][1], pos[k][ryid][1], report, 2*k+2, (l+r)/2, r);
        }
    }
public:
    OrthogonalRangeReport(const vector<pcc>& cand) : n(1), sz((int)cand.size()), xs(sz){
        while(n < sz) n *= 2;
        vector<pci> sorted(sz);
        for(int i = 0; i < sz; ++i){
            sorted[i] = {cand[i].first, i};
        }
        sort(sorted.begin(), sorted.end());
        ys.resize(2*n-1), pos.resize(n-1);
        for(int i = 0; i < sz; ++i){
            xs[i] = sorted[i].first;
            ys[i+n-1] = {make_pair(cand[sorted[i].second].second, sorted[i].second)};
        }
        for(int i = n-2; i >= 0; --i){
            ys[i].resize((int)ys[2*i+1].size() + (int)ys[2*i+2].size());
            merge(all(ys[2*i+1]), all(ys[2*i+2]), ys[i].begin(), [&](const pci& a, const pci& b){
                return (a.first == b.first) ? (cand[a.second].first < cand[b.second].first) : (a.first < b.first);
            });
            pos[i].resize((int)ys[i].size() + 1, vector<int>(2));
            for(int k = 0; k < 2; ++k){
                int p = 0;
                for(int j = 0; j < (int)ys[i].size(); ++j){
                    while(p < (int)ys[2*i+k+1].size() && less(ys[2*i+k+1][p], ys[i][j], cand)) ++p;
                    pos[i][j][k] = p;
                }
                pos[i][(int)ys[i].size()][k] = (int)ys[2*i+k+1].size();
            }
        }
    }
    // [lx,rx)×[ly,ry)の長方形領域の範囲内の点のインデックスを報告する
    void query(const CT lx, const CT rx, const CT ly, const CT ry, vector<int>& report) {
        const int lxid = lower_bound(all(xs), lx) - xs.begin();
        const int rxid = lower_bound(all(xs), rx) - xs.begin();
        const int lyid = lower_bound(all(ys[0]), pci(ly, -1)) - ys[0].begin();
        const int ryid = lower_bound(all(ys[0]), pci(ry, -1)) - ys[0].begin();
        if(lxid >= rxid) return;
        query(lxid, rxid, lyid, ryid, report, 0, 0, n);
    }
};

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n;
    cin >> n;
    vp vec(n);
    rep(i,n){
        cin >> vec[i].fi >> vec[i].se;
    }
    OrthogonalRangeReport<int> orr(vec);
    int q;
    cin >> q;
    rep(i,q){
        int a,b,c,d;
        cin >> a >> b >> c >> d;
        vector<int> res, ans;
        orr.query(a,b+1,c,d+1,res);
        each(it,res){
            ans.pb(it);
        }
        sort(all(ans));
        each(j,ans){
            cout << j << "\n";
        }
        cout << "\n";
    }
    return 0;
}