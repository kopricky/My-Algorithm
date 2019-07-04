#include "../header.hpp"

// 上位集合(含む集合)を包除
void fast_mobius_transformation(int n, vector<int>& f)
{
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < (1 << n); ++j){
            if(!(j & (1 << i))) f[j] -= f[j | (1 << i)];
        }
    }
}

// 下位集合(含まれる集合)を包除
void fast_mobius_transformation(int n, vector<int>& f)
{
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < (1 << n); ++j){
            if(j & (1 << i)) f[j] -= f[j ^ (1 << i)];
        }
    }
}
