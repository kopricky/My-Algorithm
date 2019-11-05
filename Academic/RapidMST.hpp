#include "./header.hpp"

template<typename _Key, typename _Tp> class FibonacciHeap
{
public:
    using data_type = pair<_Key, _Tp>;
    class node
    {
    public:
        data_type _data;
        unsigned short int _child;
        bool _mark;
        node *_par, *_prev, *_next, *_ch_last;
        node(data_type&& data) : _data(move(data)), _child(0), _mark(false),
            _par(nullptr), _prev(nullptr), _next(nullptr), _ch_last(nullptr){}
        inline const _Key& get_key() const noexcept { return _data.first; }
        void insert(node *cur){
            if(_ch_last) insert_impl(cur, _ch_last);
            else _ch_last = cur, _ch_last->_prev = _ch_last->_next = _ch_last;
            ++_child, cur->_par = this;
        }
        void erase(node *cur){
            if(cur == cur->_prev){
                _ch_last = nullptr;
            }else{
                erase_impl(cur);
                if(cur == _ch_last) _ch_last = cur->_prev;
            }
            --_child, cur->_par = nullptr;
        }
    };

private:
    size_t _size;
    node *_minimum;
    vector<node*> rank;

    static void insert_impl(node *cur, node *next){
        cur->_prev = next->_prev, cur->_next = next;
        cur->_prev->_next = cur, next->_prev = cur;
    }
    static void erase_impl(node *cur){
        cur->_prev->_next = cur->_next, cur->_next->_prev = cur->_prev;
    }
    void root_insert(node *cur){
        if(_minimum){
            insert_impl(cur, _minimum);
            if(cur->get_key() < _minimum->get_key()) _minimum = cur;
        }else{
            _minimum = cur, _minimum->_prev = _minimum->_next = _minimum;
        }
    }
    void root_erase(node *cur){
        if(cur == cur->_prev) _minimum = nullptr;
        else erase_impl(cur);
    }
    void _delete(node *cur){
        root_erase(cur);
        delete cur;
    }
    template<typename Key, typename Data>
    node *_push(Key&& key, Data&& data){
        ++_size;
        data_type new_data(forward<Key>(key), forward<Data>(data));
        node* new_node = new node(move(new_data));
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
            for(node *cur = _minimum->_ch_last->_next;;){
                node *next = cur->_next;
                _minimum->erase(cur), root_insert(cur);
                if(!_minimum->_ch_last) break;
                cur = next;
            }
        }
        node *next_minimum = _minimum->_next;
        for(node*& cur : rank) cur = nullptr;
        for(node *cur = next_minimum; cur != _minimum;){
            if(cur->get_key() < next_minimum->get_key()) next_minimum = cur;
            node *next = cur->_next;
            unsigned int deg = cur->_child;
            if(rank.size() <= deg) rank.resize(deg + 1, nullptr);
            while(rank[deg]){
                if(cur->get_key() < rank[deg]->get_key() || cur == next_minimum){
                    root_erase(rank[deg]), cur->insert(rank[deg]);
                }else{
                    root_erase(cur), rank[deg]->insert(cur);
                    cur = rank[deg];
                }
                rank[deg++] = nullptr;
                if(rank.size() <= deg) rank.resize(deg + 1, nullptr);
            }
            rank[deg] = cur;
            cur = next;
        }
        _delete(_minimum);
        _minimum = next_minimum;
    }
    void _decrease_key(node *cur, const _Key& key){
        assert(!(key < (_Key)0));
        node *change = ((cur->_data.first -= key) < _minimum->get_key()) ? cur : nullptr;
        if(!cur->_par || !(cur->get_key() < cur->_par->get_key())){
            if(change) _minimum = change;
            return;
        }
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
    void clear_dfs(node *cur){
        if(cur->_ch_last){
            for(node *_cur = cur->_ch_last->_next;;){
                node *next = _cur->_next;
                if(_cur == cur->_ch_last){
                    clear_dfs(_cur);
                    break;
                }else{
                    clear_dfs(_cur);
                }
                _cur = next;
            }
        }
        delete cur;
        return;
    }
    void _clear(){
        if(!_minimum) return;
        for(node *cur = _minimum->_next;;){
            node *next = cur->_next;
            if(cur == _minimum){
                clear_dfs(cur);
                break;
            }else{
                clear_dfs(cur);
            }
            cur = next;
        }
    }

public:
    FibonacciHeap() noexcept : _size(0u), _minimum(nullptr){}
    // ~FibonacciHeap(){ _clear(); }
    inline bool empty() const noexcept { return (_size == 0); }
    inline size_t size() const noexcept { return _size; }
    inline const data_type& top() const noexcept { return _minimum->_data; }
    template<typename Key, typename Data>
    node *push(Key&& key, Data&& data){ return _push(forward<Key>(key), forward<Data>(data)); }
    void pop(){ _pop(); }
    void decrease_key(node *cur, const _Key& key){ _decrease_key(cur, key); }
    void clear(){ _clear(); _size = 0; rank.~vector<node*>(); }
};

