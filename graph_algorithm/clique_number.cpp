typedef pair<int,int>P;

const int MAX_N = 20;

bool connected[MAX_N][MAX_N];

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n,m;
    cin >> n >> m;
    rep(i,m){
        int a,b;
        cin >> a >> b;
        connected[a][b] = connected[b][a] = true;
    }
    int ans = 0;
    for(int S = (1 << n) - 1;S >= 1; S--){
        int cnt = __builtin_popcount(S);
        if(cnt <= ans){
            continue;
        }
        bool ok = true;
        rep(i,n){
            rep(j,i){
                if((S & (1 << i)) && (S & (1 << j)) && !connected[i][j]){
                    ok = false;
                }
            }
        }
        if(ok){
            ans = cnt;
        }
    }
    cout << ans << endl;
    return 0;
}
