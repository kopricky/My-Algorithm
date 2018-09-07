#include "../header.hpp"

#define MAX_N 100

// 全点対間の最短距離を求める(O(n^3))

int d[MAX_N][MAX_N];

void warshall_floyd(int V)
{
    for(int i = 0; i < V; i++){
        d[i][i] = 0;
    }
    for(int i = 0; i < V; i++){
        for(int j = 0; j < V; j++){
            for(int k = 0; k < V; k++){
                d[j][k] = min(d[j][k],d[j][i]+d[i][k]);
            }
        }
    }
}
