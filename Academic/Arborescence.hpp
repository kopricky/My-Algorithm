#include "./header.hpp"

template<typename _Tp>
class LazyUnionFind {
private:
    const int sz;
    vector<_Tp> diff, lazy;
    vector<int> par, nrank;
    pair<int, _Tp> _find(int x){
        if(par[x] == x) return {x, (_Tp)0};
        auto res = _find(par[x]);
        par[x] = res.first, diff[x] += res.second, lazy[x] += res.second;
        return {res.first, lazy[x]};
    }
public:
    LazyUnionFind(const int node_size)
        : sz(node_size), diff(sz, (_Tp)0), lazy(sz, (_Tp)0), par(sz), nrank(sz, 0){
        iota(par.begin(), par.end(), 0);
    }
    int find(int x){ return _find(x).first; }
    _Tp find_value(int x){
        _find(x);
        return (par[x] == x) ? diff[x] : (diff[x] + lazy[par[x]]);
    }
    void change_value(_Tp value, int x){
        diff[x] += value, lazy[x] += value;
    }
    int unite(int x, int y){
        x = find(x), y = find(y);
        if(x == y) return -1;
    	if(nrank[x] < nrank[y]) swap(x,y);
        par[y] = x, diff[y] -= lazy[x], lazy[y] -= lazy[x];
        if(nrank[x] == nrank[y]) nrank[x]++;
        return x;
    }
};

template<class edge, typename _Tp>
class FibonacciHeap
{
public:
    using data_type = const edge*;
    class node
    {
    public:
        data_type _data;
        unsigned short int _child;
        bool _mark;
        node *_par, *_prev, *_next, *_ch_last;
        node(data_type data) : _data(data), _child(0), _mark(false),
            _par(nullptr), _prev(nullptr), _next(nullptr), _ch_last(nullptr){}
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
    node *_minimum;
    vector<node*> rank;
    LazyUnionFind<_Tp>& uf;
    vector<FibonacciHeap*>& fh;
    vector<int>& heap;

