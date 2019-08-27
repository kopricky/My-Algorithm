#include "./header.hpp"

// 速度効率, メモリ効率ともに悪い.
// ただ素直に vEB 木を実装したので実装の参考にはなると思う.
// 再帰はサイズ 64 で打ち切って unsigned long long で演算を行っている.
class vanEmdeBoasTree
{
private:
    const size_t LENGTH;
    size_t _size;
    struct layer {
        static const unsigned int THRESHOULD = 64;
        unsigned int length, num_blocks, quo, rem;
        int _max, _min;
        unsigned long long data;
        layer *summary, **sublayers;
        layer(const unsigned int length)
            : length(length), num_blocks((int)sqrt(length)),
                quo(length / num_blocks), rem(length % num_blocks),
                    _max(-1), _min(length), data(0ULL){
            if(length <= THRESHOULD) return;
            summary = new layer(num_blocks);
            sublayers = new layer*[num_blocks];
            for(unsigned int i = 0; i < num_blocks; ++i){
                sublayers[i] = new layer(quo + (i < rem));
            }
        }
        layer(const layer& another)
            : length(another.length), num_blocks(another.num_blocks), quo(another.quo), rem(another.rem),
                _max(another._max), _min(another._min), data(another.data){
            if(length <= THRESHOULD) return;
            summary = new layer(*another.summary);
            sublayers = new layer*[num_blocks];
            for(unsigned int i = 0; i < num_blocks; ++i){
                sublayers[i] = new layer(*another.sublayers[i]);
            }
        }
        layer(layer&& another)
            : length(move(another.length)), num_blocks(move(another.num_blocks)),
                quo(move(another.quo)), rem(move(another.rem)),
                    _max(move(another._max)), _min(move(another._min)), data(move(another.data)){
            if(length <= THRESHOULD) return;
            summary = another.summary, sublayers = another.sublayers;
            another.summary = nullptr, another.sublayers = nullptr;
        }
        layer& operator=(const layer& another){
            length = another.length, num_blocks = another.num_blocks, quo = another.quo, rem = another.rem;
            _max = another._max, _min = another._min, data = another.data;
            this->~layer();
            if(length <= THRESHOULD) return *this;
            summary = new layer(*another.summary);
            sublayers = new layer*[num_blocks];
            for(unsigned int i = 0; i < num_blocks; ++i){
                sublayers[i] = new layer(*another.sublayers[i]);
            }
            return *this;
        }
        layer& operator=(layer&& another){
            this->~layer();
            length = move(another.length), num_blocks = move(another.num_blocks);
            quo = move(another.quo), rem = move(another.rem);
            _max = move(another._max), _min = move(another._min), data = move(another.data);
            if(length <= THRESHOULD) return *this;
            summary = another.summary, sublayers = another.sublayers;
            another.summary = nullptr, another.sublayers = nullptr;
            return *this;
        }
        // ~layer(){
        //     if(length <= THRESHOULD) return;
        //     delete summary;
        //     for(unsigned int i = 0; i < num_blocks; ++i){
        //         delete sublayers[i];
        //     }
        //     delete[] sublayers;
        // }
        inline static int msb(unsigned long long u){
            return 63 - __builtin_clzll(u);
        }
        inline static int lsb(unsigned long long u){
            return __builtin_ctzll(u);
        }
        inline bool empty() const noexcept { return (_max == -1); }
        inline bool isOne() const noexcept { return (_max == _min); }
        inline int max() const noexcept { return _max; }
        inline int min() const noexcept { return _min; }
        inline int index(const unsigned int value) const noexcept {
            return (value >= rem * (quo + 1))
                ? (rem + (value - rem * (quo + 1)) / quo) : (value / (quo + 1));
        }
        inline int sum(unsigned int index) const noexcept {
            return (index > rem)
                ? (rem * (quo + 1) + (index - rem) * quo) : (index * (quo + 1));
        }
        bool small_find(const int value) const noexcept {
            return (data >> value) & 1ULL;
        }
        bool find(const int value) const noexcept {
            if(value == _min) return true;
            if(length <= THRESHOULD) return small_find(value);
            const int id = index(value);
            return sublayers[id]->find(value - sum(id));
        }
        bool small_insert(const int value) noexcept {
            if(small_find(value)) return false;
            data ^= (1ULL << value);
            return true;
        }
        bool insert(int value) noexcept {
            if(empty()){
                _max = _min = value;
                return true;
            }else if(value == _min){
                return false;
            }else if(value < _min){
                swap(value, _min);
            }else if(value > _max){
                _max = value;
            }
            if(length <= THRESHOULD) return small_insert(value);
            const int id = index(value);
            if(sublayers[id]->insert(value - sum(id))){
                if(sublayers[id]->isOne()) summary->insert(id);
                return true;
            }else{
                return false;
            }
        }
        void small_erase(const int value){
            if(value == _min){
                _min = lsb(data);
                data ^= (1ULL << _min);
            }else{
                data ^= (1ULL << value);
                if(data == 0) _max = _min;
                else if(value == _max){
                    _max = msb(data);
                }
            }
        }
        void erase(int value){
            if(isOne()){
                _max = -1, _min = length;
                return;
            }
            if(length <= THRESHOULD) return small_erase(value);
            if(value == _min){
                const int id = summary->min();
                 _min = value = sum(id) + sublayers[id]->min();
            }
            const int id = index(value);
            sublayers[id]->erase(value - sum(id));
            if(sublayers[id]->empty()){
                summary->erase(id);
            }
            if(value == _max){
                if(summary->empty()){
                    _max = _min;
                }else{
                    const int id = summary->max();
                    _max = sum(id) + sublayers[id]->max();
                }
            }
        }
        int small_predecessor(const int value) const noexcept {
            const unsigned long long tmp = (data & ((1ULL << value) - 1ULL));
            return tmp ? msb(tmp) : _min;
        }
        int predecessor(const int value) const noexcept {
            if(_min >= value){
                return -1;
            }else if(value > _max){
                return _max;
            }
            if(length <= THRESHOULD) return small_predecessor(value);
            const int id = index(value), sm = sum(id);
            if(value > sm + sublayers[id]->min()){
                return sm + sublayers[id]->predecessor(value - sm);
            }else{
                const int id2 = summary->predecessor(id);
                return (id2 >= 0) ? (sum(id2) + sublayers[id2]->max()) : _min;
            }
        }
        int small_successor(const int value) const noexcept {
            return lsb(data & ~((1ULL << (value + 1)) - 1ULL));
        }
        int successor(const int value) const noexcept {
            if(value < _min){
                return _min;
            }else if(value >= _max){
                return length;
            }
            if(length <= THRESHOULD) return small_successor(value);
            const int id = index(value), sm = sum(id);
            if(value < sm + sublayers[id]->max()){
                return sm + sublayers[id]->successor(value - sm);
            }else{
                const int id2 = summary->successor(id);
                return sum(id2) + sublayers[id2]->min();
            }
        }
    };
    layer base_layer;
public:
    vanEmdeBoasTree(const size_t length) : LENGTH(length), _size(0), base_layer(length){}
    vanEmdeBoasTree(const vanEmdeBoasTree&) = default;
    vanEmdeBoasTree(vanEmdeBoasTree&&) = default;
    vanEmdeBoasTree& operator=(const vanEmdeBoasTree&) = default;
    vanEmdeBoasTree& operator=(vanEmdeBoasTree&&) = default;
    friend ostream& operator<< (ostream& os, vanEmdeBoasTree& veb) noexcept {
        for(unsigned int st = veb.successor(-1); st != veb.LENGTH; st = veb.successor(st))
            os << st << " ";
        return os;
    }
    bool empty() const noexcept { return (_size == 0); }
    size_t size() const noexcept { return _size; }
    size_t max_size() const noexcept { return LENGTH; }
    bool find(const unsigned int value) const noexcept {
        if(value >= LENGTH) return false;
        return base_layer.find(value);
    }
    size_t count(const unsigned int value) const noexcept {
        return find(value);
    }
    int max() const noexcept { return base_layer.max(); }
    int min() const noexcept { return base_layer.min(); }
    void insert(const unsigned int value){
        assert(value < LENGTH);
        _size += base_layer.insert(value);
    }
    void erase(const unsigned int value){
        assert(value < LENGTH);
        base_layer.erase(value), --_size;
    }
    int predecessor(const int value) const noexcept {
        return base_layer.predecessor(value);
    }
    int successor(const int value) const noexcept {
        return base_layer.successor(value);
    }
};
