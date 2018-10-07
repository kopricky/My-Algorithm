#include "../header.hpp"

#define MAX_N 2000

class mat {
public:
    bitset<MAX_N>* a;
    mat(){ a = new bitset<MAX_N>[MAX_N]; }
    mat operator*(const mat& m) const {
        mat ans;
        for(int i = 0; i < MAX_N; i++){
            for(int j = 0; j < MAX_N; j++){
                if(this->a[i][j] == 0) continue;
                ans.a[i] |= m.a[j];
            }
        }
        return ans;
    }
};
