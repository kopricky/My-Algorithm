#include "../header.hpp"

#define INF 1000000005

const int MAX_N = 2005;

int G[MAX_N][MAX_N], nx[MAX_N], prv[MAX_N], d[MAX_N];

inline void Dijkstra(int node_size, int s){
    int V = node_size, mn, mn_id = s, v, j;
    iota(nx, nx + V, 1), iota(prv + 1, prv + V + 1, 0);
    nx[V] = 0, prv[0] = V;
    fill(d, d + V, INF);
    d[s] = 0;
    for(int i = 0; i < V - 1; i++){
        v = mn_id, mn = INF;
        nx[prv[mn_id]] = nx[mn_id], prv[nx[mn_id]] = prv[mn_id];
        for(j = nx[V]; j != V; j = nx[j]){
            d[j] = min(d[j], d[v] + G[v][j]);
            if(d[j] < mn) mn = d[j], mn_id = j;
        }
        if(mn == INF) break;
    }
}
