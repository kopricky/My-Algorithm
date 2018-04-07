//最長部分回文列を求める(O(n^2))
string LPS(const string& s)
{
    int n = (int)s.size();
    vector<vector<int> > dp(n+1,vector<int>(n+1,0));
    rep(i,n+1){
        dp[i][0] = dp[0][i] = i;
    }
    rep(i,n){
        rep(j,n-1-i){
            dp[i+1][j+1] = (s[i]==s[n-1-j])?dp[i][j]:min(dp[i][j+1]+1,dp[i+1][j]+1);
        }
    }
    int mn = n, x = 0, y = n;
    rep(i,n+1){
        if(mn > dp[i][n-i]){
            mn = dp[i][n-i];
            x = i, y = n-i;
        }
    }
    string mid;
    rep(i,n){
        if(mn > dp[i][n-1-i]){
            mn = dp[i][n-1-i];
            x = i, y = n-i-1;
            mid = s[i];
        }
    }
    string res;
    while(x > 0 && y > 0){
        int m = min({dp[x-1][y-1],dp[x-1][y],dp[x][y]});
        if(m == dp[x-1][y-1]){
            res.push_back(s[x-1]);
            x--, y--;
        }else if(m == dp[x-1][y]){
            x--;
        }else{
            y--;
        }
    }
    string rres = res;
    reverse(rres.begin(),rres.end());
    return rres + mid + res;
}
