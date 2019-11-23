#include "header.hpp"

// 無向グラフの最小カット(n 回 max_flow を回すより高速)
// Nagamochi and Ibaraki 1992 Stoer and Wagner 1997 を参照
// 計算量はO(mn + n^2 log n)
// ans_set が最小カットを構成する頂点集合(S s.t (S, V-S) が最小カット)

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

class SimpleMergeSet
{
public:
    vector<int> next;
    SimpleMergeSet(const int node_size) : next(node_size){
        iota(next.begin(), next.end(), 0);
    }
    void unite(const int u, const int v){
        swap(next[u], next[v]);
    }
};

template<typename T> class UndirectedMinCut
{
private:
    struct edge{
        int to; T cost; int rev;
        edge(int _to, T _cost, int _rev)
        : to(_to), cost(_cost), rev(_rev){}
    };
    const int V;
    SimpleMergeSet ms;
    vector<vector<edge> > G;

public:
    vector<int> ans_set;
    UndirectedMinCut(const int node_size) : V(node_size), ms(V), G(V){}
    void add_edge(int u, int v, T cost){
		G[u].emplace_back(v, cost, (int)G[v].size());
        G[v].emplace_back(u, cost, (int)G[u].size() - 1);
    }
    T solve(){
        T ans = numeric_limits<T>::max();
		vector<typename FibonacciHeap<T, int>::node*> kp(V, nullptr);
		vector<int> ord(V);
		bool *visited = new bool[V]();
		iota(ord.begin(), ord.end(), 0);
        for(int i = V; i > 1; --i){
            FibonacciHeap<T, int> fh;
			vector<int> new_ord(i-1);
            for(int id : ord){
				kp[id] = fh.push(0, id);
			}
            for(int j = 0; j < i-1; ++j){
                int cur = fh.top().second;
                fh.pop();
                visited[cur] = true, new_ord[j] = cur;
				for(edge& e : G[cur]){
					if(!visited[e.to]) fh.decrease_key(kp[e.to], e.cost);
				}
            }
            int last_ver = fh.top().second, nx_last = new_ord[i-2];
			for(edge& e : G[last_ver]){
                if(e.to == nx_last) continue;
				G[nx_last].emplace_back(e.to, e.cost, e.rev);
                edge& reve = G[e.to][e.rev];
				reve.to = nx_last, reve.rev = (int)G[nx_last].size() - 1;
			}
			for(int ver : new_ord){
                visited[ver] = false;
            }
            visited[last_ver] = true;
            if(ans > -fh.top().first){
                ans = -fh.top().first;
                ans_set.clear();
                for(int cur = last_ver;;){
                    ans_set.push_back(cur);
                    if((cur = ms.next[cur]) == last_ver) break;
                }
            }
            ms.unite(nx_last, last_ver);
            swap(ord, new_ord);
        }
        delete[] visited;
        return ans;
    }
};