    static void insert_impl(node *cur, node *next){
        cur->_prev = next->_prev, cur->_next = next;
        cur->_prev->_next = cur, next->_prev = cur;
    }
    static void erase_impl(node *cur){
        cur->_prev->_next = cur->_next, cur->_next->_prev = cur->_prev;
    }
    inline const _Tp get_key(node *cur) const noexcept {
        return cur->_data->cost + uf.find_value(cur->_data->to);
    }
    inline FibonacciHeap *home_heap(node *cur) const noexcept {
        return fh[uf.find(heap[uf.find(cur->_data->from)])];
    }
    void root_insert(node *cur){
        if(_minimum){
            insert_impl(cur, _minimum);
        }else{
            _minimum = cur, _minimum->_prev = _minimum->_next = _minimum;
        }
    }
    void root_erase(node *cur){
        if(cur == cur->_prev) _minimum = nullptr;
        else{
            if(cur == _minimum) _minimum = cur->_prev;
            erase_impl(cur);
        }
    }
    void _delete(node *cur){
        root_erase(cur);
        delete cur;
    }
    node *_push(data_type e){
        node *new_node = new node(e);
        root_insert(new_node);
        return new_node;
    }
    void detect_minimum(){
        node *cand = nullptr;
        _Tp _min = numeric_limits<_Tp>::max();
        for(node *cur = _minimum->_next;;cur = cur->_next){
            _Tp value = get_key(cur);
            if(_min > value) cand = cur, _min = value;
            if(cur == _minimum) break;
        }
        _minimum = cand;
    }
    data_type _pop(node *given_minimum = nullptr){
        if(!_minimum) return nullptr;
        if(given_minimum) _minimum = given_minimum;
        else detect_minimum();
        data_type res = _minimum->_data;
        if(_minimum->_ch_last){
            for(node *cur = _minimum->_ch_last->_next;;){
                node *next = cur->_next;
                _minimum->erase(cur);
                home_heap(cur)->root_insert(cur);
                if(!_minimum->_ch_last) break;
                cur = next;
            }
        }
        node *next_minimum = _minimum->_next;
        if(next_minimum == _minimum){
            _delete(_minimum);
            return res;
        }
        for(node*& cur : rank) cur = nullptr;
        for(node *cur = next_minimum; cur != _minimum;){
            if(get_key(cur) < get_key(next_minimum)) next_minimum = cur;
            node *next = cur->_next;
            unsigned int deg = cur->_child;
            if(rank.size() <= deg) rank.resize(deg + 1, nullptr);
            while(rank[deg]){
                if(get_key(cur) < get_key(rank[deg]) || cur == next_minimum){
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
        return res;
    }
    void _to_root(node *cur){
        if(!cur->_par) return;
        while(true){
            node *next = cur->_par;
            next->erase(cur);
            home_heap(cur)->root_insert(cur);
            cur->_mark = false, cur = next;
            if(!cur->_par) break;
            if(!cur->_mark){
                cur->_mark = true;
                break;
            }
        }
    }
    void _delete_node(node *cur){
        _to_root(cur), home_heap(cur)->_pop(cur);
    }

public:
    FibonacciHeap(LazyUnionFind<_Tp>& _uf, vector<FibonacciHeap*>& _fh, vector<int>& _heap)
        noexcept : _minimum(nullptr), uf(_uf), fh(_fh), heap(_heap){}
    node *push(data_type e){ return _push(e); }
    data_type pop(){ return _pop(); }
    void to_root(node *cur){ _to_root(cur); }
    void delete_node(node *cur){ _delete_node(cur); }
    friend void move_node(FibonacciHeap *fh1, FibonacciHeap::node *cur, FibonacciHeap *fh2){
        if(!cur->_par){
            fh1->root_erase(cur), fh2->root_insert(cur);
            return;
        }
        bool _first = true;
        while(true){
            node *next = cur->_par;
            next->erase(cur);
            if(_first) fh2->root_insert(cur), _first = false;
            else fh1->home_heap(cur)->root_insert(cur);
            cur->_mark = false, cur = next;
            if(!cur->_par) break;
            if(!cur->_mark){
                cur->_mark = true;
                break;
            }
        }
    }
    friend FibonacciHeap *meld(FibonacciHeap *fh1, FibonacciHeap *fh2){
        if(!fh2->_minimum){
            return delete fh2, fh1;
        }
        if(!fh1->_minimum){
            return delete fh1, fh2;
        }
        fh1->_minimum->_prev->_next = fh2->_minimum->_next;
        fh2->_minimum->_next->_prev = fh1->_minimum->_prev;
        fh2->_minimum->_next = fh1->_minimum;
        fh1->_minimum->_prev = fh2->_minimum;
        return delete fh2, fh1;
    }
};

template<typename _Tp>
class Arborescence {
private:
    struct edge {
        const int from, to;
        const _Tp cost;
        edge(int _from, int _to, _Tp _cost)
            : from(_from), to(_to), cost(_cost){}
    };
    struct info {
        const edge *e;
        typename list<info>::iterator itr;
        info(const edge *_e) : e(_e){}
        info(const edge *_e, typename list<info>::iterator _itr) : e(_e), itr(_itr){}
    };
    struct cycle_edge {
        int from, super_from, super_to;
        const edge *e;
        cycle_edge(int _from, int _super_from, int _super_to, const edge *_e)
            : from(_from), super_from(_super_from), super_to(_super_to), e(_e){}
    };
    const int V;
    int super_id;
    vector<vector<edge> > revG;
    vector<forward_list<cycle_edge> > cycle;
    forward_list<forward_list<cycle_edge> > path;
    vector<list<info> > _exit, passive;
    vector<int> used, heap, par;
    LazyUnionFind<_Tp> uf;
    vector<FibonacciHeap<edge, _Tp>*> fh;
    vector<typename FibonacciHeap<edge, _Tp>::node*> nodes;
public:
    _Tp ans;
    vector<int> parent;
    Arborescence(int node_size)
        : V(node_size), super_id(V), revG(V), cycle(2*V), _exit(V), passive(V),
            used(V, -1), heap(V, -1), par(2*V), uf(V), fh(V, nullptr), nodes(V, nullptr),
                ans((_Tp)0), parent(V, -1){
        iota(par.begin(), par.end(), 0);
    }
    // ~Arborescence(){
        // for(int i = 0; i < V; ++i) if(nodes[i]) delete nodes[i];
        // for(int i = 0; i < V; ++i) if(fh[i]) delete fh[i];
    // }
    void add_edge(int from, int to, _Tp cost){
        revG[to].emplace_back(from, to, cost);
    }
    void _move_node(int prev, int vertex, int next, const edge *e){
        move_node(fh[prev], nodes[vertex], fh[next]);
        heap[vertex] = next, nodes[vertex]->_data = e;
    }
    void grow_path(int cur, forward_list<int>& visit){
        visit.push_front(cur);
        while(!_exit[cur].empty()){
            const info& res = _exit[cur].front();
            passive[uf.find(res.e->to)].erase(res.itr);
            _exit[cur].pop_front();
        }
        fh[cur] = new FibonacciHeap<edge, _Tp>(uf, fh, heap);
        for(const edge& e : revG[cur]){
            const int from = uf.find(e.from);
            if(cur == from) continue;
            if(heap[from] < 0){
                heap[from] = cur, nodes[from] = fh[cur]->push(&e);
            }else if(heap[from] == cur){
                if(e.cost < nodes[from]->_data->cost){
                    nodes[from]->_data = &e;
                    fh[cur]->to_root(nodes[from]);
                }
            }else{
                const int hfrom = uf.find(heap[from]);
                passive[hfrom].emplace_front(nodes[from]->_data);
                _exit[from].emplace_front(nodes[from]->_data, passive[hfrom].begin());
                passive[hfrom].front().itr = _exit[from].begin();
                _move_node(hfrom, from, cur, &e);
            }
        }
    }
    int contract_cycle(int cur, forward_list<cycle_edge>&& cur_path){
        int modify = -1;
        for(auto it = next(cur_path.begin(), 1);; ++it){
            const int v = (it == cur_path.end()) ? cur : it->from;
            while(!passive[v].empty()){
                const info& res1 = passive[v].front();
                const int from = uf.find(res1.e->from);
                if(nodes[from] &&
                    nodes[from]->_data->cost + uf.find_value(nodes[from]->_data->to) >
                        res1.e->cost + uf.find_value(res1.e->to)){
                    _move_node(uf.find(heap[from]), from, v, res1.e);
                }
                _exit[from].erase(res1.itr), passive[v].pop_front();
            }
            if(v == cur) break;
            par[it->super_from] = super_id;
            modify = it->super_to;
        }
        par[cur_path.begin()->super_from = modify] = super_id;
        int ver = -1;
        for(auto it = next(cur_path.begin(), 1);; ++it){
            const int v = (it == cur_path.end()) ? cur : it->from;
            if(heap[v] >= 0) fh[v]->delete_node(nodes[v]), heap[v] = -1, nodes[v] = nullptr;
            if(ver >= 0){
                if(uf.unite(ver, v) == ver) fh[ver] = meld(fh[ver], fh[v]), fh[v] = nullptr;
                else fh[v] = meld(fh[ver], fh[v]), fh[ver] = nullptr, ver = v;
            }else ver = v;
            if(v == cur){
                cycle[super_id].push_front(cur_path.front()), cur_path.pop_front();
                cycle[super_id].splice_after(cycle[super_id].begin(), move(cur_path),
                    cur_path.before_begin(), it);
                break;
            }
        }
        if(!cur_path.empty()) cur_path.begin()->from = ver, cur_path.begin()->super_from = super_id;
        return ver;
    }
    void cycle_dfs(int u, int par_cycle){
        while(u != par[u] && par[u] != par_cycle){
            const int p = par[u];
            for(auto it = cycle[p].begin(); it != cycle[p].end(); ++it){
                int v = it->super_from;
                if(v == u){
                    while(u != it->super_to){
                        const int w = it->e->to;
                        parent[w] = it->e->from;
                        if(p != par[w]) cycle_dfs(w, p);
                        if(++it == cycle[p].end()) it = cycle[p].begin();
                    }
                    break;
                }
            }
            u = p;
        }
    }
    void identify_tree(){
        for(forward_list<cycle_edge>& cur_path : path){
            while(!cur_path.empty()){
                int v = cur_path.begin()->e->to;
                parent[v] = cur_path.begin()->e->from;
                cur_path.pop_front();
                cycle_dfs(v, -1);
            }
        }
    }
    _Tp solve(const int root){
        used[root] = 1;
        for(int i = 0; i < V; ++i){
            if(used[i] != -1) continue;
            int cur = i, super_cur;
            forward_list<cycle_edge> cur_path;
            forward_list<int> visit;
            while(used[cur] != 1){
                if(used[cur] == -1){
                    grow_path(cur, visit);
                    used[super_cur = cur] = 0;
                }else{
                    cur = contract_cycle(cur, move(cur_path));
                    super_cur = super_id++;
                }
                const edge *e = fh[cur]->pop();
                if(!e) return numeric_limits<_Tp>::max();
                ans += e->cost + uf.find_value(e->to);
                uf.change_value(-e->cost - uf.find_value(e->to), cur);
                const int next = uf.find(e->from);
                heap[next] = -1, nodes[next] = nullptr;
                cur_path.emplace_front(next, e->from, super_cur, e);
                cur = next;
            }
            path.push_front(move(cur_path));
            for(const int ver : visit) used[ver] = 1;
        }
        identify_tree();
        return ans;
    }
};
