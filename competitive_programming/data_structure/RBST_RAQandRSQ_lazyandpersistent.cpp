//永続化遅延処理平衡二分木
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

const int max_size = 30000000;

template <typename T> class RBST
{
public:
    struct node{
        T val, lazy, al;
        int st_size;   // 部分木のサイズ
        node* left; node* right;
        node(){};
        node(T v) : val(v), al(v), left(nullptr), right(nullptr), st_size(1), lazy(0){}
        ~node() { delete left; delete right; }
    };
    using pnn = pair<node*,node*>;
    node* memo; //永続配列
    int pool_size;
    node* root;
    T id1,id2;  //opr1,opr2の単位元
    //クエリに答える演算(sumに対応)
    T opr1(T l, T r) {
    	return l + r;
    }
    //遅延処理の演算(addに対応)
    T opr2(T l, T r) {
    	return l + r;
    }
    RBST(){
        memo = new node[max_size];
        id1 = id2 = 0;
    }
    //永続化
    node* fix(node* t){
        if(!t)  return t;
        if(pool_size >= max_size) assert(false);
        memo[pool_size] = *t;
        return &memo[pool_size++];
    }
    int size(node* t) { return t ? t->st_size : 0; }
    T que(node* t) { return t ? t->al + t->lazy*size(t) : id1; }
    //遅延伝播
    node* push(node* t){
        if(!t) return t;
        t = fix(t);
        if(t->left){
            t->left = fix(t->left);
    		t->left->lazy = opr2(t->left->lazy, t->lazy);
        }
        if(t->right){
            t->right = fix(t->right);
            t->right->lazy = opr2(t->right->lazy,t->lazy);
        }
        t->val = opr2(t->val, t->lazy);
        t->al = opr1(que(t->left), opr1(t->val, que(t->right)));
        t->lazy = id2;
        return t;
    }
    node* update(node *t){
        node* l = t->left; node* r = t->right;
        t->st_size = size(l) + size(r) + 1;
        t->al = que(l) + que(r) + t->val;
        return t;
    }
    unsigned rnd(){
        static unsigned x = 123456789, y = 362436069, z = 521288629, w = 86675123;
        unsigned t = (x^(x<<11));
        x = y,y = z,z = w;
        return (w = (w^(w>>19))^(t^(t>>8)));
    }
    node* merge(node* l, node* r){
        if (!l || !r) return (!l) ? r : l;
        if(rnd() % (size(l) + size(r)) < (unsigned)size(l)){
            l = push(l);
            l->right = merge(l->right, r);
            return update(l);
        }else{
            r = push(r);
            r->left = merge(l, r->left);
            return update(r);
        }
    }
    pnn split(node* t, int k){   //木のサイズが(k,n-k)となるように分割する
        if(!t) return pnn(nullptr, nullptr);
        t = push(t);
        if(k <= size(t->left)){
            pnn s = split(t->left, k);
            t->left = s.second;
            return pnn(s.first,update(t));
        }else{
            pnn s = split(t->right, k-size(t->left)-1);
            t->right = s.first;
            return pnn(update(t), s.second);
        }
    }
    vector<T> x;    //再構築の際にも用いる
    int node_size;
    int x_size;
    void build(){
        node_size = (int)x.size();
    	pool_size = 0;
    	node *res = nullptr;
    	for (int i = 0; i < node_size; i++) {
    		memo[pool_size] = node(x[i]);
    		res = merge(res, &memo[pool_size++]);
    	}
    	root = res;
    }
    void up(node *t) {
    	if (!t) return;
    	t = push(t);
    	up(t->left);
    	if(x_size >= node_size) return;
    	x[x_size++] = t->val;
    	up(t->right);
    }
    void rebuild(node* t) {
    	x_size = 0;
    	up(t);
        build();
    }
    void add(int l, int r, T val){
        auto sr = split(root, r);
        auto sl = split(sr.first, l);
        node* lr = sl.second;
        lr->lazy = val;
        root = merge(merge(sl.first,sl.second),sr.second);
    }
    void trans(int p, int q, int r, int s){
        auto sq = split(root,q);
        auto sp = split(sq.first,p);
        auto ss = split(root,s);
        auto sr = split(ss.first,r);
        root = merge(merge(sp.first,sr.second),sq.second);
    }
    T query(int l,int r){
        auto sr = split(root,r);
        auto sl = split(sr.first,l);
        T res = que(sl.second);
        return res;
    }
};

int main()
{
    int n,q;
    scanf("%d%d",&n,&q);
    RBST<ll> rb;
    rb.x.resize(n);
    rep(i,n){
        cin >> rb.x[i];
    }
    rb.build();
    rep(i,q){
        int a;
        scanf("%d",&a);
        if(a == 1){
            int b,c,d;
            scanf("%d%d%d",&b,&c,&d);
            rb.add(b-1,c,d);
        }else if(a == 2){
            int b,c,d,e;
            scanf("%d%d%d%d",&b,&c,&d,&e);
            rb.trans(b-1,c,d-1,e);
        }else{
            int b,c;
            scanf("%d%d",&b,&c);
            cout << rb.query(b-1,c) << "\n";
        }
        if(rb.pool_size >= max_size / 2){
            rb.rebuild(rb.root);
        }
    }
    return 0;
}
