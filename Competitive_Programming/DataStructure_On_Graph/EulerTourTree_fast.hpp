#include "../header.hpp"

template<class _Key, class _Tp, class _Hash, bool DOWNSIZE> class UnorderedMapIterator;

template<class _Key, class _Tp, class _Hash = hash<_Key>, bool DOWNSIZE = false>
class UnorderedMap
{
private:
    using iterator = UnorderedMapIterator<_Key, _Tp, _Hash, DOWNSIZE>;
    using value_type = _Tp;
    using data_type = pair<_Key, _Tp>;
    using aligned_pointer = typename aligned_storage<sizeof(value_type), alignof(value_type)>::type;
    friend UnorderedMapIterator<_Key, _Tp, _Hash, DOWNSIZE>;
    struct bucket {
        _Key _key;
        short int _dist;
        bool _last, _end;
        aligned_pointer _value_ptr;
        bucket() noexcept : _dist(-1), _last(false), _end(false){}
        bucket& operator=(const bucket& another) noexcept {
            _key = another._key, _dist = another._dist, _last = another._last, _end = another._end;
            if(!another.empty()){
                new(&_value_ptr) value_type(*reinterpret_cast<const value_type*>(&another._value_ptr));
            }
            return *this;
        }
        ~bucket(){ if(!empty()) _delete(); }
        inline void clear() noexcept { _dist = -1; }
        inline void _delete(){ _dist = -1, value_ptr()->~value_type(); }
        inline bool empty() const noexcept { return (_dist == -1); }
        inline value_type& value() noexcept {
            return *reinterpret_cast<value_type*>(&_value_ptr);
        }
        inline value_type* value_ptr() noexcept {
            return reinterpret_cast<value_type*>(&_value_ptr);
        }
        inline void new_value(value_type&& value){
            new(&_value_ptr) value_type(move(value));
        }
    };
    inline static unsigned int ceilpow2(unsigned int u) noexcept {
        if(u == 0u) return 0u;
        --u, u |= u >> 1, u |= u >> 2, u |= u >> 4, u |= u >> 8;
        return (u | (u >> 16)) + 1u;
    }
    inline static bucket *increment(bucket *cur) noexcept {
        for(++cur; !cur->_end; ++cur){
            if(!cur->empty()) break;
        }
        return cur;
    }
    inline bucket *next_bucket(bucket *cur) const noexcept {
        return cur->_last ? _buckets : cur + 1;
    }
    inline unsigned int make_hash(const _Key& key) const noexcept {
        return _Hash()(key);
    }
    inline float load_rate() const noexcept {
        return (float)_data_count / _bucket_count;
    }
    bucket *insert(bucket *cur, _Key&& key, short int dist, value_type&& value){
        bucket *ret = cur;
        bool flag = false;
        while(true){
            if(cur->empty()){
                cur->_key = move(key), cur->_dist = dist, cur->new_value(move(value));
                if(!flag) ret = cur, flag = true;
                break;
            }else if(dist > cur->_dist){
                swap(key, cur->_key), swap(dist, cur->_dist), swap(value, cur->value());
                if(!flag) ret = cur, flag = true;
            }
            ++dist;
            cur = next_bucket(cur);
        }
        return ret;
    }
    template<class Key>
    bucket *_find(Key&& key, bool push = false){
        unsigned int hash = make_hash(key);
        bucket *cur = _buckets + (hash & _mask);
        short int dist = 0;
        while(dist <= cur->_dist){
            if(key == cur->_key) return cur;
            ++dist, cur = next_bucket(cur);
        }
        if(!push) return _buckets + _bucket_count;
        ++_data_count;
        if(rehash_check()){
            cur = _buckets + (hash & _mask), dist = 0;
        }
        value_type new_value = value_type();
        _Key new_key = forward<Key>(key);
        return insert(cur, move(new_key), dist, move(new_value));
    }
    template<class Data>
    bucket *find_insert(Data&& data){
        const _Key& key = data.first;
        unsigned int hash = make_hash(key);
        bucket *cur = _buckets + (hash & _mask);
        short int dist = 0;
        while(dist <= cur->_dist){
            if(key == cur->_key) return cur;
            ++dist, cur = next_bucket(cur);
        }
        ++_data_count;
        if(rehash_check()){
            cur = _buckets + (hash & _mask), dist = 0;
        }
        data_type new_data = forward<Data>(data);
        return insert(cur, move(new_data.first), dist, move(new_data.second));
    }
    template<typename... Args>
    bucket *emplace(Args&&... args){
        return find_insert(data_type(forward<Args>(args)...));
    }
    bucket *backward_shift(bucket *cur, bool next_ret){
        bucket *next = next_bucket(cur), *ret = cur;
        if(next->_dist < 1) return next_ret ? increment(cur) : cur;
        do {
            cur->_key = next->_key, cur->_dist = next->_dist - 1;
            cur->new_value(move(next->value()));
            cur = next, next = next_bucket(cur);
        }while(next->_dist >= 1);
        cur->clear();
        return ret;
    }
    bucket *erase_impl(bucket *cur, bool next_ret){
        assert(static_cast<size_t>(cur - _buckets) != _bucket_count);
        cur->_delete();
        --_data_count;
        return backward_shift(cur, next_ret);
    }
    bucket *erase_itr(bucket *cur, bool next_ret = true){
        const _Key key = cur->_key;
        return erase_impl(rehash_check() ? _find(key) : cur, next_ret);
    }
    bucket *erase_key(const _Key& key, bool next_ret = true){
        rehash_check();
        return erase_impl(_find(key), next_ret);
    }
    bool rehash_check(){
        if(_bucket_count == 0){
            rehash(1u);
            return true;
        }else if(load_rate() >= MAX_LOAD_RATE){
            rehash(_bucket_count * 2u);
            return true;
        }else if(DOWNSIZE){
            if(load_rate() <= MIN_LOAD_RATE && _bucket_count >= DOWNSIZE_THRESHOLD){
                rehash(_bucket_count / 2u);
                return true;
            }
        }
        return false;
    }
    void move_data(bucket *cur){
        insert(_buckets + (make_hash(cur->_key) & _mask), move(cur->_key), 0, move(cur->value()));
    }
    void rehash(unsigned int new_bucket_count){
        UnorderedMap new_unordered_map(new_bucket_count);
        new_unordered_map._data_count = _data_count;
        for(bucket *cur = _buckets; !cur->_end; ++cur){
            if(!cur->empty()){
                new_unordered_map.move_data(cur);
            }
        }
        swap(*this, new_unordered_map);
    }
    friend void swap(UnorderedMap& ump1, UnorderedMap& ump2){
        swap(ump1._bucket_count, ump2._bucket_count);
        swap(ump1._mask, ump2._mask);
        swap(ump1._data_count, ump2._data_count);
        swap(ump1._buckets, ump2._buckets);
    }

private:
    unsigned int _bucket_count, _mask, _data_count;
    bucket *_buckets;
public:
    const float MAX_LOAD_RATE = 0.5f;
    const float MIN_LOAD_RATE = 0.1f;
    const unsigned int DOWNSIZE_THRESHOLD = 16u;
    UnorderedMap(unsigned int bucket_size = 0u)
     : _bucket_count(ceilpow2(bucket_size)), _mask(_bucket_count - 1),
        _data_count(0u), _buckets(new bucket[_bucket_count + 1]){
        if(_bucket_count > 0) _buckets[_bucket_count - 1]._last = true;
        else _mask = 0;
        _buckets[_bucket_count]._end = true;
    }
    UnorderedMap(const UnorderedMap& another)
        : _bucket_count(another._bucket_count), _mask(another._mask), _data_count(another._data_count){
        _buckets = new bucket[_bucket_count + 1u];
        for(unsigned int i = 0u; i <= _bucket_count; ++i){
            _buckets[i] = another._buckets[i];
        }
    }
    UnorderedMap(UnorderedMap&& another)
        : _bucket_count(move(another._bucket_count)), _mask(move(another._mask)),
            _data_count(move(another._data_count)), _buckets(another._buckets){
        another._buckets = nullptr;
    }
    UnorderedMap& operator=(const UnorderedMap& another){
        delete[] _buckets;
        _bucket_count = another._bucket_count;
        _mask = another._mask;
        _data_count = another._data_count;
        _buckets = new bucket[_bucket_count + 1u];
        for(unsigned int i = 0u; i <= _bucket_count; ++i){
            _buckets[i] = another._buckets[i];
        }
        return *this;
    }
    UnorderedMap& operator=(UnorderedMap&& another){
        delete[] _buckets;
        _bucket_count = move(another._bucket_count);
        _mask = move(another._mask);
        _data_count = move(another._data_count);
        _buckets = another._buckets;
        another._buckets = nullptr;
        return *this;
    }
    void allocate(unsigned int element_size){
        rehash(ceilpow2(ceil(element_size / MAX_LOAD_RATE) + 1));
    }
    ~UnorderedMap(){ delete[] _buckets; }
    friend ostream& operator<< (ostream& os, UnorderedMap& ump) noexcept {
        for(auto val : ump) os << '{' << val.first << ',' << val.second << "} ";
        return os;
    }
    _Tp& operator[](const _Key& key){ return _find(key, true)->value(); }
    _Tp& operator[](_Key&& key){ return _find(move(key), true)->value(); }
    const _Tp& at(const _Key& key){
        bucket *res = _find(key);
        if(res == _buckets + _bucket_count) __throw_out_of_range(__N("Unordered_Map::at"));
        return res->value();
    }
    void clear(){
        UnorderedMap new_unordered_map(1u);
        swap(*this, new_unordered_map);
    }
    size_t size() const noexcept { return _data_count; }
    size_t bucket_count() const noexcept { return _bucket_count; }
    bool empty() const noexcept { return (_data_count == 0); }
    iterator begin() noexcept {
        return (_buckets->empty() && _bucket_count > 0) ? iterator(increment(_buckets)) : iterator(_buckets);
    }
    iterator end() noexcept { return iterator(_buckets + _bucket_count); }
    iterator find(const _Key& key){ return iterator(_find(key)); }
    iterator insert(const data_type& data){ return iterator(find_insert(data)); }
    iterator insert(data_type&& data){ return iterator(find_insert(move(data))); }
    template<typename... Args>
    iterator emplace(Args&&... args){ return iterator(_emplace(forward<Args>(args)...)); }
    iterator erase(const _Key& key){ return iterator(erase_key(key)); }
    iterator erase(const iterator& itr){ return iterator(erase_itr(itr.bucket_ptr)); }
    void simple_erase(const _Key& key){ erase_key(key, false); }
    void simple_erase(const iterator& itr){ erase_itr(itr.bucket_ptr, false); }

