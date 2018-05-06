#include "../header.hpp"

vector<bool> v(n.false);
fill(v.end() - r, v.end(), true);
do {
    vector<int> res;
    rep(i,n) {
        if (v[i]) {
            res.push_back(i);
        }
    }
} while (next_permutation(all(v)));
