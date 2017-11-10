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

template <typename T> class RBST {
public:
    struct node{
        T val;
        int st_size;   // 部分木のサイズ
        node* left; node* right;
        node(T v) : val(v), left(nullptr), right(nullptr), st_size(1){}
        ~node() { delete left; delete right; }
    };
    node *root;
    using pnn = pair<node*,node*>;
    RBST() : root(nullptr) {}
    ~RBST() {}
    int size(node* t) { return t ? t->st_size : 0; }
    node* update(node *t) {
        node* l = t->left; node* r = t->right;
        t->st_size = size(l) + size(r) + 1;
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
        update(l); update(r);
        if(rnd() % (size(l) + size(r)) < (unsigned)size(l)){
            l->right = merge(l->right, r);
            return update(l);
        }else{
            r->left = merge(l, r->left);
            return update(r);
        }
    }
    pnn split(node* t, int k){   //木のサイズが(k,n-k)となるように分割する
        if(!t) return pnn(nullptr, nullptr);
        update(t);
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
    node* insert(node* t, int k, node* u){
        pnn s = split(t, k);
        return merge(merge(s.first, u), s.second);
    }
    pnn erase(node* t, int k){
        pnn sr = split(t, k+1);
        pnn sl = split(sr.first, k);
        return pnn(merge(sl.first, sr.second), sl.second);
    }
    void insert(int k, T val){
        root = insert(root, k, new node(val));
    }
    void erase(int k) {
        node *p;
        tie(root, p) = erase(root, k);
        p->left = p->right = nullptr;
        delete p;
    }
    T get(int k){
        node* v = root;
        while(v) {
            int s = size(v->left);
            if(s > k){
                 v = v->left;
            }else if(s == k){
                return v->val;
            }else{
                v = v->right;
                k -= s+1;
            }
        }
        return v->val;
    }
    //k番目の値をvalに変更
    void set(int k, T val) {
        pnn sr = split(root, k+1);
        pnn sl = split(sr.first, k);
        pnn lr = sl.second;
        lr->val = val;
        root = merge(merge(sl.first, lr), sr.second);
    }
};
