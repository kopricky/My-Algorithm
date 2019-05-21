#include "../header.hpp"

#define MAX_N 100005

//iを中心とする最長の回文の半径をR[i]に格納(O(n))
// abaab を $a$b$a$a$b$ みたいにすると偶数長のもの求めることが可能
void manacher(const string& S,vector<int>& res)
{
    int sz = (int)S.size(), i = 0, j = 0, k;
    res.resize(sz);
    while(i < sz){
        while(i-j >= 0 && i+j < sz && S[i-j] == S[i+j]) j++;
        res[i] = j, k = 1;
        while(i-k >= 0 && i+k < sz && k+res[i-k] < j){
            res[i+k] = res[i-k], k++;
        }
        i += k; j -= k;
    }
}
