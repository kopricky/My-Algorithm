#include "../header.hpp"

vector<bool> v(n.false);
fill(v.end() - r, v.end(), true);
do {
    vector<int> res;
    for(int i = 0; i < n; i++){
        if(v[i]){
            res.push_back(i);
        }
    }
} while (next_permutation(v.begin(), v.end()));
