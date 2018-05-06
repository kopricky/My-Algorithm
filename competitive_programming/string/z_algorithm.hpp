#include "../header.hpp"

//各iについてS と S[i:|S|-1] の最長共通接頭辞の長さをA[i]に記録する(O(n))
void z_algorithm(string& S,vector<int>& res)
{
    int sz = (int)S.size();
    res.resize(sz);
    int c = 0;
    for(int i = 1; i < sz; i++){
        if(i+res[i-c] < c+res[c]){
            res[i] = res[i-c];
        }else{
            int j = max(0, c+res[c]-i);
            while (i+j < sz && S[j] == S[i+j]) ++j;
            res[i] = j;
        }
    }
    res[0] = sz;
}