class UnionFind {
private:
    int sz;
    vector<int> par, nrank;
public:
    UnionFind(){}
    UnionFind(int node_size) : sz(node_size), par(sz), nrank(sz, 0){
        iota(par.begin(), par.end(), 0);
    }
    int find(int x){
        if(par[x] == x) return x;
        else return par[x] = find(par[x]);
    }
    int unite(int x, int y){
    	if(nrank[x] < nrank[y]) swap(x,y);
        par[y] = x;
        if(nrank[x] == nrank[y]) nrank[x]++;
        return x;
    }
};

template<typename T> class RapidMST {
private:
    struct edge {
        int to;
        T cost;
        typename list<edge>::iterator rev;
        edge(const int _to, const T _cost)
            : to(_to), cost(_cost){}
        edge(const int _to, const T _cost, const typename list<edge>::iterator _rev)
            : to(_to), cost(_cost), rev(_rev){}
    };
    struct info {
        T cost;
        typename list<edge>::iterator itr;
        info(){}
        info(const T _cost) : cost(_cost){}
        info(const T _cost, const typename list<edge>::iterator _itr) : cost(_cost), itr(_itr){}
    };
    const int V;
    const T inf;
    UnionFind uf;
    vector<list<edge> > G;
    void cleanup(vector<int>& rem_node, vector<info>& check){
        vector<int> tmp;
        for(int v : rem_node){
            for(auto it = G[v].begin(); it != G[v].end();){
                const edge& e = *it;
                const int p = uf.find(e.to);
                if(p == v){
                    it = G[v].erase(it);
                    continue;
                }
                if(e.cost < check[p].cost){
                    if(check[p].cost == inf) tmp.push_back(p);
                    else G[p].erase(check[p].itr->rev), G[v].erase(check[p].itr);
                    check[p].cost = e.cost, check[p].itr = it++;
                }else{
                    G[p].erase(e.rev), it = G[v].erase(it);
                }
            }
            for(int id : tmp) check[id].cost = inf;
            tmp.clear();
        }
    }
    bool contract(vector<int>& rem_node, vector<int>& check){
        vector<int> new_rem_node;
        bool update = false;
        for(int v : rem_node){
            const int p = uf.find(v);
            if(G[p].empty() || p != v) continue;
            T mn = inf; int id = -1;
            for(const edge& e : G[v]){
                if(e.cost < mn) mn = e.cost, id = e.to;
            }
            id = uf.find(id);
            if(id != v){
                ans += mn, update = true, uf.unite(id, v);
            }
        }
        for(int v : rem_node){
            const int p = uf.find(v);
            if(v == p) new_rem_node.push_back(v);
            else G[p].splice(G[p].begin(), move(G[v]));
        }
        swap(rem_node, new_rem_node);
        return update;
    }
    int boruvka(vector<int>& rem_node){
        vector<int> erase_check(V, 0);
        iota(rem_node.begin(), rem_node.end(), 0);
        vector<info> multiple_check(V);
        const int cnt = ceil(log2(ceil(log2(V)) + 1)) + 1;
        for(int i = 0; i < V; ++i) multiple_check[i] = info(inf);
        for(int i = 0; i < cnt; ++i){
            if((int)rem_node.size() == 1) return 1;
            if(!contract(rem_node, erase_check)) return 0;
            cleanup(rem_node, multiple_check);
        }
        return 2;
    }
    void prim(vector<int>& rem_node){
        int remV = 0;
        vector<int> trans(V);
        for(int v : rem_node) trans[v] = remV++;
        vector<T> d(remV, inf);
        FibonacciHeap<T, int> fheap;
        vector<typename FibonacciHeap<T, int>::node*> nodes(remV, nullptr);
        d[0] = 0;
        nodes[0] = fheap.push(0, 0);
        while(!fheap.empty()){
            const int v = fheap.top().second;
            ans += fheap.top().first;
            fheap.pop();
            d[v] = -1;
            for(auto& e : G[rem_node[v]]){
                const int p = trans[uf.find(e.to)];
                if(d[p] >= 0 && d[p] > e.cost){
                    if(d[p] == inf){
                        nodes[p] = fheap.push(e.cost, p);
                    }else{
                        fheap.decrease_key(nodes[p], d[p] - e.cost);
                    }
                    d[p] = e.cost;
                }
            }
        }
    }

public:
    T ans;
    RapidMST(const int node_size)
        : V(node_size), inf(numeric_limits<T>::max()), uf(V), G(V), ans(0){}
    void add_edge(const int from, const int to, const T cost){
        G[from].emplace_back(to, cost);
        G[to].emplace_back(from, cost, --G[from].end());
        G[from].back().rev = --G[to].end();
    }
    T solve(){
        vector<int> rem_node(V);
        const int res = boruvka(rem_node);
        if(res == 0) return inf;
        else if(res == 1) return ans;
        prim(rem_node);
        return ans;
    }
};
