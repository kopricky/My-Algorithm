#include "header.hpp"

//無向グラフの最小カット(n回max_flowを回すより高速)
//Nagamochi and Ibaraki 1992 Stoer and Wagner 1997 を参照
//フィボナッチヒープを用いているが縮約の部分の実装をサボっているので計算量はO(n^3)...
//縮約された頂点を持てば最小カットを構成する頂点集合を得ることが可能

template<typename KeyType> class Fibonacci_Heap
{
public:
    class node
    {
    public:
        KeyType key;
        int child, id;
        node *par, *prv, *nx, *ch_sentinel;
        bool mark;
        node(KeyType val, int _id) : key(val), child(0), id(_id),
                par(nullptr), prv(nullptr), nx(nullptr), mark(false){}
        node() : key(numeric_limits<KeyType>::max()), prv(this), nx(this){}
    };
    
private:
    int _size;
    node *minimum, *sentinel;
    
    inline void _insert(node *_node1, node *_node2){
        _node1->prv = _node2->prv, _node1->nx = _node2;
        _node1->prv->nx = _node1, _node2->prv = _node1;
    }
    inline void _erase(node *_node){
        _node->prv->nx = _node->nx, _node->nx->prv = _node->prv;
    }
    
public:
    Fibonacci_Heap() : _size(0){
        minimum = sentinel = new node();
    }
    inline bool empty(){
        return (_size == 0);
    }
    inline int size(){
        return _size;
    }
    inline node* top(){
        return minimum;
    }
    node* push(KeyType val, int _id){
        _size++;
        node* new_node = new node(val, _id);
        new_node->ch_sentinel = new node();
        _insert(new_node, sentinel);
        if(minimum->key > val) minimum = new_node;
        return new_node;
    }
    void pop(){
        assert(_size > 0);
        _size--;
        for(auto _node = minimum->ch_sentinel->nx; _node != minimum->ch_sentinel;){
            node* nx_node = _node->nx;
            _node->par = nullptr, _insert(_node, sentinel), _node = nx_node;
        }
        _erase(minimum);
        delete minimum;
        if(_size == 0){
            minimum = sentinel;
            return;
        }
        int sz = (int)log2(_size)+2;
        node** array = new node*[sz];
        for(int i = 0; i < sz; i++) array[i] = nullptr;
        for(auto _node = sentinel->nx; _node != sentinel;){
            node* nx_node = _node->nx;
            int deg = _node->child;
            while(array[deg]){
                if(_node->key < array[deg]->key){
                    array[deg]->par = _node, _node->child++;
                    _erase(array[deg]), _insert(array[deg], _node->ch_sentinel);
                }else{
                    _node->par = array[deg], array[deg]->child++;
                    _erase(_node), _insert(_node, array[deg]->ch_sentinel);
                    _node = array[deg];
                }
                array[deg] = nullptr;
                deg++;
                assert(sz > deg);
            }
            array[deg] = _node;
            _node = nx_node;
        }
        delete array;
        KeyType val = sentinel->key;
        for(auto _node = sentinel->nx; _node != sentinel; _node = _node->nx){
            if(val > _node->key) val = _node->key, minimum = _node;
        }
    }
    void update(node* _node, KeyType val){
        assert(val >= 0);
        _node->key -= val;
        if(_node->key < minimum->key) minimum = _node;
        if(_node->par && _node->par->key <= val) return;
        while(_node->par){
            node* nx_node = _node->par;
            _erase(_node), _node->par->child--, _insert(_node, sentinel);
            _node->mark = false, _node->par = nullptr, _node = nx_node;
            if(!_node) break;
            if(!_node->mark){
                _node->mark = true;
                break;
            }
        }
    }
    friend void unite(Fibonacci_Heap* fh1, Fibonacci_Heap* fh2){
        if(fh2->_size == 0){
            delete fh2;
            return;
        }
        if(fh1->minimum->key > fh2->minimum->key) fh1->minimum = fh2->minimum;
        fh1->_size += fh2->_size;
        fh1->sentinel->prv->nx = fh2->sentinel->nx;
        fh2->sentinel->nx->prv = fh1->sentinel->prv;
        fh2->sentinel->prv->nx = fh1->sentinel;
        fh1->sentinel->prv = fh2->sentinel->prv;
        delete fh2->sentinel;
    }
};

template<typename T> class UndirectedMinCut
{
public:
    struct edge{
        int to, rev;
        T cost;
    };
    
    int V;
    vector<vector<int> > G;
    vector<int> order;
    UndirectedMinCut(int node_size) : V(node_size), G(V, vector<int>(V, 0)), order(V){
        iota(order.begin(),order.end(),0);
    }
    void add_edge(int u, int v, T cap){
        G[u][v] += cap, G[v][u] += cap;
    }
    T solve(){
        T ans = numeric_limits<T>::max();
        for(int i = V; i > 1; i--){
            vector<typename Fibonacci_Heap<T>::node*> kp(i, nullptr);
            vector<int> new_order(i-1), rev_order(V, -1);
            for(int j = 0; j < i; j++) rev_order[order[j]] = j;
            Fibonacci_Heap<T> fh;
            for(int j = 0; j < i; j++) kp[j] = fh.push(0, j);
            for(int j = 0; j < i-1; j++){
                int mx_id = fh.top()->id;
                fh.pop();
                new_order[j] = order[mx_id], rev_order[order[mx_id]] = -1;
                for(int k = 0; k < i; k++){
                    if(rev_order[order[mx_id]] >= 0){
                        fh.update(kp[rev_order[order[mx_id]]], G[order[mx_id]][order[k]]);
                    }
                }
            }
            int last_ver = order[fh.top()->id], nx_last = new_order.back();
            for(int j = 0; j < (int)new_order.size()-1; j++){
                G[nx_last][new_order[j]] += G[last_ver][new_order[j]];
                G[new_order[j]][nx_last] += G[new_order[j]][last_ver];
            }
            order = new_order;
            ans = min(ans, -fh.top()->key);
        }
        return ans;
    }
};

