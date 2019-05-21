#include "../header.hpp"

//各iについてS と S[i:|S|-1] の最長共通接頭辞の長さをA[i]に記録する(O(n))
void z_algorithm(const string& S, vector<int>& res)
{
    int sz = (int)S.size();
    res.resize(sz, 0);
    int i = 1, j = 0;
    while(i < sz){
        while(i+j < sz && S[j] == S[i+j]) j++;
        res[i] = j;
        if(j == 0){
            i++;
            continue;
        }
        int k = 1;
        while(i+k < sz && k+res[k] < j){
            res[i+k] = res[k], k++;
        }
        i += k; j -= k;
    }
}
