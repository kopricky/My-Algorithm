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

template<typename _Tp> class node {
public:
    int sz;
    _Tp val, al, lazy;
    node *left, *right, *par;
    node(_Tp _val) noexcept : sz(1), val(_val), al(val), lazy(id1),
        left(nullptr), right(nullptr), par(nullptr){}
    static const _Tp id1 = (_Tp)0;
    static const _Tp id2 = (_Tp)0;
    static void opr1(_Tp& arg1, const _Tp arg2){ arg1 += arg2; }
    static _Tp opr2(const _Tp arg1, const _Tp arg2){ return arg1 + arg2; }
    inline bool isRoot() const { return !par; }
    void push(){
        if(lazy != id1){
            opr1(val, lazy), al += lazy * sz;
            if(left) opr1(left->lazy, lazy);
            if(right) opr1(right->lazy, lazy);
            lazy = id1;
        }
    }
    void eval(){
        sz = 1, al = val;
        if(left) left->push(), sz += left->sz, al = opr2(left->al, al);
        if(right) right->push(), sz += right->sz, al = opr2(al, right->al);
    }
    void rotate(bool right_){
        node<_Tp> *p = par, *g = p->par;
        if(right_){
            if((p->left = right)) right->par = p;
            right = p, p->par = this;
        }else{
            if((p->right = left)) left->par = p;
            left = p, p->par = this;
        }
        p->eval(), eval();
        if(!(par = g)) return;
        if(g->left == p) g->left = this;
        if(g->right == p) g->right = this;
        g->eval();
    }
};

template<typename _Tp>
int size(node<_Tp>* u){ return u ? u->sz : 0; }

// 頂点 u を木の根にする
template<typename _Tp>
node<_Tp>* splay(node<_Tp>* u) noexcept {
    if(!u) return nullptr;
    while(!(u->isRoot())){
        node<_Tp> *p = u->par, *gp = p->par;
        if(p->isRoot()){
            p->push(), u->push();
            u->rotate((u == p->left));
        }else{
            gp->push(), p->push(), u->push();
            bool flag = (u == p->left);
            if((u == p->left) == (p == gp->left)){
                p->rotate(flag), u->rotate(flag);
            }else{
                u->rotate(flag), u->rotate(!flag);
            }
        }
    }
    u->push();
    return u;
}

// root を根とする木の頂点で k(0-indexed) 番目のものを探索する
// 返り値は (木の根, k 番目が存在するかしないか(0/1))
template<typename _Tp>
pair<node<_Tp>*, bool> get(const int k, node<_Tp>* root) noexcept {
    if(size(root) <= k) return make_pair(root, false);
    int sum = k + 1;
    node<_Tp> *cur = nullptr, *nx = root;
    while(nx){
        cur = nx, cur->push();
        if(size(cur->left) < sum - 1){
            nx = cur->right, sum -= size(cur->left) + 1;
        }else if(size(cur->left) >= sum){
            nx = cur->left;
        }else{
            return make_pair(splay(cur), true);
        }
    }
}

template<typename _Tp>
node<_Tp>* join(node<_Tp>* root1, node<_Tp>* root2) noexcept {
    if(!root1 || !root2) return root1 ? root1 : root2;
    node<_Tp> *cur = nullptr, *nx = root1;
    while(nx) cur = nx, cur->push(), nx = cur->right;
    node<_Tp>* ver = splay(cur);
    ver->right = root2, ver->eval(), root2->par = ver;
    return ver;
}

// 添字での split ([0, ..., k) と [k, ..., n)) (0-indexed)
template<typename _Tp>
pair<node<_Tp>*, node<_Tp>*> split(const int k, node<_Tp>* root) noexcept {
    if(size(root) <= k) return make_pair(root, nullptr);
    int sum = k + 1;
    node<_Tp> *cur = nullptr, *nx = root;
    while(nx){
        cur = nx, cur->push();
        if(size(cur->left) < sum - 1){
            nx = cur->right, sum -= size(cur->left) + 1;
        }else if(size(cur->left) >= sum){
            nx = cur->left;
        }else{
            node<_Tp> *res = cur->left;
            if(res) cur->left->par = nullptr, cur->left = nullptr;
            cur->eval();
            return make_pair(res, cur);
        }
    }
}

// (target 未満) と (target 以上) に split する
template<typename _Tp>
pair<node<_Tp>*, node<_Tp>* > split_lower_bound(node<_Tp>* target) noexcept {
    node<_Tp> *res = splay(target)->left;
    if(res) target->left->par = nullptr, target->left = nullptr;
    target->eval();
    return make_pair(res, target);
}

// (target 以下) と (target より大) に split する
template<typename _Tp>
pair<node<_Tp>*, node<_Tp>* > split_upper_bound(node<_Tp>* target) noexcept {
    node<_Tp> *res = splay(target)->right;
    if(res) target->right->par = nullptr, target->right = nullptr;
    target->eval();
    return make_pair(target, res);
}

// 配列 nodes に対応する splay 木を完全二分木の形で構築する
template<typename _Tp>
node<_Tp> *bst_build(vector<node<_Tp>* >& nodes) noexcept {
    int i, n = (int)nodes.size(), st = 2, isolate = ((n % 4 == 1) ? (n-1) : -1);
    while(st <= n){
        for(i = st-1; i < n; i += 2*st){
            nodes[i]->left = nodes[i-st/2], nodes[i-st/2]->par = nodes[i];
            if(i+st/2 < n) nodes[i]->right = nodes[i+st/2], nodes[i+st/2]->par = nodes[i];
            else if(isolate >= 0) nodes[i]->right = nodes[isolate], nodes[isolate]->par = nodes[i];
            nodes[i]->eval();
        }
        isolate = ((n % (4*st) >= st && (n % (4*st) < 2*st)) ? (i-2*st): isolate);
        st <<= 1;
    }
    return nodes[st / 2 - 1];
}