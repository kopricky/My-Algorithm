#include "../header.hpp"

template<typename _Key, typename _Tp> class MapIterator;

template<typename _Key, typename _Tp> class Map {
private:
    using iterator = MapIterator<_Key, _Tp>;
    struct node {
        pair<const _Key, _Tp> data;
        node *_M_left, *_M_right, *_M_parent;
        node(const _Key _key, const _Tp _value) noexcept
         : data(_key, _value), _M_left(nullptr), _M_right(nullptr), _M_parent(nullptr){}
        inline bool isRoot() const noexcept { return !_M_parent; }
        void rotate(const bool right) noexcept {
            node *p = _M_parent, *g = p->_M_parent;
            if(right){
                if((p->_M_left = _M_right)) _M_right->_M_parent = p;
                _M_right = p, p->_M_parent = this;
            }else{
                if((p->_M_right = _M_left)) _M_left->_M_parent = p;
                _M_left = p, p->_M_parent = this;
            }
            if(!(_M_parent = g)) return;
            if(g->_M_left == p) g->_M_left = this;
            if(g->_M_right == p) g->_M_right = this;
        }
    };
    friend MapIterator<_Key, _Tp>;
    node *root, *_M_header, *start;
    size_t _M_node_count;
    node *splay(node *u) noexcept {
        while(!(u->isRoot())){
            node *p = u->_M_parent, *gp = p->_M_parent;
            if(p->isRoot()){
                u->rotate((u == p->_M_left));
            }else{
                bool flag = (u == p->_M_left);
                if((u == p->_M_left) == (p == gp->_M_left)){
                    p->rotate(flag), u->rotate(flag);
                }else{
                    u->rotate(flag), u->rotate(!flag);
                }
            }
        }
        return root = u;
    }
    static node *increment(node *ver) noexcept {
        if(ver->_M_right){
            ver = ver->_M_right;
            while(ver->_M_left) ver = ver->_M_left;
        }else{
            node *nx = ver->_M_parent;
            while(ver == nx->_M_right) ver = nx, nx = nx->_M_parent;
            ver = nx;
        }
        return ver;
    }
    static node *decrement(node *ver) noexcept {
        if(ver->_M_left){
            ver = ver->_M_left;
            while(ver->_M_right) ver = ver->_M_right;
        }else{
            node *nx = ver->_M_parent;
            while(ver == nx->_M_left) ver = nx, nx = nx->_M_parent;
            ver = nx;
        }
        return ver;
    }
    node *join(node *ver1, node *ver2, const node *ver) noexcept {
        while(ver2->_M_left) ver2 = ver2->_M_left;
        splay(ver2)->_M_left = ver1;
        return ver1 ? (ver1->_M_parent = ver2) : (start = ver2);
    }
    node *_find(const _Key& _key, bool push=false) noexcept {
        node *cur = nullptr, *nx = root;
        do {
            cur = nx;
            if(cur == _M_header || cur->data.first > _key) nx = cur->_M_left;
            else if(cur->data.first < _key) nx = cur->_M_right;
            else return splay(cur);
        }while(nx);
        if(!push) return _M_header;
        if(cur == _M_header || cur->data.first > _key){
            nx = new node(_key, _Tp());
            cur->_M_left = nx, nx->_M_parent = cur;
            if(cur == start) start = nx;
            return _M_node_count++, splay(nx);
        }else{
            nx = new node(_key, _Tp());
            cur->_M_right = nx, nx->_M_parent = cur;
            return _M_node_count++, splay(nx);
        }
    }
    node *_insert(node *ver) noexcept {
        node *cur = nullptr, *nx = root;
        do {
            cur = nx;
            if(cur == _M_header || cur->data.first > ver->data.first) nx = cur->_M_left;
            else nx = cur->_M_right;
        }while(nx);
        if(cur == _M_header || cur->data.first > ver->data.first){
            cur->_M_left = ver, ver->_M_parent = cur;
            if(cur == start) start = ver;
            return _M_node_count++, splay(ver);
        }else{
            cur->_M_right = ver, ver->_M_parent = cur;
            return _M_node_count++, splay(ver);
        }
    }
    node *_erase(node *root_ver){
        assert(root_ver != _M_header);
        if(root_ver->_M_left) root_ver->_M_left->_M_parent = nullptr;
        if(root_ver->_M_right) root_ver->_M_right->_M_parent = nullptr;
        node *res = join(root_ver->_M_left, root_ver->_M_right, root_ver);
        delete root_ver;
        return _M_node_count--, res;
    }
    node *_erase(const _Key& _key){
        node *ver = _find(_key);
        return _erase(ver);
    }
    node *_lower_bound(const _Key& _key) noexcept {
        node *cur = nullptr, *nx = root, *res = nullptr;
        do {
            cur = nx;
            if(cur == _M_header){ nx = cur->_M_left; continue; }
            else if(cur->data.first >= _key){
                nx = cur->_M_left;
                if(!res || cur->data.first <= res->data.first) res = cur;
            }else nx = cur->_M_right;
        }while(nx);
        splay(cur);
        return res ? res : _M_header;
    }
    node *_upper_bound(const _Key& _key) noexcept {
        node *cur = nullptr, *nx = root, *res = nullptr;
        do {
            cur = nx;
            if(cur == _M_header){ nx = cur->_M_left; continue; }
            else if(cur->data.first > _key){
                nx = cur->_M_left;
                if(!res || cur->data.first <= res->data.first) res = cur;
            }else nx = cur->_M_right;
        }while(nx);
        splay(cur);
        return res ? res : _M_header;
    }
    void clear_dfs(node *cur) noexcept {
        if(cur->_M_left) clear_dfs(cur->_M_left);
        if(cur->_M_right) clear_dfs(cur->_M_right);
        delete cur;
    }

public:
    Map() noexcept : _M_node_count(0){ root = _M_header = start = new node(_Key(), _Tp()); }
    ~Map() noexcept { clear_dfs(root); }
    friend ostream& operator<< (ostream& os, Map& mp) noexcept {
        for(auto& val : mp) os << '{' << val.first << ',' << val.second << "} ";
        return os;
    }
    _Tp& operator[](const _Key _key) noexcept { return _find(_key, true)->data.second; }
    const _Tp& at(const _Key& _key){
        node *res = _find(_key);
        if(res == _M_header) __throw_out_of_range(__N("Map::at"));
        return res->data.second;
    }
    size_t size() const noexcept { return _M_node_count; }
    bool empty() const noexcept { return size() == 0; }
    iterator begin() const noexcept { return iterator(start); }
    iterator end() const noexcept { return iterator(_M_header); }
    void clear() noexcept { clear_dfs(root), _M_node_count = 0, root = _M_header = start = new node(_Key(), _Tp()); }
    iterator find(const _Key& _key) noexcept { return iterator(_find(_key)); }
    iterator insert(const _Key& _key, const _Tp& _value) noexcept
        { return iterator(_insert(new node(_key, _value))); }
    iterator erase(const _Key& _key){ return iterator(_erase(_key)); }
    iterator erase(const iterator& itr){ return iterator(_erase(splay(itr.map_ptr))); }
    iterator lower_bound(const _Key& _key) noexcept { return iterator(_lower_bound(_key)); }
    iterator upper_bound(const _Key& _key) noexcept { return iterator(_upper_bound(_key)); }
};

