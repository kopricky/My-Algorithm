#include "../header.hpp"

struct pair_hash {
    template <class C1, class C2>
    unsigned int operator() (const pair<C1, C2>& p) const {
        unsigned int lhs = hash<C1>()(p.first), rhs = hash<C2>()(p.second);
        return lhs^(rhs+0x9e3779b9+(lhs<<6)+(lhs>>2));
    }
};

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

struct murmur_hash32 {
    const unsigned int random = random_device{}();
    unsigned int operator()(const vector<int>& p) const {
        const unsigned int m = 0x5bd1e995;
        unsigned int h = (random ^ (4 * p.size()));
        for(unsigned int k : p){
            k *= m, k = (k^(k>>24))*m, h = (h*m)^k;
        }
        h = (h^(h>>13))*m;
        return (h^(h>>15));
    }
};

struct murmur_hash64 {
    const unsigned int random = random_device{}();
    unsigned long long operator()(const vector<long long>& p) const {
        const unsigned long long m = 0xc6a4a7935bd1e995;
        unsigned long long h = (random ^ (4 * p.size()));
        for(unsigned long long k : p){
            k *= m, k = (k^(k>>47))*m, h = (h*m)^k;
        }
        h = (h^(h>>47))*m;
        return (h^(h>>47));
    }
};
