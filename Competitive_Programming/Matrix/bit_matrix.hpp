#include "../header.hpp"

#define MAX_N 2000

struct mat
{
    bitset<MAX_N> a[MAX_N];
};

mat operator*(const mat& m1,const mat& m2){
    mat ans;
    for(int i = 0; i < MAX_N; i++){
        for(int j = 0; j < MAX_N; j++){
            if(m1.a[i][j] == 0) continue;
            ans.a[i] |= m2.a[j];
        }
    }
    return ans;
}
