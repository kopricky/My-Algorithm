#include "../header.hpp"

// pair に対する std::hash を用いたハッシュ関数
struct pair_hash {
    template <class C1, class C2>
    unsigned int operator() (const pair<C1, C2>& p) const {
        unsigned int lhs = hash<C1>()(p.first), rhs = hash<C2>()(p.second);
        return lhs^(rhs+0x9e3779b9+(lhs<<6)+(lhs>>2));
    }
};

// vector に対する std::hash を用いたハッシュ関数
struct vector_hash {
    const unsigned int random = mt19937(random_device{}())();
    template <class C>
    unsigned int operator()(const vector<C>& p) const {
        unsigned int h = (random ^ p.size());
        for(const C& k : p){
            h ^= hash<C>()(k)+0x9e3779b9+(h<<6)+(h>>2);
        }
        return h;
    }
};

// int に対する murmur hash
struct murmur_hash_int32 {
    unsigned int operator()(int p) const {
        const unsigned int m = 0x5bd1e995; p *= m;
        unsigned int h = (p^(p>>24))*m;
        return h = (h^(h>>13))*m, (h^(h>>15));
    }
};

// long long に対する murmur hash
struct murmur_hash_int64 {
    unsigned long long operator()(long long p) const {
        const unsigned long long m = 0xc6a4a7935bd1e995; p *= m;
        unsigned long long h = (p^(p>>47))*m;
        return h = (h^(h>>47))*m, (h^(h>>47));
    }
};

// vector<int> に対する murmur hash
struct murmur_hash32 {
    const unsigned int random = random_device{}();
    unsigned int operator()(const vector<int>& p) const {
        const unsigned int m = 0x5bd1e995;
        unsigned int h = (random ^ (4 * p.size()));
        for(unsigned int k : p){
            k *= m, h = (h*m)^((k^(k>>24))*m);
        }
        h = (h^(h>>13))*m;
        return (h^(h>>15));
    }
};

// vector<long long> に対する murmur hash
struct murmur_hash64 {
    const unsigned int random = random_device{}();
    unsigned long long operator()(const vector<long long>& p) const {
        const unsigned long long m = 0xc6a4a7935bd1e995;
        unsigned long long h = (random ^ (4 * p.size()));
        for(unsigned long long k : p){
            k *= m, h = (h*m)^((k^(k>>47))*m);
        }
        h = (h^(h>>47))*m;
        return (h^(h>>47));
    }
};