    // DEBUG 用
    short int maximum_distance() const noexcept {
        short int ret = -1;
        for(bucket *cur = _buckets; !cur->_end; ++cur){
            ret = max(ret, cur->_dist);
        }
        return ret;
    }
};

template<class _Key, class _Tp, class _Hash, bool DOWNSIZE>
class UnorderedMapIterator {
private:
    friend UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>;
    typename UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>::bucket *bucket_ptr;
    using iterator_category = forward_iterator_tag;
    using value_type = pair<const _Key, _Tp>;
    using difference_type = ptrdiff_t;
    using reference = pair<const _Key&, _Tp&>;

private:
    UnorderedMapIterator(typename UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>::bucket *_bucket_ptr)
        noexcept : bucket_ptr(_bucket_ptr){}
public:
    UnorderedMapIterator() noexcept : bucket_ptr(){}
    UnorderedMapIterator(const UnorderedMapIterator& itr) noexcept : bucket_ptr(itr.bucket_ptr){}
    UnorderedMapIterator& operator=(const UnorderedMapIterator& itr)
        & noexcept { return bucket_ptr = itr.bucket_ptr, *this; }
    UnorderedMapIterator& operator=(const UnorderedMapIterator&& itr)
        & noexcept { return bucket_ptr = itr.bucket_ptr, *this; }
    reference operator*() const noexcept { return {bucket_ptr->_key, bucket_ptr->value()}; }
    UnorderedMapIterator& operator++() noexcept {
        return bucket_ptr = UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>::increment(bucket_ptr), *this;
    }
    UnorderedMapIterator operator++(int) const noexcept {
        return UnorderedMapIterator(UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>::increment(this->bucket_ptr));
    }
    bool operator==(const UnorderedMapIterator& itr) const noexcept { return !(*this != itr); };
    bool operator!=(const UnorderedMapIterator& itr) const noexcept { return bucket_ptr != itr.bucket_ptr; }
};

