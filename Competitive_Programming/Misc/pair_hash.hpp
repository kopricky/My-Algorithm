#include "../header.hpp"

struct pair_hash {
    template <class T1, class T2>
    size_t operator() (const pair<T1, T2>& p) const {
        size_t lhs = hash<T1>()(p.first), rhs = hash<T2>()(p.second);
        return lhs^(rhs+0x9e3779b9+(lhs<<6)+(lhs>>2));
    }
};
