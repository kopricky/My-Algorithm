const int MAX_N = 100000;
const int MAX_M = 100000;

struct edge
{
	int from,to,cost;
};

int d[MAX_N];
edge es[MAX_M];
bool flag;	//負の有向閉路の判定

//sからたどり着ける負の閉路検出可能
void shortest_path(int n,int m,int s)
{
	int cnt = 0;
	fill(d,d+n,INF);
	d[s] = 0;
	while(cnt < n){
		bool update = false;
		rep(i,m){
			edge e = es[i];
			if(d[e.from] != INF && d[e.to] > d[e.from] + e.cost){
				d[e.to] = d[e.from] + e.cost;
				update = true;
			}
		}
		if(!update) break;
		cnt++;
	}
	if(cnt == n){
		flag = true;
	}
}

//すべての負の閉路の検出を行なえる
bool find_negative_loop(int n,int m)
{
    memset(d,0,sizeof(d));
    rep(i,n){
        rep(j,m){
            edge e = es[j];
            if(d[e.to] > d[e.from] + e.cost){
                d[e.to] = d[e.from] + e.cost;
                if(i == n-1){
                    return true;
                }
            }
        }
    }
    return false;
}
