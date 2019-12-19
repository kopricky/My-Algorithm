#include "../header.hpp"

//根つき木の同型性判定(ハッシュ値を比較)
class tree_hashing {
private:
    static vector<unsigned long long> rnum[2];
    static constexpr unsigned long long mod[2] = {1000000007ULL, 1000000009ULL};
    const int V;
    vector<vector<int> > G;
    vector<int> depth_st;
public:
    tree_hashing(const int node_size) : V(node_size), G(V), depth_st(V, 0){
        random_device rnd;
        mt19937 mt(rnd());
        uniform_int_distribution<> rand1(0, mod[0] - 1), rand2(0, mod[1] - 1);
        while((int)rnum[0].size() < V){
            rnum[0].push_back(rand1(mt)), rnum[1].push_back(rand2(mt));
        }
    }
    void add_edge(const int a, const int b){
        G[a].push_back(b), G[b].push_back(a);
    }
    pair<unsigned long long, unsigned long long> hash_dfs(const int u, const int p)
    {
        unsigned long long res[] = {1ULL, 1ULL};
        for(auto v : G[u]){
            if(v != p){
                auto val = hash_dfs(v, u);
                depth_st[u] = max(depth_st[u], depth_st[v]);
                res[0] = (res[0] * val.first) % mod[0];
                res[1] = (res[1] * val.second) % mod[1];
            }
        }
        res[0] = (res[0] + rnum[0][depth_st[u]]) % mod[0];
        res[1] = (res[1] + rnum[1][depth_st[u]]) % mod[1];
        return make_pair(res[0], res[1]);
    }
    pair<unsigned long long, unsigned long long> hash(int root=0)
    {
        return hash_dfs(root, -1);
    }
};
vector<unsigned long long> tree_hashing::rnum[2];
