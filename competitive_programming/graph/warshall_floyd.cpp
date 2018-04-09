int d[MAX_N][MAX_N];

void warshall_floyd(int V)
{
    rep(i,V){
        d[i][i] = 0;
    }
    rep(i,V){
        rep(j,V){
            rep(k,V){
                d[j][k] = min(d[j][k],d[j][i]+d[i][k]);
            }
        }
    }
}
