#include <bits/stdc++.h>
#define ll long long
#define INF 1000000005
#define MOD 1000000007
#define EPS 1e-10
#define rep(i,n) for(int i=0;i<(int)(n);++i)
#define rrep(i,n) for(int i=(int)(n)-1;i>=0;--i)
#define srep(i,s,t) for(int i=(int)(s);i<(int)(t);++i)
#define each(a,b) for(const auto& (a): (b))
#define all(v) (v).begin(),(v).end()
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
typedef pair<ll,ll> pll;
typedef TRI<int, int, int> tri;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<ll> vl;
typedef vector<vl> vvl;
typedef vector<double> vd;
typedef vector<P> vp;
typedef vector<string> vs;

const int MAX_N = 100005;

template<typename T> class Dinic {
private:
    int V;
    vector<int> level,iter;
    void bfs(int s){
        fill(level.begin(),level.end(),-1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while(!que.empty()){
            int v = que.front();
            que.pop();
            for(auto& e : G[v]){
                if(e.cap > 0 && level[e.to] < 0){
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }
    T dfs(int v,int t,T f) {
        if(v==t){
            return f;
        }
        for(int& i = iter[v]; i < (int)G[v].size(); i++){
            edge& e = G[v][i];
            if(e.cap > 0 && level[v] < level[e.to]){
                ashow(v, e.to);
                T d = dfs(e.to,t,min(f,e.cap));
                if(d > 0){
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

public:
    struct edge{
        int to, rev;
        T cap;
    };
    vector<vector<edge> > G;

    Dinic(const int node_size) : V(node_size), level(V), iter(V), G(V){}
    void clear(){ rep(i, V) G[i].clear(); }
    //辺を張る
    void add_edge(int from,int to,T cap) {
        G[from].push_back((edge){to, (int)G[to].size(), cap});
        G[to].push_back((edge){from, (int)G[from].size()-1, (T)0});
    }
    //最大流を計算
    T solve(int s,int t) {
        T flow = 0;
        for(;;){
            bfs(s);
            if(level[t] < 0) return flow;
            fill(iter.begin(),iter.end(),0);
            T f;
            while((f=dfs(s,t,numeric_limits<T>::max())) > 0){
                flow += f;
            }
        }
    }
};

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    int T = 1;
    // cin >> T;
    Dinic<int> dn(20);
    rep(t,T){
        int n, m, ans = 0;
        cin >> n;
        vi cnt(9, 0);
        string s;
        rep(i,n){
            cin >> s;
            m = len(s);
            rep(j,m){
              if(s[j] == 'A') s[j] = 'S';
              else if(s[j] == 'O') s[j] = 'A';
              else s[j] = 'D';
            }
            rep(j,m-2) ans += (s[j] == 'S' && s[j+1] == 'A' && s[j+2] == 'D');
            if(m == 1){
                if(s[0] == 'S') ++cnt[0];
                else if(s[0] == 'A') ++cnt[8];
                else ++cnt[2];
            }else{
                if(s[0] == 'D'){
                    if(s[m-1] == 'S') ++cnt[3];
                    else if(s[m-2] == 'S' && s[m-1] == 'A') ++cnt[4];
                    else ++cnt[2];
                }else if(s[0] == 'A' && s[1] == 'D'){
                    if(s[m-1] == 'S') ++cnt[6];
                    else if(s[m-2] == 'S' && s[m-1] == 'A') ++cnt[7];
                    else ++cnt[5];
                }else{
                    if(s[m-1] == 'S') ++cnt[0];
                    else if(s[m-2] == 'S' && s[m-1] == 'A') ++cnt[1];
                }
            }
        }
        int sm = accumulate(all(cnt), 0);
        dn.clear();
        rep(i,8) dn.add_edge(18, i, cnt[i]), dn.add_edge(i + 9, 19, cnt[i]);
        rep(i,3){
            srep(j,11,14){
                if(i == 1 && j == 12) dn.add_edge(3 * i, 8, cnt[3 * i] - 1), dn.add_edge(17, j, cnt[3 * i] - 1);
                else dn.add_edge(3 * i, 8, cnt[3 * i]), dn.add_edge(17, j, cnt[3 * i]);
            }
        }
        dn.add_edge(8, 17, cnt[8]);
        rep(i,3){
            srep(j,14,17){
                if(i == 2 && j == 15) dn.add_edge(3*i, j, cnt[3*i] - 1);
                else dn.add_edge(3*i, j, INF);
            }
        }
        rep(i,3){
            srep(j,11,14){
                if(i == 1 && j == 13) dn.add_edge(3*i+1, j, cnt[3*i+1] - 1);
                else dn.add_edge(3*i+1, j, INF);
            }
        }
        int res = dn.solve(18, 19);
        cout << ans + res - (sm > 0 && res == sm) << "\n";
        // cout << "Case #" << t + 1 << ": " << ans + res - (sm > 0 && res == sm) << "\n";
    }
    return 0;
}