template<typename _Tp> class BSTNode {
public:
    int from, to, sz;
    _Tp value, al, lazy;
    BSTNode *left, *right, *par;
    BSTNode(const int _from, const int _to) noexcept
        : from(_from), to(_to), sz(0), value(id1), al(id2), lazy(id1),
            left(nullptr), right(nullptr), par(nullptr){}
    BSTNode(const int _from, const int _to, const _Tp& _value) noexcept
        : from(_from), to(_to), sz(1), value(_value), al(value), lazy(id1),
            left(nullptr), right(nullptr), par(nullptr){}
    static const _Tp id1 = (_Tp)0;
    static const _Tp id2 = (_Tp)0;
    static void opr1(_Tp& arg1, const _Tp arg2) noexcept { arg1 += arg2; }
    static _Tp opr2(const _Tp arg1, const _Tp arg2) noexcept { return arg1 + arg2; }
    inline bool IsRoot() const noexcept { return !par; }
    inline bool IsVertex() const noexcept { return from == to; }
    void push() noexcept {
        if(lazy != id1){
            if(IsVertex()) opr1(value, lazy);
            al += lazy * sz;
            if(left) opr1(left->lazy, lazy);
            if(right) opr1(right->lazy, lazy);
            lazy = id1;
        }
    }
    void eval() noexcept {
        sz = IsVertex(), al = value;
        if(left) left->push(), sz += left->sz, al = opr2(left->al, al);
        if(right) right->push(), sz += right->sz, al = opr2(al, right->al);
    }
    void rotate(const bool right_) noexcept {
        BSTNode *p = par, *g = p->par;
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
BSTNode<_Tp>* splay(BSTNode<_Tp> *u) noexcept {
    if(!u) return nullptr;
    while(!(u->IsRoot())){
        BSTNode<_Tp> *p = u->par, *gp = p->par;
        if(p->IsRoot()){ // zig
            p->push(), u->push();
            u->rotate((u == p->left));
        }else{
            gp->push(), p->push(), u->push();
            bool flag = (u == p->left);
            if((u == p->left) == (p == gp->left)){ // zig-zig
                p->rotate(flag), u->rotate(flag);
            }else{ // zig-zag
                u->rotate(flag), u->rotate(!flag);
            }
        }
    }
    u->push();
    return u;
}

template<typename _Tp>
BSTNode<_Tp>* join(BSTNode<_Tp> *root1, BSTNode<_Tp> *root2) noexcept {
    if(!root1 || !root2) return root1 ? root1 : root2;
    BSTNode<_Tp> *cur = nullptr, *nx = root1;
    do{ cur = nx, cur->push(), nx = cur->right; }while(nx);
    BSTNode<_Tp> *ver = splay(cur);
    ver->right = root2, ver->eval(), root2->par = ver;
    return ver;
}

template<typename _Tp>
pair<BSTNode<_Tp>*, BSTNode<_Tp>*> split_lower_bond(BSTNode<_Tp> *ver) noexcept {
    BSTNode<_Tp> *res = splay(ver)->left;
    ver->left = nullptr, ver->eval();
    if(res) res->par = nullptr;
    return make_pair(res, ver);
}

template<typename _Tp>
pair<BSTNode<_Tp>*, BSTNode<_Tp>*> split_upper_bond(BSTNode<_Tp> *ver) noexcept {
    BSTNode<_Tp> *res = splay(ver)->right;
    ver->right = nullptr, ver->eval();
    if(res) res->par = nullptr;
    return make_pair(ver, res);
}

template<typename T> class EulerTourTree {
private:
    struct murmur_hash64 {
        unsigned long long operator()(unsigned long long p) const {
            const unsigned long long m = 0xc6a4a7935bd1e995; p *= m;
            unsigned long long h = (p^(p>>47))*m;
            return h = (h^(h>>47))*m, (h^(h>>47));
        }
    };
    BSTNode<T>** vertex_set;
    UnorderedMap<unsigned long long, pair<BSTNode<T>*, BSTNode<T>*>, murmur_hash64> edge_set;
    inline static unsigned long long pair_to_ll(const int u, const int v){
        return ((unsigned long long)(u) << 32) | v;
    }
    BSTNode<T> *reroot(BSTNode<T> *ver) noexcept {
        BSTNode<T> *res = splay(ver)->left;
        if(!res) return ver;
        ver->left = nullptr, ver->eval();
        while(ver->right) ver->push(), ver = ver->right;
        splay(ver), ver->right = res, ver->eval(), res->par = ver;
        return ver;
    }
    void link(BSTNode<T> *ver1, BSTNode<T> *ver2) noexcept {
        BSTNode<T>* e1 = new BSTNode<T>(ver1->from, ver2->from);
        BSTNode<T>* e2 = new BSTNode<T>(ver2->from, ver1->from);
        edge_set[pair_to_ll(ver1->from, ver2->from)] = {e1, e2};
        join(join(reroot(ver1), e1), join(reroot(ver2), e2));
    }
    void cut(BSTNode<T> *edge1, BSTNode<T> *edge2) noexcept {
        splay(edge1), splay(edge2);
        BSTNode<T> *p = edge1->par;
        bool _right = (edge1 == edge2->right);
        if(p != edge2){
            _right = (p == edge2->right);
            p->par = nullptr, edge1->rotate((edge1 == p->left));
        }
        if(edge1->left) edge1->left->par = nullptr;
        if(edge1->right) edge1->right->par = nullptr;
        if(_right){
            if(edge2->left) edge2->left->par = nullptr;
            join(edge2->left, edge1->right);
        }else{
            if(edge2->right) edge2->right->par = nullptr;
            join(edge1->left, edge2->right);
        }
        // delete edge1; delete edge2;
    }
    bool connected(BSTNode<T> *ver1, BSTNode<T> *ver2) noexcept {
        splay(ver1), splay(ver2);
        return ver1->par;
    }
    T& get(BSTNode<T> *ver) noexcept { return splay(ver)->value; }
    void point_update(BSTNode<T> *ver, const T& val){
        BSTNode<T>::opr1(splay(ver)->value, val);
        ver->al = BSTNode<T>::opr2(ver->al, val);
    }
    void range(BSTNode<T> *edge1, BSTNode<T> *edge2, const T& val) noexcept {
        auto res1 = split_lower_bond(edge1);
        auto res2 = split_upper_bond(edge2);
        BSTNode<T>::opr1(res2.first->lazy, val);
        join(join(res1.first,res2.first), res2.second);
    }
    T query(BSTNode<T> *edge1, BSTNode<T> *edge2) noexcept {
        auto res1 = split_lower_bond(edge1);
        auto res2 = split_upper_bond(edge2);
        T res = res2.first->al;
        return join(join(res1.first,res2.first), res2.second), res;
    }
    void dfs(const int u, const int p, const BSTNode<T> *cur,
        bool *visit, vector<BSTNode<T>*>& nodes, const vector<vector<int> >& G) noexcept {
        visit[u] = true;
        nodes.push_back(vertex_set[u]);
        for(auto& v : G[u]){
            if(v != p){
                BSTNode<T>* e1 = new BSTNode<T>(u, v);
                nodes.push_back(e1);
                dfs(v, u, cur, visit, nodes, G);
                BSTNode<T>* e2 = new BSTNode<T>(v, u);
                if(u < v) edge_set[pair_to_ll(u, v)] = {e1, e2};
                else edge_set[pair_to_ll(v, u)] = {e2, e1};
                nodes.push_back(e2);
            }
        }
    }
    void bst_build(vector<BSTNode<T>* >& nodes) noexcept {
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
    }
    void build_forest(const vector<vector<int> >& forest) noexcept {
        bool *visit = new bool[V]();
        for(int i = 0; i < V; i++){
            if(!visit[i]){
                vector<BSTNode<T>* > nodes;
                BSTNode<T> *cur = nullptr;
                dfs(i, -1, cur, visit, nodes, forest);
                bst_build(nodes);
            }
        }
        // delete[] visit;
    }
    void build_tree(const int root, const vector<vector<int> >& tree) noexcept {
        bool *visit = new bool[V]();
        vector<BSTNode<T>* > nodes;
        BSTNode<T> *cur = nullptr;
        dfs(root, -1, cur, visit, nodes, tree);
        bst_build(nodes);
        // delete[] visit;
    }

public:
    const int V;
    EulerTourTree(const vector<T>& ver_value) noexcept : V((int)ver_value.size()){
        vertex_set = new BSTNode<T>*[V];
        for(int i = 0; i < V; i++) vertex_set[i] = new BSTNode<T>(i, i, ver_value[i]);
    }
    EulerTourTree(const vector<T>& ver_value, const vector<vector<int> >& forest) noexcept
        : EulerTourTree(ver_value){
        unsigned int element_size = 0;
        for(int i = 0; i < V; ++i) element_size += forest[i].size();
        edge_set.allocate(element_size);
        build_forest(forest);
    }
    EulerTourTree(const vector<T>& ver_value, const int root, const vector<vector<int> >& tree) noexcept
        : EulerTourTree(ver_value){
        unsigned int element_size = 0;
        for(int i = 0; i < V; ++i) element_size += tree[i].size();
        edge_set.allocate(element_size);
        build_tree(root, tree);
    }
    // ~EulerTourTree(){
    //     for(auto it : edge_set){
    //         delete (it.second).first;
    //         delete (it.second).second;
    //     }
    //     for(int i = 0; i < V; ++i) delete vertex_set[i];
    //     delete[] vertex_set;
    // }
    // 根を node_id にする
    void reroot(const int node_id) noexcept { reroot(vertex_set[node_id]); }
    // 辺(node1_id, node2_id) を追加
    void link(int node1_id, int node2_id) noexcept {
        if(node1_id > node2_id) swap(node1_id, node2_id);
        link(vertex_set[node1_id], vertex_set[node2_id]);
    }
    // 辺(node1_id, node2_id) を削除(逆向きでも問題ない)
    void cut(int node1_id, int node2_id){
        if(node1_id > node2_id) swap(node1_id, node2_id);
        auto it = edge_set.find(pair_to_ll(node1_id, node2_id));
        assert(it != edge_set.end());
        BSTNode<T> *edge1 = ((*it).second).first, *edge2 = ((*it).second).second;
        edge_set.simple_erase(it);
        cut(edge1, edge2);
    }
    // node1_id と node2_id が同じ木(連結成分)に属するか
    bool connected(const int node1_id, const int node2_id) noexcept {
        if(node1_id == node2_id) return true;
        return connected(vertex_set[node1_id], vertex_set[node2_id]);
    }
    // 頂点 ver_id の値を取得
    T& get(int ver_id) noexcept { return get(vertex_set[ver_id]); }
    // 頂点 ver_id に val を加える
    void point_update(const int ver_id, const T& val) noexcept {
        return point_update(vertex_set[ver_id], val);
    }
    // 頂点 ver_id の存在する連結成分内の頂点全体に val を加える
    void component_range(const int ver_id, const T& val){ range(ver_id, -1, val); }
    // 親が par_id であるような頂点 ver_id の部分木内に存在する頂点全体に val を加える
    void range(const int ver_id, const int par_id, const T& val){
        if(par_id < 0) return BSTNode<T>::opr1(splay(vertex_set[ver_id])->lazy, val);
        if(ver_id < par_id){
            auto it = edge_set.find(pair_to_ll(ver_id, par_id));
            assert(it != edge_set.end());
            range(((*it).second).second, ((*it).second).first, val);
        }else{
            auto it = edge_set.find(pair_to_ll(par_id, ver_id));
            assert(it != edge_set.end());
            range(((*it).second).first, ((*it).second).second, val);
        }
    }
    // 頂点 ver_id の存在する連結成分内の頂点全体の総和を取得
    T component_query(const int ver_id){ return query(ver_id, -1); }
    // 親が par_id であるような頂点 ver_id の部分木内に存在する頂点全体の総和を取得
    T query(const int ver_id, const int par_id){
        if(par_id < 0) return splay(vertex_set[ver_id])->al;
        if(ver_id < par_id){
            auto it = edge_set.find(pair_to_ll(ver_id, par_id));
            assert(it != edge_set.end());
            return query(((*it).second).second, ((*it).second).first);
        }else{
            auto it = edge_set.find(pair_to_ll(par_id, ver_id));
            assert(it != edge_set.end());
            return query(((*it).second).first, ((*it).second).second);
        }
    }

private:
    void check_dfs(const BSTNode<T>* cur) const noexcept {
        cur->push();
        if(cur->left) check_dfs(cur->left);
        if(cur->IsVertex()) cout <<"{" << (cur->from) << "," << (cur->value) << "} ";
        if(cur->right) check_dfs(cur->right);
    }
};

