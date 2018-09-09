#include "../header.hpp"

//各iについてS[0:i]の最小周期長(っぽいもの)を求める(O(n))
//例えばababaの最小周期長は5ではなく2となるので注意
vector<int> table,res;
void minimum_loop(const string s){
    int slen = (int)s.size();
    table.resize(slen+1);
    table[0] = -1;
    int j = -1;
    for(int i = 0; i < slen; i++){
        while(j >= 0 && s[i] != s[j]){
            j = table[j];
        }
        table[i+1] = ++j;
    }
    res.resize(slen);
    for(int i = 0; i < slen; i++){
        res[i] = i+1-table[i+1];
    }
}
