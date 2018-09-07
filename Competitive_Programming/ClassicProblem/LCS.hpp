#include "../header.hpp"

//最長共通部分列の長さを求める(O(nlogn))

#define INF 1000000005
const int MAX_SIZE = 26;

int LCS(const string& a, const string& b){
    int n = (int)a.size(), m = (int)b.size();
    vector<int> x(n), y(m);
    for(int i = 0; i < n; i++){
        x[i] = a[i]-'a';
    }
    for(int i = 0; i < m; i++){
        y[i] = b[i]-'a';
    }
    vector<vector<int> > vec(MAX_SIZE);
    for(int j = m-1; j >= 0; --j){
        vec[y[j]].push_back(j);
    }
    vector<int> xs(n, INF);
    for(int i = 0; i < n; i++){
        for(int val : vec[x[i]]){
            *lower_bound(xs.begin(),xs.end(),val) = val;
        }
    }
    return lower_bound(xs.begin(),xs.end(),INF) - xs.begin();
}
