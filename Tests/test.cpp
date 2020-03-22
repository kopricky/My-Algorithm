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

class Stack {
private:
    const int N, H;
    int sz;
    vector<int> node;
public:
    Stack(const int _N, const int _H) : N(_N), H(_H), node(N+H){ clear(); }
    bool empty() const { return sz == 0; }
    bool empty(const int h) const { return node[N+h] == N+h; }
    int top(const int h) const { return node[N+h]; }
    void pop(const int h){ --sz, node[N+h] = node[node[N+h]]; }
    void push(const int h, const int u){ ++sz, node[u] = node[N+h], node[N+h] = u; }
    void clear(){ sz = 0, iota(node.begin() + N, node.end(), N); }
};

#define base 3

template <typename T> class PushRelabel {
public:
    static_assert(std::is_integral<T>::value, "Integral required.");
    struct edge {
        const int to, rev;
        T cap;
        edge(const int _to, const int _rev, const T _cap) : to(_to), rev(_rev), cap(_cap){}
    };
private:
    const int V;
    int s, t, checker;
    vector<T> excess;
    vector<int> potential, cur_edge, que;
    Stack act_ver;
    static unsigned long long ceil2(unsigned long long v){
        --v;
        v = v | (v >> 1), v = v | (v >> 2);
        v = v | (v >> 4), v = v | (v >> 8);
        v = v | (v >> 16), v = v | (v >> 32);
        return ++v;
    }
    int calc_active(const T delta){
        int pot_min = V;
        for(int i = 0; i < V; ++i){
            if(potential[i] < V){
                if(excess[i] >= delta && i != t){
                    act_ver.push(potential[i], i);
                    pot_min = min(pot_min, potential[i]);
                }
            }else{
                potential[i] = V + 1;
            }
        }
        return pot_min;
    }
    void bfs(){
        for(int i = 0; i < V; ++i) potential[i] = max(potential[i], V);
        potential[t] = 0;
        int qh = 0, qt = 0;
        for(que[qt++] = t; qh++ < qt;){
            int u = que[qh - 1];
            for(const edge& e : G[u]){
                if(potential[e.to] == V && G[e.to][e.rev].cap > 0){
                    potential[e.to] = potential[u] + 1, que[qt++] = e.to;
                }
            }
        }
    }
    T init(){
        T mx = 0;
        potential[s] = V + 1;
        bfs();
        for(edge& e : G[s]){
            if(potential[e.to] < V){
                G[e.to][e.rev].cap = e.cap, excess[s] -= e.cap, excess[e.to] += e.cap;
                mx = max(mx, e.cap), e.cap = 0;
            }
        }
        return mx;
    }
    int global_relabel(const T delta){
        bfs();
        act_ver.clear();
        return calc_active(delta);
    }
    int discharge(const int u, const T delta){
        for(int& i = cur_edge[u]; i < (int)G[u].size(); ++i){
            edge& e = G[u][i];
            if(potential[u] == potential[e.to] + 1 && e.cap > 0){
                return push(u, e, delta) ? potential[e.to] : potential[u];
            }
        }
        return relabel(u);
    }
    bool push(const int u, edge& e, const T delta){
        const int v = e.to;
        T f = min(e.cap, excess[u]);
        if(v != t) f = min(f, base * delta - 1 - excess[v]);
        e.cap -= f, excess[u] -= f;
        G[v][e.rev].cap += f, excess[v] += f;
        if(excess[u] >= delta) act_ver.push(potential[u], u);
        if(excess[v] >= delta && v != t){
            act_ver.push(potential[v], v);
            return true;
        }else{
            return false;
        }
    }
    int relabel(const int u){
        ++checker;
        int prv = potential[u], cur = V;
        for(int i = 0; i < (int)G[u].size(); ++i){
            const edge& e = G[u][i];
            if(cur > potential[e.to] + 1 && e.cap > 0){
                cur_edge[u] = i, cur = potential[e.to] + 1;
            }
        }
        if((potential[u] = cur) == V) return potential[u] = V + 1, prv;
        act_ver.push(cur, u);
        return prv;
    }

public:
    vector<vector<edge> > G;
    PushRelabel(const int node_size)
        : V(node_size), checker(0), excess(V, (T)0),
            potential(V, 0), cur_edge(V), que(V), act_ver(V, V), G(V){}
    void add_edge(const int _from, const int _to, const T _cap){
        G[_from].emplace_back(_to, (int)G[_to].size(), _cap);
        G[_to].emplace_back(_from, (int)G[_from].size() - 1, 0);
    }
    T solve(const int source, const int sink){
        s = source, t = sink;
        const T U = init();
        T delta = 1;
        while(delta <= U) delta *= base;
        delta /= base;
        int level = calc_active(delta);
        while(delta > 0){
            while(!act_ver.empty()){
                if(act_ver.empty(level)){
                    ++level;
                    continue;
                }
                const int u = act_ver.top(level);
                act_ver.pop(level);
                level = discharge(u, delta);
                if(checker >= V / 2){
                    level = global_relabel(delta);
                    checker = 0;
                }
            }
            if(delta == 1) break;
            delta = max(delta / base, 1), level = calc_active(delta);
        }
        return excess[t];
    }
};

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    PushRelabel<int> pr(n);
    rep(i,m){
        int a,b,c;
        cin >> a >> b >> c;
        pr.add_edge(a, b, c);
    }
    cout << pr.solve(0, n-1) << "\n";
    return 0;
}
