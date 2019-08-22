#include "../header.hpp"

template<class _Key, class _Tp, class _Hash, bool DOWNSIZE> class UnorderedMapIterator;

template<class _Key, class _Tp, class _Hash = hash<_Key>, bool DOWNSIZE = false>
class UnorderedMap
{
private:
    using iterator = UnorderedMapIterator<_Key, _Tp, _Hash, DOWNSIZE>;
    using data_type = pair<_Key, _Tp>;
    using aligned_pointer = typename aligned_storage<sizeof(data_type), alignof(data_type)>::type;
    friend UnorderedMapIterator<_Key, _Tp, _Hash, DOWNSIZE>;
    struct bucket {
        unsigned int _hash;
        short int _dist;
        bool _last, _end;
        aligned_pointer _data_ptr;
        bucket() noexcept : _dist(-1), _last(false), _end(false){}
        ~bucket(){ if(!empty()) _delete(); }
        inline void clear() noexcept { _dist = -1; }
        inline void _delete(){ _dist = -1, data_ptr()->~data_type(); }
        inline bool empty() const noexcept { return (_dist == -1); }
        inline data_type& data() noexcept {
            return *reinterpret_cast<data_type*>(&_data_ptr);
        }
        inline data_type* data_ptr() noexcept {
            return reinterpret_cast<data_type*>(&_data_ptr);
        }
        inline const _Key& get_key() noexcept { return data_ptr()->first; }
        inline void new_data(const data_type& data){
            new(&_data_ptr) data_type(data);
        }
    };
    inline static unsigned int ceilpow2(unsigned int u) noexcept {
        --u, u |= u >> 1, u |= u >> 2, u |= u >> 4, u |= u >> 8;
        return (u | (u >> 16)) + 1;
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
    inline float load_factor() const noexcept {
        return (float)_data_count / _bucket_count;
    }
    bucket *insert_impl(bucket *cur, unsigned int hash, short int dist, data_type data){
        bucket *ret = cur;
        bool flag = false;
        while(true){
            if(cur->empty()){
                cur->_hash = hash, cur->_dist = dist, cur->new_data(data);
                if(!flag) ret = cur, flag = true;
                break;
            }else if(dist > cur->_dist){
                swap(hash, cur->_hash), swap(dist, cur->_dist), swap(data, cur->data());
                if(!flag) ret = cur, flag = true;
            }
            ++dist;
            cur = next_bucket(cur);
        }
        return ret;
    }
    bucket *_find(const _Key& key, bool push=false){
        unsigned int hash = make_hash(key);
        bucket *cur = _buckets + (hash & _mask);
        short int dist = 0;
        while(dist <= cur->_dist){
            if(hash == cur->_hash && key == cur->get_key()){
                return cur;
            }
            ++dist;
            cur = next_bucket(cur);
        }
        if(!push) return _buckets + _bucket_count;
        ++_data_count;
        if(rehash_check()){
            hash = make_hash(key), cur = _buckets + (hash & _mask), dist = 0;
        }
        return insert_impl(cur, hash, dist, data_type(key, _Tp()));
    }
    bucket *_insert(const data_type& data){
        const _Key& key = data.first;
        unsigned int hash = make_hash(key);
        bucket *cur = _buckets + (make_hash(hash) & _mask);
        short int dist = 0;
        while(dist <= cur->_dist){
            if(hash == cur->_hash && key == cur->get_key()){
                return cur;
            }
            ++dist;
            cur = next_bucket(cur);
        }
        ++_data_count;
        if(rehash_check()){
            hash = make_hash(key), cur = _buckets + (hash & _mask), dist = 0;
        }
        return insert_impl(cur, hash, dist, data);
    }
    bucket *backward_shift(bucket *cur, bool next_ret){
        bucket *next = next_bucket(cur), *ret = cur;
        if(next->_dist < 1) return next_ret ? increment(cur) : cur;
        do {
            cur->_hash = next->_hash;
            cur->_dist = next->_dist - 1;
            cur->_data_ptr = next->_data_ptr;
            cur = next, next = next_bucket(cur);
        }while(next->_dist >= 1);
        cur->clear();
        return ret;
    }
    bucket *erase_impl(bucket *cur, bool next_ret){
        assert(cur != _buckets + _bucket_count);
        cur->_delete();
        return backward_shift(cur, next_ret);
    }
    bucket *erase_itr(bucket *cur, bool next_ret = true){
        --_data_count;
        const _Key& key = cur->get_key();
        return erase_impl(rehash_check() ? _find(key) : cur, next_ret);
    }
    bucket *erase_key(const _Key& key, bool next_ret = true){
        --_data_count;
        rehash_check();
        return erase_impl(_find(key), next_ret);
    }
    bool rehash_check(){
        if(load_factor() >= MAX_LOAD_FACTOR){
            rehash(_bucket_count * 2);
            return true;
        }else if(DOWNSIZE){
            if(load_factor() <= MIN_LOAD_FACTOR && _bucket_count >= DOWNSIZE_THRESHOLD){
                rehash(_bucket_count / 2);
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
    void move_data(aligned_pointer data_ptr){
        unsigned int hash = make_hash(reinterpret_cast<data_type*>(&data_ptr)->first);
        bucket *cur = _buckets + (hash & _mask);
        short int dist = 0;
        while(true){
            if(cur->empty()){
                cur->_hash = hash, cur->_dist = dist, cur->_data_ptr = data_ptr;
                break;
            }else if(dist > cur->_dist){
                swap(hash, cur->_hash), swap(dist, cur->_dist), swap(data_ptr, cur->_data_ptr);
            }
            ++dist;
            cur = next_bucket(cur);
        }
    }
    void rehash(unsigned int new_bucket_count){
        UnorderedMap new_unordered_map(new_bucket_count);
        new_unordered_map._data_count = _data_count;
        for(bucket *cur = _buckets; !cur->_end; ++cur){
            if(!cur->empty()){
                new_unordered_map.move_data(cur->_data_ptr);
                cur->clear();
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
    const float MAX_LOAD_FACTOR = 0.5f;
    const float MIN_LOAD_FACTOR = 0.1f;
    const unsigned int DOWNSIZE_THRESHOLD = 16u;
    UnorderedMap(unsigned int bucket_size = 1u)
     : _bucket_count(ceilpow2(max(bucket_size, 1u))), _mask(_bucket_count - 1),
        _data_count(0u), _buckets(new bucket[_bucket_count + 1]){
        _buckets[_bucket_count - 1]._last = true, _buckets[_bucket_count]._end = true;
    }
    ~UnorderedMap(){ delete[] _buckets; }
    friend ostream& operator<< (ostream& os, UnorderedMap& ump) noexcept {
        for(data_type& val : ump) os << '{' << val.first << ',' << val.second << "} ";
        return os;
    }
    _Tp& operator[](const _Key& key){
        return _find(key, true)->data_ptr()->second;
    }
    const _Tp& at(const _Key& key){
        bucket *res = _find(key);
        if(res == _buckets + _bucket_count) __throw_out_of_range(__N("Unordered_Map::at"));
        return res->data_ptr()->second;
    }
    void clear(){
        UnorderedMap new_unordered_set(1u);
        swap(*this, new_unordered_set);
    }
    size_t size() const noexcept { return _data_count; }
    size_t bucket_count() const noexcept { return _bucket_count; }
    bool empty() const noexcept { return (_data_count == 0); }
    iterator begin() const noexcept { return _buckets->empty() ? iterator(increment(_buckets)) : iterator(_buckets); }
    iterator end() const noexcept { return iterator(_buckets + _bucket_count); }
    iterator find(const _Key& key){ return iterator(_find(key)); }
    iterator insert(const data_type& data){ return iterator(_insert(data)); }
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
    using value_type = pair<_Key, _Tp>;
    using difference_type = ptrdiff_t;
    using pointer = pair<_Key, _Tp>*;
    using reference = pair<_Key, _Tp>&;

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
    reference operator*() const noexcept { return bucket_ptr->data(); }
    pointer operator->() const noexcept { return bucket_ptr->data_ptr(); }
    UnorderedMapIterator& operator++() noexcept {
        return bucket_ptr = UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>::increment(bucket_ptr), *this;
    }
    UnorderedMapIterator operator++(int) const noexcept {
        return UnorderedMapIterator(UnorderedMap<_Key, _Tp, _Hash, DOWNSIZE>::increment(this->bucket_ptr));
    }
    bool operator==(const UnorderedMapIterator& itr) const noexcept { return !(*this != itr); };
    bool operator!=(const UnorderedMapIterator& itr) const noexcept { return bucket_ptr != itr.bucket_ptr; }
};
