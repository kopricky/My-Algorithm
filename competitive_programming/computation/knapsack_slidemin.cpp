const int MAX_N = 101;  //個数
const int MAX_W = 10005;    //重さ

int n,W;
vector<int> v,w,m;
int dp[MAX_W];
int deq[MAX_W];
int deqv[MAX_W];

void solve()
{
    rep(i,n){
		rep(a,w[i]){
			int s=0,t=0;
			for(int j=0;j*w[i]+a<=W;j++){
				int val = dp[j*w[i]+a]-j*v[i];
				while(s<t&&deqv[t-1]<=val){
					t--;
				}
				deq[t] = j;
				deqv[t++] = val;
				dp[j*w[i]+a] = deqv[s] + j*v[i];
				if(deq[s] == j-m[i]){
					s++;
				}
			}
		}
	}
}

int main()
{
	cin >> n >> W;
    v.resize(n),w.resize(n),m.resize(n);
	rep(i,n){
		cin >> v[i] >> w[i] >> m[i];
	}
	cout << dp[W] << "\n";
	return 0;
}
