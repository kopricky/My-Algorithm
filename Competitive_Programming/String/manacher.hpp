#include "../header.hpp"

#define MAX_N 100005

//iを中心とする最長の回文の半径をR[i]に格納(O(n))
// abaab を $a$b$a$a$b$ みたいにすると偶数長のもの求めることが可能
void manacher(string& S,vector<int>& res)
{
    int sz = (int)S.size();
    res.resize(sz);
    int np=0,nr=0;
    while(np < sz){
        while(np-nr >= 0 && np+nr < sz && S[np-nr] == S[np+nr]){
            nr++;
        }
        res[np] = nr;
        int k = 1;
        while(np-k >= 0 && np+k < sz && k+res[np-k] < nr){
            res[np+k] = res[np-k];
            k++;
        }
        np += k; nr -= k;
    }
}
