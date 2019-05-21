#include "../header.hpp"

bool IsExist(vector<int> d){
    int n = (int)d.size();
    vector<long long> sm(n+1, 0);
    sort(d.begin(), d.end(), greater<int>());
    for(int i = 0; i < n; i++) sm[i+1] = sm[i] + d[i];
    if(sm[n] % 2) return false;
    int pos = n-1;
    for(int i = 0; i < n; i++){
        while(pos >= 0 && d[pos] < i+1) pos--;
        if(sm[i+1]>(long long)(i+1)*i+sm[n]-sm[max(i, pos)+1]+(long long)(i+1)*max(pos-i,0)){
            return false;
        }
    }
    return true;
}
