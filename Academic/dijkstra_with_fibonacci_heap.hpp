#include "./header.hpp"

template<typename KeyType> class Fibonacci_Heap
{
public:
    class node
    {
    public:
        KeyType key;
        int id, child;
        node *par, *prv, *nx, *ch_sentinel;
        bool mark;
        node(KeyType val, int _id, node *_prv, node *_nx) : key(val), id(_id), child(0),
                par(nullptr), prv(_prv), nx(_nx), mark(false){}
        node(KeyType val, int _id) : node(val, _id, nullptr, nullptr){}
        node() : node(0, -1, this, this){}
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
    Fibonacci_Heap() : _size(0), minimum(nullptr){
        sentinel = new node();
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
    node *push(KeyType val, int _id){
        _size++;
        node* new_node = new node(val, _id);
        new_node->ch_sentinel = new node();
        _insert(new_node, sentinel);
        if(!minimum || minimum->key > val) minimum = new_node;
        return new_node;
    }
    void pop(){
        _size--;
        for(auto _node = minimum->ch_sentinel->nx; _node != minimum->ch_sentinel;){
            node* nx_node = _node->nx;
            _node->par = nullptr, _insert(_node, sentinel), _node = nx_node;
        }
        _erase(minimum);
        delete minimum;
        if(_size == 0) return;
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
        KeyType val = numeric_limits<KeyType>::max();
        for(auto _node = sentinel->nx; _node != sentinel; _node = _node->nx){
            if(val > _node->key) val = _node->key, minimum = _node;
        }
    }
    void update(node* _node, KeyType val){
        assert(_node->key - val >= 0);
        _node->key = val;
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
};

template<typename T> class Dijkstra {
public:
    struct edge{
        int to; T cost;
    };
    int V;
    vector<vector<edge> > G;
    vector<T> d;
    using pti = pair<T,int>;
    Fibonacci_Heap<T> fheap;
    typename Fibonacci_Heap<T>::node** keep;
    Dijkstra(int node_size) : V(node_size), G(V), d(V, numeric_limits<T>::max()),
        keep(new typename Fibonacci_Heap<T>::node*[V]){}
    //無向グラフの場合
    void add_edge(int u,int v,T cost){
        G[u].push_back((edge){v,cost}), G[v].push_back((edge){u,cost});
    }
    void solve(int s){
        d[s] = 0;
        keep[s] = fheap.push(0, s);
        while(!fheap.empty()){
            int v = fheap.top()->id, val = fheap.top()->key;
            fheap.pop();
            if(d[v] < val) continue;
            for(auto& w : G[v]){
                if(d[w.to] > d[v] + w.cost){
                    if(d[w.to] == numeric_limits<T>::max()){
                        keep[w.to] = fheap.push(d[v] + w.cost, w.to);
                    }else{
                        fheap.update(keep[w.to], d[v] + w.cost);
                    }
                    d[w.to] = d[v] + w.cost;
                }
            }
        }
    }
};