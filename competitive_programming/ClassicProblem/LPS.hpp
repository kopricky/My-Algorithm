#include "../header.hpp"
//最長部分回文列を求める(O(n^2))
int LPS(const string& s)
{
    int n = (int)s.size();
    vector<vector<int> > dp(n,vector<int>(n+1,0));
    rep(i,n){
        dp[i][i+1] = 1;
    }
    //iは部分文字列の長さ
    for(int i = 2; i <= n; i++){
        rep(j,n-i+1){
            int k = i + j;
            if(s[j] == s[k-1]){
                dp[j][k] = dp[j+1][k-1] + 2;
            }else{
                dp[j][k] = max(dp[j][k-1], dp[j+1][k]);
            }
        }
    }
    return dp[0][n];
    // 最長部分回文列の出力
    // string res;
    // int ni = 0, nj = n;
    // while(nj-ni > 1){
    //     if(s[ni] == s[nj-1]){
    //         res.push_back(s[ni]);
    //         ni++,nj--;
    //     }else{
    //         if(dp[ni][nj] == dp[ni][nj-1]){
    //             nj--;
    //         }else{
    //             ni++;
    //         }
    //     }
    // }
    // if(nj-ni){
    //     string tmp = res;
    //     res.push_back(s[ni]);
    //     reverse(tmp.begin(),tmp.end());
    //     res += tmp;
    // }else{
    //     string tmp = res;
    //     reverse(tmp.begin(),tmp.end());
    //     res += tmp;
    // }
}
