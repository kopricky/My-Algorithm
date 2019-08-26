#include "./header.hpp"

class vanEmdeBoasTree
{
private:
    const size_t LENGTH;
    size_t _size;
    struct layer {
        static const unsigned int THRESHOULD = 64;
        const unsigned int length, num_blocks, quo, rem;
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
            _max = (_max >= value) ? _max : value;
            _min = (_min <= value) ? _min : value;
            data ^= (1ULL << value);
            return true;
        }
        bool insert(int value) noexcept {
            if(value == _min) return false;
            if(length <= THRESHOULD) return small_insert(value);
            if(empty()){
                _max = _min = value;
                return true;
            }else if(value < _min){
                swap(value, _min);
            }else if(value > _max){
                _max = value;
            }
            const int id = index(value);
            bool res = sublayers[id]->insert(value - sum(id));
            if(res && sublayers[id]->isOne()){
                summary->insert(id);
            }
            return res;
        }
        void small_erase(const int value){
            data ^= (1ULL << value);
            if(data == 0) _max = -1, _min = length;
            else _max = msb(data), _min = lsb(data);
        }
        void erase(int value){
            if(length <= THRESHOULD) return small_erase(value);
            if(isOne()){
                _max = -1, _min = length;
                return;
            }else if(value == _min){
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
            return msb(data & ((1ULL << value) - 1ULL));
        }
        int predecessor(const int value) const noexcept {
            if(length <= THRESHOULD) return small_predecessor(value);
            if(_min >= value){
                return -1;
            }else if(value > _max){
                return _max;
            }
            const int id = index(value), sm = sum(id);
            if(value > sm + sublayers[id]->min()){
                return sm + sublayers[id]->predecessor(value - sm);
            }else{
                const int id2 = summary->predecessor(id);
                return sum(id2) + sublayers[id2]->max();
            }
        }
        int small_successor(const int value) const noexcept {
            return lsb(data & ~((1ULL << (value + 1)) - 1ULL));
        }
        int successor(const int value) const noexcept {
            if(length <= THRESHOULD) return small_successor(value);
            if(value < _min){
                return _min;
            }else if(value >= _max){
                return length;
            }
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
    vanEmdeBoasTree(const size_t length)
        : LENGTH(length), _size(0), base_layer(length){}
    friend ostream& operator<< (ostream& os, vanEmdeBoasTree& veb) noexcept {
        for(unsigned int st = veb.successor(-1); st != veb.LENGTH; st = veb.successor(st))
            os << st << " ";
        return os;
    }
    bool empty() const noexcept { return (_size == 0); }
    size_t size() const noexcept { return _size; }
    bool find(const unsigned int value) const noexcept {
        if(value >= LENGTH) return false;
        return base_layer.find(value);
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
