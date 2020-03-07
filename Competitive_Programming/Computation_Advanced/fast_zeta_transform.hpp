#include "../header.hpp"

// 上位集合(含む集合)の和
void fast_zeta_transform(int n, vector<int>& f)
{
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < (1 << n); ++j){
            if(!(j >> i & 1)) f[j] += f[j ^ (1 << i)];
        }
    }
}

// 下位集合(含まれる集合)の和
void fast_zeta_transform(int n, vector<int>& f)
{
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < (1 << n); ++j){
            if(j >> i & 1) f[j] += f[j ^ (1 << i)];
        }
    }
}