//最長部分回文列を求める(O(n^2))
int LPS(const string& s)
{
    int n = (int)s.size();
    vector<vector<int> > dp(n,vector<int>(n,0));
    rep(i,n){
        dp[i][i] = 1;
    }
    for(int i = 2; i <= n; i++){
        rep(j,n-i+1){
            int k = i + j - 1;
            if(s[j] == s[k] && i == 2){
                dp[j][k] = 2;
            }else if (s[j] == s[k]){
                dp[j][k] = dp[j+1][k-1] + 2;
            }else{
                dp[j][k] = max(dp[j][k-1], dp[j+1][k]);
            }
        }
    }
    return dp[0][n-1];
}
