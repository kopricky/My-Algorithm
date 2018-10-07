#include "../header.hpp"
//binary treeは1スタート
//binary tree上のキョリ
int dist(long long x, long long y)
{long
    int c = __builtin_clzll(x) - __builtin_clzll(y);
    return c + (64 - __builtin_clzll(x ^ (y >> c))) * 2;
}
//binary tree上のLCA
long long LCA(long long x,long long y)
{
    int c = __builtin_clzll(x) - __builtin_clzll(y);
    return (x >> (64 - __builtin_clzll(x ^ (y >> c))));
}
