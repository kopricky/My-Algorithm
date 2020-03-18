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

template<typename T> class LinkCutTree {
public:
    struct node {
        pair<T, int> val, al;
        T lazy;
        node *left, *right, *par;
        node() : val((T)0, -1), al(val), lazy(e1),
            left(nullptr), right(nullptr), par(nullptr){}
        void set(const T value, const int index){
            val = al = {value, index};
        }
        void reset(){
            left = right = par = nullptr;
        }
        bool isRoot() const {
            return (!par) || (par->left != this && par->right != this);
        }
        void push(){
            if(lazy != e1){
                val.first += lazy, al.first += lazy;
                if(left) left->lazy += lazy;
                if(right) right->lazy += lazy;
                lazy = e1;
            }
        }
        void eval(){
            al = val;
            if(left){
                left->push();
                if((left->al).first <= al.first) al = left->al;
            }
            if(right){
                right->push();
                if((right->al).first < al.first) al = right->al;
            }
        }
    };
    static const T e1 = (T)0;

private:
    void rotate(node* u, bool right){
        node *p = u->par, *g = p->par;
        if(right){
            if((p->left = u->right)) u->right->par = p;
            u->right = p, p->par = u;
        }else{
            if((p->right = u->left)) u->left->par = p;
            u->left = p, p->par = u;
        }
        p->eval(), u->eval(), u->par = g;
        if(!g) return;
        if(g->left == p) g->left = u;
        if(g->right == p) g->right = u;
        g->eval();
    }
    void splay(node* u){
        while(!(u->isRoot())){
            node *p = u->par, *gp = p->par;
            if(p->isRoot()){
                p->push(), u->push();
                rotate(u, (u == p->left));
            }else{
                gp->push(), p->push(), u->push();
                bool flag = (u == p->left);
                if((u == p->left) == (p == gp->left)){
                    rotate(p, flag), rotate(u, flag);
                }else{ // zig-zag
                    rotate(u, flag), rotate(u, !flag);
                }
            }
        }
        u->push();
    }
    node* access(node* u){
        node* last = nullptr;
        for(node* v = u; v; v = v->par){
            splay(v);
            if(last) v->right = last;
            v->eval();
            last = v;
        }
        splay(u);
        return u;
    }
    void update_dfs(node* u, auto& G, const vector<int>& iter, vector<int>& used){
        u->push();
        if(u->left) update_dfs(u->left, G, iter, used);
        if(u->right) update_dfs(u->right, G, iter, used);
        const int id = (u->val).second;
        if(used[id] != 1) return;
        assert(iter[id] < (int)G[id].size());
        auto& e = G[id][iter[id]];
        G[e.to][e.rev].rev_cap += e.rev_cap - (u->val).first;
        e.rev_cap = (u->val).first;
        used[id] = 0;
    }
    bool connected(node* u, node* v){
        access(u), access(v);
        return u == v || u->par;
    }
    void link(node* u, node* v){
        access(u), u->par = v;
    }
    void cut(node* u){
        access(u);
        if(!u->left) return;
        u->left->par = nullptr, u->left = nullptr, u->eval();
    }
    void update(node* u, const pair<T, int>& x){
        access(u), u->val = x, u->eval();
    }
    int prev(node* u){
        access(u);
        (u = u->left)->push();
        while(u->right){
            u = u->right;
            u->push();
        }
        return (access(u)->val).second;
    }
    int top(node* u){
        while(u->right){
            u = u->right;
            u->push();
        }
        return (access(u)->val).second;
    }

public:
    const int V;
    node** arr;
    LinkCutTree(const int _V) : V(_V){
        arr = new node*[V];
        for(int i = 0; i < V; ++i){
            arr[i] = new node();
        }
    }
    // ~LinkCutTree(){
    //     for(int i = 0; i < V; i++){
    //         delete arr[i];
    //     }
    //     delete[] arr;
    // }
    int bst_build(const vector<int>& index){
        int i = 2, n = (int)index.size(), st = 2, isolate = ((n%4==1) ? (n-1) : -1);
        while(st <= n){
            for(i = st-1; i < n; i += 2*st){
                arr[index[i]]->left = arr[index[i-st/2]], arr[index[i-st/2]]->par = arr[index[i]];
                if(i+st/2 < n){
                    arr[index[i]]->right = arr[index[i+st/2]], arr[index[i+st/2]]->par = arr[index[i]];
                }else if(isolate >= 0){
                    arr[index[i]]->right = arr[index[isolate]], arr[index[isolate]]->par = arr[index[i]];
                }
                arr[index[i]]->eval();
            }
            isolate = ((n % (4*st) >= st && (n % (4*st) < 2*st)) ? (i-2*st) : isolate);
            st <<= 1;
        }
        return index[i - st];
    }
    void update_and_clear(auto& G, const vector<int>& iter, vector<int>& used){
        for(int i = 0; i < V; ++i){
            if(used[i] == 1) update_dfs(arr[i], G, iter, used);
            arr[i]->reset();
        }
    }
    bool connected(const int id1, const int id2){ return connected(arr[id1], arr[id2]); }
    void link(const int id1, const int id2){ return link(arr[id1], arr[id2]); }
    void cut(const int id){ return cut(arr[id]); }
    void update(const int id, const T x){ return update(arr[id], {x, id}); }
    int prev(const int id){ return prev(arr[id]); }
    int top(const int id){ return top(arr[id]);}
    void range(const int id, const T x){ arr[id]->lazy += x, arr[id]->push(); }
    pair<T, int> query(const int id){ return access(arr[id])->al; }
};

