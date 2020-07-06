#include "../header.hpp"

// S[0:i] の接頭辞と接尾辞が最大何文字一致しているかを表す配列 res[i] を線形時間で構築するアルゴリズム
void MP(const string& S, vector<int>& res){
    int n = (int)S.size(), j = 0;
    res.resize(n, 0);
    for(int i = 0; i < n-1; i++){
        while(S[i+1] != S[j] && --j >= 0) j = res[j];
        res[i+1] = ++j;
    }
}
