#include "./header.hpp"

class vanEmdeBoasTree
{
private:
    static const uint32_t  LENGTH = 1073741824;
    uint32_t  _size;
    #define msb(u) (63 - __builtin_clzll(u))
    #define lsb(u) (__builtin_ctzll(u))
    struct first_layer;
    struct middle_layer;
    struct last_layer;
    struct first_layer {
        uint64_t *data;
        middle_layer *summary;
        int32_t _max, _min;
        first_layer() : _max(-1), _min(1073741824){
            data = new uint64_t[16777216](), summary = new middle_layer();
        }
        first_layer(const first_layer& another) : _max(another._max), _min(another._min){
            data = new uint64_t[16777216];
            for(uint32_t i = 0; i < 16777216; ++i) data[i] = another.data[i];
            summary = new middle_layer(*another.summary);
        }
        first_layer(first_layer&& another)
            : _max(move(another._max)), _min(move(another._min)){
            data = another.data, summary = another.summary;
            data = nullptr, summary = nullptr;
        }
        first_layer& operator=(const first_layer& another){
            this->~first_layer();
            _max = another._max, _min = another._min;
            data = new uint64_t[16777216];
            for(uint32_t i = 0; i < 16777216; ++i) data[i] = another.data[i];
            summary = new middle_layer(*another.summary);
            return *this;
        }
        first_layer& operator=(first_layer&& another){
            this->~first_layer();
            _max = move(another._max), _min = move(another._min);
            data = another.data, summary = another.summary;
            data = nullptr, summary = nullptr;
            return *this;
        }
        // ~first_layer(){
        //     delete summary;
        //     delete[] data;
        // }
        inline bool empty() const noexcept { return (_max == -1); }
        inline bool isOne() const noexcept { return (_max == _min); }
        inline int32_t max() const noexcept { return _max; }
        inline int32_t min() const noexcept { return _min; }
        bool find(const int32_t value) const noexcept {
            return (value == _min) || ((data[value >> 6] >> (value & 63)) & 1ULL);
        }
        bool insert(int32_t value) noexcept {
            if(value == _min) return false;
            else if(_max == -1) return _max = _min = value, true;
            else if(value < _min) swap(value, _min);
            else if(value > _max) _max = value;
            const int32_t id = (value >> 6);
            if((data[id] >> (value & 63)) & 1ULL) return false;
            else{
                if(data[id] == 0) summary->insert(id);
                data[id] ^= (1ULL << (value & 63));
                return true;
            }
        }
        void erase(int32_t value) noexcept {
            if(_max == _min){
                _max = -1, _min = 1073741824;
                return;
            }else if(value == _min){
                const int32_t id = summary->min();
                 _min = value = (id << 6) + lsb(data[id]);
            }
            const int32_t id = (value >> 6);
            data[id] ^= (1ULL << (value & 63));
            if(data[id] == 0) summary->erase(id);
            if(value == _max){
                if(summary->empty()) _max = _min;
                else{
                    const int32_t id = summary->max();
                    _max = (id << 6) + msb(data[id]);
                }
            }
        }
        int32_t predecessor(const int32_t value) const noexcept {
            if(_min >= value) return -1;
            else if(value > _max) return _max;
            const int32_t id = (value >> 6), sm = (id << 6);
            if(data[id] && value > sm + lsb(data[id]))
                return sm + msb(data[id] & ((1ULL << (value & 63)) - 1ULL));
            else{
                const int32_t id2 = summary->predecessor(id);
                return (id2 >= 0) ? ((id2 << 6) + msb(data[id2])) : _min;
            }
        }
        int32_t successor(const int32_t value) const noexcept {
            if(value < _min) return _min;
            else if(value >= _max) return 1073741824;
            const int32_t id = (value >> 6), sm = (id << 6);
            if(data[id] && value < sm + msb(data[id]))
                return sm + lsb(data[id] & ~((1ULL << ((value & 63) + 1)) - 1ULL));
            else{
                const int32_t id2 = summary->successor(id);
                return (id2 << 6) + lsb(data[id2]);
            }
        }
    };
    struct middle_layer{
        last_layer *sublayers, *summary;
        int32_t _max, _min;
        middle_layer() : _max(-1), _min(16777216){
            sublayers = new last_layer[4096](), summary = new last_layer();
        }
        middle_layer(const middle_layer& another) : _max(another._max), _min(another._min){
            sublayers = new last_layer[4096];
            for(uint32_t i = 0; i < 4096; ++i)
                sublayers[i] = last_layer(another.sublayers[i]);
            summary = new last_layer(*another.summary);
        }
        middle_layer(middle_layer&& another)
            : _max(move(another._max)), _min(move(another._min)){
            sublayers = another.sublayers, summary = another.summary;
            another.sublayers = another.summary = nullptr;
        }
        middle_layer& operator=(const middle_layer& another){
            this->~middle_layer();
            _max = another._max, _min = another._min;
            sublayers = new last_layer[4096];
            for(uint32_t i = 0; i < 4096; ++i)
                sublayers[i] = last_layer(another.sublayers[i]);
            summary = new last_layer(*another.summary);
            return *this;
        }
        middle_layer& operator=(middle_layer&& another){
            this->~middle_layer();
            _max = move(another._max), _min = move(another._min);
            sublayers = another.sublayers, summary = another.summary;
            another.sublayers = another.summary = nullptr;
            return *this;
        }
        // ~middle_layer(){
        //     delete summary;
        //     delete[] sublayers;
        // }
        inline bool empty() const noexcept { return (_max == -1); }
        inline bool isOne() const noexcept { return (_max == _min); }
        inline int32_t max() const noexcept { return _max; }
        inline int32_t min() const noexcept { return _min; }
        bool insert(int32_t value) noexcept {
            if(value == _min) return false;
            else if(_max == -1) return _max = _min = value, true;
            else if(value < _min) swap(value, _min);
            else if(value > _max) _max = value;
            const int32_t id = (value >> 12);
            if(sublayers[id].insert(value & 4095)){
                if(sublayers[id].isOne()) summary->insert(id);
                return true;
            }else return false;
        }
        void erase(int32_t value) noexcept {
            if(_max == _min){
                _max = -1, _min = 16777216;
                return;
            }else if(value == _min){
                const int32_t id = summary->min();
                 _min = value = (id << 12) + sublayers[id].min();
            }
            const int32_t id = (value >> 12);
            sublayers[id].erase(value & 4095);
            if(sublayers[id].empty()) summary->erase(id);
            if(value == _max){
                if(summary->empty()) _max = _min;
                else{
                    const int32_t id = summary->max();
                    _max = (id << 12) + sublayers[id].max();
                }
            }
        }
        int32_t predecessor(const int32_t value) const noexcept {
            if(_min >= value) return -1;
            else if(value > _max) return _max;
            const int32_t id = (value >> 12), sm = (id << 12);
            if(value > sm + sublayers[id].min()){
                return sm + sublayers[id].predecessor(value & 4095);
            }else{
                const int32_t id2 = summary->predecessor(id);
                return (id2 >= 0) ? ((id2 << 12) + sublayers[id2].max()) : _min;
            }
        }
        int32_t successor(const int32_t value) const noexcept {
            if(value < _min) return _min;
            else if(value >= _max) return 16777216;
            const int32_t id = (value >> 12), sm = (id << 12);
            if(value < sm + sublayers[id].max()){
                return sm + sublayers[id].successor(value & 4095);
            }else{
                const int32_t id2 = summary->successor(id);
                return (id2 << 12) + sublayers[id2].min();
            }
        }
    };
    struct last_layer{
        uint64_t data[64], summary;
        int32_t _max, _min;
        last_layer() noexcept : summary(0ULL), _max(-1), _min(4096){
            memset(data, 0, sizeof(data));
        }
        inline bool empty() const noexcept { return (_max == -1); }
        inline bool isOne() const noexcept { return (_max == _min); }
        inline int32_t max() const noexcept { return _max; }
        inline int32_t min() const noexcept { return _min; }
        bool insert(int32_t value) noexcept {
            if(value == _min) return false;
            else if(_max == -1) return _max = _min = value, true;
            else if(value < _min) swap(value, _min);
            else if(value > _max) _max = value;
            const int32_t id = (value >> 6);
            if((data[id] >> (value & 63)) & 1ULL) return false;
            else{
                data[id] ^= (1ULL << (value & 63)), summary |= (1ULL << id);
                return true;
            }
        }
        void erase(int32_t value) noexcept {
            if(_max == _min){
                _max = -1, _min = 4096;
                return;
            }else if(value == _min){
                const int32_t id = lsb(summary);
                _min = value = (id << 6) + lsb(data[id]);
            }
            const int32_t id = (value >> 6);
            data[id] ^= (1ULL << (value & 63));
            if(data[id] == 0) summary ^= (1ULL << id);
            if(value == _max){
                if(summary == 0) _max = _min;
                else{
                    const int32_t id = msb(summary);
                    _max = (id << 6) + msb(data[id]);
                }
            }
        }
        int32_t predecessor(const int32_t value) const noexcept {
            if(_min >= value) return -1;
            else if(value > _max) return _max;
            const int32_t id = (value >> 6), sm = (id << 6);
            if(data[id] && value > sm + lsb(data[id]))
                return sm + msb(data[id] & ((1ULL << (value & 63)) - 1ULL));
            else{
                const uint64_t tmp = (summary & ((1ULL << id) - 1ULL));
                if(tmp == 0ULL) return _min;
                else{
                    const int32_t id2 = msb(tmp);
                    return (id2 << 6) + msb(data[id2]);
                }
            }
        }
        int32_t successor(const int32_t value) const noexcept {
            if(value < _min) return _min;
            else if(value >= _max) return 4096;
            const int32_t id = (value >> 6), sm = (id << 6);
            if(data[id] && value < sm + msb(data[id]))
                return sm + lsb(data[id] & ~((1ULL << ((value & 63) + 1)) - 1ULL));
            else{
                const int32_t id2 = lsb(summary & ~((1ULL << (id + 1)) - 1ULL));
                return (id2 << 6) + lsb(data[id2]);
            }
        }
    };
    first_layer base_layer;
public:
    vanEmdeBoasTree() : _size(0), base_layer(){}
    vanEmdeBoasTree(const vanEmdeBoasTree&) = default;
    vanEmdeBoasTree(vanEmdeBoasTree&&) = default;
    vanEmdeBoasTree& operator=(const vanEmdeBoasTree&) = default;
    vanEmdeBoasTree& operator=(vanEmdeBoasTree&&) = default;
    friend ostream& operator<< (ostream& os, vanEmdeBoasTree& veb) noexcept {
        for(uint32_t st = veb.successor(-1); st != veb.LENGTH; st = veb.successor(st))
            os << st << " ";
        return os;
    }
    bool empty() const noexcept { return (_size == 0); }
    uint32_t  size() const noexcept { return _size; }
    uint32_t  max_size() const noexcept { return LENGTH; }
    bool find(const uint32_t value) const noexcept {
        if(value >= LENGTH) return false;
        return base_layer.find(value);
    }
    uint32_t  count(const uint32_t value) const noexcept {
        return find(value);
    }
    int32_t max() const noexcept { return base_layer.max(); }
    int32_t min() const noexcept { return base_layer.min(); }
    void insert(const uint32_t value){
        assert(value < LENGTH);
        _size += base_layer.insert(value);
    }
    void erase(const uint32_t value){
        assert(value < LENGTH);
        base_layer.erase(value), --_size;
    }
    int32_t predecessor(const int32_t value) const noexcept {
        return base_layer.predecessor(value);
    }
    int32_t successor(const int32_t value) const noexcept {
        return base_layer.successor(value);
    }
};
