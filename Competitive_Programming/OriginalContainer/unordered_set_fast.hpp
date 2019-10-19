#include "../header.hpp"

template<class _Key, class _Hash, bool DOWNSIZE> class UnorderedSetIterator;

template<class _Key, class _Hash = hash<_Key>, bool DOWNSIZE = false>
class UnorderedSet
{
private:
    using iterator = UnorderedSetIterator<_Key, _Hash, DOWNSIZE>;
    friend UnorderedSetIterator<_Key, _Hash, DOWNSIZE>;
    struct bucket {
        _Key _key;
        short int _dist;
        bool _last, _end;
        bucket() noexcept : _dist(-1), _last(false), _end(false){}
        bucket& operator=(const bucket& another) = default;
        ~bucket(){ if(!empty()) _key.~_Key(); }
        inline void clear() noexcept { _dist = -1; }
        inline void _delete(){ _dist = -1, _key.~_Key(); }
        inline bool empty() const noexcept { return (_dist == -1); }
    };
    inline static unsigned int ceilpow2(unsigned int u) noexcept {
        if(u == 0u) return 0u;
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
    inline float load_rate() const noexcept {
        return (float)_data_count / _bucket_count;
    }
    bucket *insert(bucket *cur, short int dist, _Key&& key){
        bucket *ret = cur;
        bool flag = false;
        while(true){
            if(cur->empty()){
                cur->_key = move(key), cur->_dist = dist;
                if(!flag) ret = cur, flag = true;
                break;
            }else if(dist > cur->_dist){
                swap(key, cur->_key), swap(dist, cur->_dist);
                if(!flag) ret = cur, flag = true;
            }
            ++dist;
            cur = next_bucket(cur);
        }
        return ret;
    }
    bucket *_find(const _Key& key) const {
        bucket *cur = _buckets + (make_hash(key) & _mask);
        int dist = 0;
        while(dist <= cur->_dist){
            if(key == cur->_key) return cur;
            ++dist, cur = next_bucket(cur);
        }
        return _buckets + _bucket_count;
    }
    template<class Key>
    bucket *find_insert(Key&& key){
        unsigned int hash = make_hash(key);
        bucket *cur = _buckets + (hash & _mask);
        int dist = 0;
        while(dist <= cur->_dist){
            if(key == cur->_key) return cur;
            ++dist, cur = next_bucket(cur);
        }
        ++_data_count;
        if(rehash_check()){
            cur = _buckets + (hash & _mask), dist = 0;
        }
        _Key new_key = forward<Key>(key);
        return insert(cur, dist, move(new_key));
    }
    template<typename... Args>
    bucket *emplace(Args&&... args){
        return find_insert(_Key(forward<Args>(args)...));
    }
    bucket *backward_shift(bucket *cur, bool next_ret){
        bucket *next = next_bucket(cur), *ret = cur;
        if(next->_dist < 1) return next_ret ? increment(cur) : cur;
        do {
            cur->_key = next->_key, cur->_dist = next->_dist - 1;
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
        insert(_buckets + (make_hash(cur->_key) & _mask), 0, move(cur->_key));
    }
    void rehash(unsigned int new_bucket_count){
        UnorderedSet new_unordered_set(new_bucket_count);
        new_unordered_set._data_count = _data_count;
        for(bucket *cur = _buckets; !cur->_end; ++cur){
            if(!cur->empty()){
                new_unordered_set.move_data(cur);
            }
        }
        swap(*this, new_unordered_set);
    }
    friend void swap(UnorderedSet& ust1, UnorderedSet& ust2){
        swap(ust1._bucket_count, ust2._bucket_count);
        swap(ust1._mask, ust2._mask);
        swap(ust1._data_count, ust2._data_count);
        swap(ust1._buckets, ust2._buckets);
    }

private:
    unsigned int _bucket_count, _mask, _data_count;
    bucket *_buckets;
public:
    const float MAX_LOAD_RATE = 0.5f;
    const float MIN_LOAD_RATE = 0.1f;
    const unsigned int DOWNSIZE_THRESHOLD = 16u;
    UnorderedSet(unsigned int bucket_size = 0u)
     : _bucket_count(ceilpow2(bucket_size)), _mask(_bucket_count - 1),
        _data_count(0u), _buckets(new bucket[_bucket_count + 1]){
        if(_bucket_count > 0) _buckets[_bucket_count - 1]._last = true;
        else _mask = 0;
        _buckets[_bucket_count]._end = true;
    }
    UnorderedSet(const UnorderedSet& another)
        : _bucket_count(another._bucket_count), _mask(another._mask), _data_count(another._data_count){
        _buckets = new bucket[_bucket_count + 1u];
        for(unsigned int i = 0u; i <= _bucket_count; ++i){
            _buckets[i] = another._buckets[i];
        }
    }
    UnorderedSet(UnorderedSet&& another)
        : _bucket_count(move(another._bucket_count)), _mask(move(another._mask)),
            _data_count(move(another._data_count)), _buckets(another._buckets){
        another._buckets = nullptr;
    }
    UnorderedSet& operator=(const UnorderedSet& another){
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
    UnorderedSet& operator=(UnorderedSet&& another){
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
    ~UnorderedSet(){ delete[] _buckets; }
    friend ostream& operator<< (ostream& os, UnorderedSet& ust) noexcept {
        for(_Key& val : ust) os << val << " ";
        return os;
    }
    void clear(){
        UnorderedSet new_unordered_set(1u);
        swap(*this, new_unordered_set);
    }
    size_t size() const noexcept { return _data_count; }
    size_t bucket_count() const noexcept { return _bucket_count; }
    bool empty() const noexcept { return (_data_count == 0); }
    iterator begin() noexcept {
        return (_buckets->empty() && _bucket_count > 0) ? iterator(increment(_buckets)) : iterator(_buckets);
    }
    iterator end() noexcept { return iterator(_buckets + _bucket_count); }
    iterator find(const _Key& key) const { return iterator(_find(key)); }
    size_t count(const _Key& key) const { return (_find(key) != _buckets + _bucket_count); }
    iterator insert(const _Key& key){ return iterator(find_insert(key)); }
    iterator insert(_Key&& key){ return iterator(find_insert(move(key))); }
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

template<class _Key, class _Hash, bool DOWNSIZE>
class UnorderedSetIterator {
private:
    friend UnorderedSet<_Key, _Hash, DOWNSIZE>;
    typename UnorderedSet<_Key, _Hash, DOWNSIZE>::bucket *bucket_ptr;
    using iterator_category = forward_iterator_tag;
    using value_type = _Key;
    using difference_type = ptrdiff_t;
    using pointer = _Key*;
    using reference = _Key&;

private:
    UnorderedSetIterator(typename UnorderedSet<_Key, _Hash, DOWNSIZE>::bucket *_bucket_ptr)
        noexcept : bucket_ptr(_bucket_ptr){}
public:
    UnorderedSetIterator() noexcept : bucket_ptr(){}
    UnorderedSetIterator(const UnorderedSetIterator& itr) noexcept : bucket_ptr(itr.bucket_ptr){}
    UnorderedSetIterator& operator=(const UnorderedSetIterator& itr)
        & noexcept { return bucket_ptr = itr.bucket_ptr, *this; }
    UnorderedSetIterator& operator=(const UnorderedSetIterator&& itr)
        & noexcept { return bucket_ptr = itr.bucket_ptr, *this; }
    reference operator*() const noexcept { return bucket_ptr->_key; }
    pointer operator->() const noexcept { return &(bucket_ptr->_key); }
    UnorderedSetIterator& operator++() noexcept {
        return bucket_ptr = UnorderedSet<_Key, _Hash, DOWNSIZE>::increment(bucket_ptr), *this;
    }
    UnorderedSetIterator operator++(int) const noexcept {
        return UnorderedSetIterator(UnorderedSet<_Key, _Hash, DOWNSIZE>::increment(this->bucket_ptr));
    }
    bool operator==(const UnorderedSetIterator& itr) const noexcept { return !(*this != itr); };
    bool operator!=(const UnorderedSetIterator& itr) const noexcept { return bucket_ptr != itr.bucket_ptr; }
};
