#include "../header.hpp"
//binary treeは1スタート
//binary tree上のキョリ
int dist(ll x, ll y)
{
    int c = __builtin_clzll(x) - __builtin_clzll(y);
    return c + (64 - __builtin_clzll(x ^ (y >> c))) * 2;
}
//binary tree上のLCA
ll LCA(ll x,ll y)
{
    int c = __builtin_clzll(x) - __builtin_clzll(y);
    return (x >> (64 - __builtin_clzll(x ^ (y >> c))));
}
