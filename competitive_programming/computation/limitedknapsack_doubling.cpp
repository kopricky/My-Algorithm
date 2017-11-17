int dp[MAX_N];

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n,W;
    cin >> n >> W;
    vector<int> w(n),v(n),m(n);
    rep(i,n){
        cin >> w[i] >> v[i] >> m[i];
    }
    rep(i,n){
        int num = m[i];
        for(int k=1;num>0;k<<=1){
            int mul = min(k,num);
            for(int j=W;j>=w[i]*mul;j--){
                dp[j] = max(dp[j],dp[j-w[i]*mul]+v[i]*mul);
            }
            num -= mul;
        }
    }
    cout << dp[W] << "\n";
    return 0;
}
