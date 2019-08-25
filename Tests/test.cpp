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

#define getchar getchar_unlocked
#define putchar putchar_unlocked

inline int in() {
    int n = 0; short c;
    while ((c = getchar()) >= '0') n = n * 10 + c - '0';
    return n;
}

inline void out(int n) {
    short res[10], i = 0;
    do { res[i++] = n % 10, n /= 10; } while (n);
    while (i) putchar(res[--i] + '0');
    putchar('\n');
}

template<typename _Key, typename _Tp> class Fibonacci_Heap
{
public:
    class node
    {
    public:
        _Key _key;
        _Tp _data;
        unsigned short int _child;
        bool _mark;
        node *_par, *_prev, *_next, *_ch_last;
        node(const _Key& key, const _Tp& data) : _key(key), _data(data),
            _child(0), _par(nullptr), _prev(nullptr), _next(nullptr), _ch_last(nullptr){}
        node(const _Key& key, _Tp&& data) : _key(key), _data(move(data)),
            _child(0), _par(nullptr), _prev(nullptr), _next(nullptr), _ch_last(nullptr){}
        void insert(node *cur){
            if(_ch_last) insert_impl(cur, _ch_last);
            else _ch_last = cur, _ch_last->_prev = _ch_last->_next = _ch_last;
            ++_child, cur->_par = this;
        }
        void erase(node *cur){
            if(cur == cur->_next) _ch_last = nullptr;
            else erase_impl(cur);
            --_child, cur->_par = nullptr;
        }
    };

public:
    static constexpr float FACTOR = 1.0f/log2((1.0f+sqrt(5.0f))/2.0f);
    size_t _size;
    node *_minimum;

    // cur を next の前に追加
    static void insert_impl(node *cur, node *next){
        cur->_prev = next->_prev, cur->_next = next;
        cur->_prev->_next = cur, next->_prev = cur;
    }
    static void erase_impl(node *cur){
        cur->_prev->_next = cur->_next, cur->_next->_prev = cur->_prev;
    }
    static int ceil2(int u){
        return 32 - __builtin_clz(u);
    }
    void root_insert(node *cur){
        if(_minimum){
            insert_impl(cur, _minimum);
            if(cur->_key < _minimum->_key){
                _minimum = cur;
            }
        }else{
            _minimum = cur;
            _minimum->_prev = _minimum->_next = _minimum;
        }
    }
    void root_erase(node *cur){
        if(cur == cur->_next) _minimum = nullptr;
        else erase_impl(cur);
    }
    void _delete(node *cur){
        root_erase(cur);
        delete cur;
    }
    template<typename Data>
    node *_push(const _Key& key, Data&& data){
        ++_size;
        node* new_node = new node(key, forward<Data>(data));
        root_insert(new_node);
        return new_node;
    }
    void _pop(){
        assert(_size > 0);
        --_size;
        if(_size == 0){
            _delete(_minimum);
            return;
        }
        if(_minimum->_ch_last){
            for(auto cur = _minimum->_ch_last->_next;;){
                node *next = cur->_next;
                _minimum->erase(cur), root_insert(cur);
                if(!_minimum->_ch_last) break;
                cur = next;
            }
        }
        unsigned int max_rank = ceil(ceil2(_size) * FACTOR) + 1;
        node **array = new node*[max_rank]();
        node *next__minimum = _minimum->_next;
        for(node *cur = next__minimum; cur != _minimum;){
            if(cur->_key < next__minimum->_key) next__minimum = cur;
            node *next = cur->_next;
            unsigned int deg = cur->_child;
            while(array[deg]){
                if(cur->_key < array[deg]->_key){
                    root_erase(array[deg]), cur->insert(array[deg]);
                }else{
                    root_erase(cur), array[deg]->insert(cur);
                    cur = array[deg];
                }
                array[deg++] = nullptr;
                assert(deg < max_rank);
            }
            array[deg] = cur;
            cur = next;
        }
        delete[] array;
        _delete(_minimum);
        _minimum = next__minimum;
    }
    void _update(node *cur, const _Key& key){
        assert(key >= (_Key)0);
        node *change = (cur->_key -= key < _minimum->_key) ? cur : nullptr;
        if(!cur->_par || cur->_par->_key <= key) return;
        while(true){
            node *next = cur->_par;
            next->erase(cur), root_insert(cur);
            cur->_mark = false, cur = next;
            if(!cur->_par) break;
            if(!cur->_mark){
                cur->_mark = true;
                break;
            }
        }
        if(change) _minimum = change;
    }

public:
    Fibonacci_Heap() noexcept : _size(0u), _minimum(nullptr){}
    inline bool empty() const noexcept { return (_size == 0); }
    inline size_t size() const noexcept { return _size; }
    inline pair<const _Key&, const _Tp&> top() const noexcept {
        return make_pair(_minimum->_key, _minimum->_data);
    }
    node *push(const _Key& key, const _Tp& data){ return _push(key, data); }
    node *push(const _Key& key, _Tp&& data){ return _push(key, move(data)); }
    void pop(){ _pop(); }
    void update(node *cur, const _Key& key){ _update(cur, key); }
    friend Fibonacci_Heap *unite(Fibonacci_Heap *fh1, Fibonacci_Heap *fh2){
        if(fh2->_size == 0){
            fh2->~Fibonacci_Heap();
            return fh1;
        }
        if(fh1->size == 0){
            fh1->~Fibonacci_Heap();
            return fh2;
        }
        fh1->__minimum->_prev->_next = fh2->__minimum->_next;
        fh2->__minimum->_next->_prev = fh1->__minimum->_prev;
        fh2->__minimum->_next = fh1->__minimum;
        fh1->__minimum->_prev = fh2->__minimum;
        fh1->_size += fh2->_size;
        if(fh1->__minimum->_key > fh2->__minimum->_key) fh1->__minimum = fh2->__minimum;
        fh2->~Fibonacci_Heap();
        return fh1;
    }
};

template<typename T> class Dijkstra {
public:
	struct edge{
		int to; T cost;
	};
    const int V;
	vector<vector<edge> > G;
	vector<T> d;
	using pti = pair<T,int>;
    Fibonacci_Heap<T, int> fheap;
    typename Fibonacci_Heap<T, int>::node** keep;
	Dijkstra(int node_size) : V(node_size), G(V), d(V, numeric_limits<T>::max()),
        keep(new typename Fibonacci_Heap<T, int>::node*[V]){}
	//無向グラフの場合
	void add_edge(int u,int v,T cost){
		G[u].pb((edge){v,cost});
	}
	void solve(int s){
		d[s] = 0;
		keep[s] = fheap.push(0, s);
		while(!fheap.empty()){
			int v = fheap.top().second, val = fheap.top().first;
			fheap.pop();
			if(d[v] < val) continue;
			for(auto& w : G[v]){
				if(d[w.to] > d[v] + w.cost){
                    if(d[w.to] == numeric_limits<T>::max()){
                        keep[w.to] = fheap.push(d[v] + w.cost, w.to);
                    }else{
                        fheap.update(keep[w.to], d[w.to] - (d[v] + w.cost));
                    }
                    d[w.to] = d[v] + w.cost;
				}
			}
		}
	}
};

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n = in(), m = in(), s = in();
    Dijkstra<int> dj(n);
    rep(i,m){
        int a = in(), b = in(), c = in();
        dj.add_edge(a,b,c);
    }
    dj.solve(s);
    rep(i,n){
        if(dj.d[i] == numeric_limits<int>::max()){
            putchar('I'), putchar('N'), putchar('F'), putchar('\n');
        }else{
            out(dj.d[i]);
        }
    }
    return 0;
}
