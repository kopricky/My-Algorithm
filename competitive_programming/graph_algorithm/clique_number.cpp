//最大独立集合のサイズは補グラフのクリーク数を考える
//最小頂点被覆は(頂点数)-(最大独立集合のサイズ)
//kazumaさんの実装を参照(自分の実装より速度がだいぶ速い)
class CliqueNumber{
private:
	static const int MAX_V = 210;
	int V, ans;
	int el[MAX_V][MAX_V / 30 + 1];
	int dp[MAX_V];
	int s[MAX_V][MAX_V / 30 + 1];
	vector<int> sol;
    bool dfs(int v, int k) {
		int c = 0, d = 0;
		for(int i = 0; i < (V + 31) / 32; i++){
			s[k][i] = el[v][i];
			if (k != 1) s[k][i] &= s[k - 1][i];
			c += __builtin_popcount(s[k][i]);
		}
		if(c == 0){
			if(k > ans){
				ans = k;
				sol.clear();
				sol.push_back(v);
				return 1;
			}
			return 0;
		}
		for(int i = 0; i<(V + 31) / 32; i++){
			for(int a = s[k][i]; a; d++){
				if (k + (c - d) <= ans) return 0;
				int lb = a&(-a), lg = 0;
				a ^= lb;
				while (lb != 1) {
					lb = (unsigned int)(lb) >> 1;
					lg++;
				}
				int u = i * 32 + lg;
				if (k + dp[u] <= ans) return 0;
				if (dfs(u, k + 1)) {
					sol.push_back(v);
					return 1;
				}
			}
		}
		return 0;
	}
public:
	CliqueNumber(int node_size) {
		V = node_size; ans = 0;
		memset(el, 0, sizeof(int) * MAX_V * (MAX_V / 30 + 1));
		memset(dp, 0, sizeof(int) * MAX_V);
	}
    void add_edge(int u, int v) {
        if (u > v) swap(u, v);
        if (u == v) return;
        el[u][v / 32] |= (1 << (v % 32));
    }
	int solve() {
		for(int i = V - 1; i >= 0; i--){
			dfs(i, 1);
			dp[i] = ans;
		}
		return ans;
	}
};