template<typename _Key, typename _Tp>
class MapIterator
    : public std::iterator<bidirectional_iterator_tag, pair<const _Key, _Tp>,
        ptrdiff_t, pair<const _Key, _Tp>*, pair<const _Key, _Tp>&> {
private:
    friend Map<_Key, _Tp>;
    typename Map<_Key, _Tp>::node *map_ptr;
    using base = std::iterator<bidirectional_iterator_tag, pair<const _Key, _Tp>,
        ptrdiff_t, pair<const _Key, _Tp>*, pair<const _Key, _Tp>&>;
    using value_type = typename base::value_type;
    using pointer = typename base::pointer;
    using reference = typename base::reference;

private:
    MapIterator(typename Map<_Key, _Tp>::node *mp) noexcept : map_ptr(mp){}

public:
    MapIterator() noexcept : map_ptr(){}
    MapIterator(const MapIterator& itr) noexcept : map_ptr(itr.map_ptr){}
    MapIterator& operator=(const MapIterator& itr) & { return map_ptr = itr.map_ptr, *this; }
    MapIterator& operator=(const MapIterator&& itr) & noexcept { return map_ptr = itr.map_ptr, *this; }
    reference operator*() const { return map_ptr->data; }
    pointer operator->() const { return &(map_ptr->data); }
    MapIterator& operator++() noexcept { return map_ptr = Map<_Key, _Tp>::increment(map_ptr), *this; }
    MapIterator operator++(int) noexcept { return MapIterator(Map<_Key, _Tp>::increment(this->map_ptr)); }
    MapIterator& operator--() noexcept { return map_ptr = Map<_Key, _Tp>::decrement(map_ptr), *this; }
    MapIterator operator--(int) noexcept { return MapIterator(Map<_Key, _Tp>::decrement(this->map_ptr)); }
    bool operator==(const MapIterator& itr) const noexcept { return !(*this != itr); };
    bool operator!=(const MapIterator& itr) const noexcept { return map_ptr != itr.map_ptr; }
};