// used[v](v から出る辺について) 初期 -1, 行きに辿ると 0, 帰りに辿ると 1, update すると 0 にする

template<typename T> class Dinic {
private:
    const int V;
    bool *is_new;
    vector<int> que, level, iter, used;
    LinkCutTree<T> lct;
    void bfs(const int s, const int t){
        level[s] = 0;
        int qh = 0, qt = 0;
        for(que[qt++] = s; qh < qt;){
            int v = que[qh++];
            for(const edge& e : G[v]){
                if(level[e.to] < 0 && G[e.to][e.rev].rev_cap > 0){
                    level[e.to] = level[v] + 1, que[qt++] = e.to;
                }
            }
        }
    }
    T block_flow(int cur, const int s, const int t){
        T flow = 0;
        lct.update(s, numeric_limits<T>::max());
        bool new_edge = true;
        while(true){
            used[cur] = 1;
            if(cur == s){
                const pair<T, int> res = lct.query(s);
                flow += res.first;
                lct.range(s, -res.first);
                cur = res.second;
                edge& e = G[cur][iter[cur]++];
                is_new[cur] = true;
                new_edge = true;
                G[e.to][e.rev].rev_cap += e.rev_cap, e.rev_cap = 0;
                lct.cut(e.to);
            }
            bool update = false;
            for(int& i = iter[cur]; i < (int)G[cur].size(); ++i, is_new[cur] = true){
                edge& e = G[cur][i];
                if(e.rev_cap > 0 && level[e.to] < level[cur] && used[e.to] >= 0){
                    update = true;
                    if(new_edge) lct.update(cur, e.rev_cap);
                    new_edge = false;
                    is_new[cur] = false;
                    if(used[e.to] == 0){
                        lct.link(e.to, cur), cur = e.to, new_edge = true;
                    }else if(used[e.to] == 1){
                        lct.cut(e.to), lct.link(e.to, cur);
                        const pair<T, int> res = lct.query(cur = e.to);
                        if(res.first == 0){
                            cur = res.second;
                            edge& e2 = G[cur][iter[cur]++];
                            is_new[cur] = true;
                            new_edge = true;
                            G[e2.to][e2.rev].rev_cap += e2.rev_cap, e2.rev_cap = 0;
                            lct.cut(e2.to);
                        }else{
                            cur = lct.top(cur), new_edge = false;
                        }
                    }
                    break;
                }
            }
            if(update) continue;
            used[cur] = -1;
            if(cur == t) break;
            const int new_cur = lct.prev(cur);
            lct.cut(cur), cur = new_cur;
            // if(cur == 1) cout << "AD" << endl;
            edge& e = G[cur][iter[cur]++];
            is_new[cur] = true;
            new_edge = true;
            const T nw = (lct.arr[cur]->val).first;
            G[e.to][e.rev].rev_cap += e.rev_cap - nw, e.rev_cap = nw;
        }
        used[s] = -1;
        return flow;
    }
public:
    struct edge {
        const int to, rev;
        T rev_cap;
        edge(const int _to, const int _rev, const T _rev_cap) :
            to(_to), rev(_rev), rev_cap(_rev_cap){}
    };
    vector<vector<edge> > G;
    Dinic(const int node_size)
        : V(node_size), is_new(new bool[V]), que(V), level(V), iter(V), used(V), lct(V), G(V){}
    void add_edge(const int from, const int to, const T cap){
        G[from].emplace_back(to, (int)G[to].size(), (T)0);
        G[to].emplace_back(from, (int)G[from].size() - 1, cap);
    }
    T solve(const int s, const int t){
        T flow = 0;
        while(true){
            fill(level.begin(), level.end(), -1);
            bfs(s, t);
            if(level[t] < 0) break;
            fill(is_new, is_new + V, true);
            fill(iter.begin(), iter.end(), 0);
            fill(used.begin(), used.end(), 0);
            flow += block_flow(t, s, t);
            // show(level[t]);
            // show(flow);
            // show(iter[1]);
            // show(used[1]);
            // show(lct.arr[1]->val);
            lct.update_and_clear(G, iter, used);
            // show(level);
            // rep(i,V){
                // for(const edge& e : G[i]){
                    // if(level[e.to] < level[i]){
                        // cout << e.to << " " << i << " " << e.rev_cap << endl;
                    // }
                // }
            // }
            // for(const edge& e : G[t]){
            //     // ashow(e.to, level[e.to]);
            //     if(level[e.to] < level[t]){
            //         cout << e.to << " " << e.rev_cap << endl;
            //     }
            //     // assert(e.rev_cap < 100);
            // }
        }
        return flow;
    }
};

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    Dinic<int> dn(n);
    rep(i,m){
        int a,b,c;
        cin >> a >> b >> c;
        dn.add_edge(a, b, c);
    }
    cout << dn.solve(0, n-1) << "\n";
    return 0;
}
