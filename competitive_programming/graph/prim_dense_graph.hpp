#include "../header.hpp"

#define INF 1000000005

const int MAX_N = 2005;

int G[MAX_N][MAX_N], nx[MAX_N], prv[MAX_N], d[MAX_N];

inline int Prim(int node_size){
    int V = node_size, res = 0, mn = INF, mn_id = 0, v = 0, j;
    iota(nx, nx + V, 1), iota(prv, prv + V, -1);
    fill(d, d + V, INF);
    for(int i = 0; i < V-1; i++){
        for(j = nx[0]; j != V; j = nx[j]){
            d[j] = min(d[j], G[v][j]);
            if(d[j] < mn) mn = d[j], mn_id = j;
        }
        res += mn, v = mn_id;
        d[mn_id] = mn = INF;
        prv[nx[mn_id]] = prv[mn_id], nx[prv[mn_id]] = nx[mn_id];
    }
    return res;
}
