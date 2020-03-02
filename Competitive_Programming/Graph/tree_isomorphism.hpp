#include "../header.hpp"

//根つき木の同型性判定(ハッシュ値を比較)
class tree_hashing {
private:
    static vector<unsigned int> rnum[2];
    static constexpr unsigned int mod[2] = {1000000007, 1000000009};
    const int V;
    vector<vector<int> > G;
    
    pair<unsigned int, unsigned int> hash_dfs(const int u, const int p, int& depth){
        unsigned int res[] = {1u, 1u};
        for(auto v : G[u]){
            if(v != p){
                int d = 0;
                auto val = hash_dfs(v, u, d);
                depth = max(depth, d);
                res[0] = (unsigned long long)res[0] * val.first % mod[0];
                res[1] = (unsigned long long)res[1] * val.second % mod[1];
            }
        }
        ++depth;
        res[0] = (res[0] + rnum[0][depth] >= mod[0]) ?
        (res[0] + rnum[0][depth] - mod[0]) : (res[0] + rnum[0][depth]);
        res[1] = (res[1] + rnum[1][depth] >= mod[1]) ?
        (res[1] + rnum[1][depth] - mod[1]) : (res[1] + rnum[1][depth]);
        return make_pair(res[0], res[1]);
    }
public:
    tree_hashing(const int node_size) : V(node_size), G(V){
        random_device rnd;
        mt19937 mt(rnd());
        uniform_int_distribution<> rand1(1, mod[0] - 1), rand2(1, mod[1] - 1);
        while((int)rnum[0].size() < V){
            rnum[0].push_back(rand1(mt)), rnum[1].push_back(rand2(mt));
        }
    }
    void add_edge(const int a, const int b){
        G[a].push_back(b), G[b].push_back(a);
    }
    pair<unsigned int, unsigned int> hash(int root=0)
    {
        int d = 0;
        return hash_dfs(root, -1, d);
    }
};
vector<unsigned int> tree_hashing::rnum[2];
