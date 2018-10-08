#include "../header.hpp"

//根つき木の同型性判定(ハッシュ値を比較)
class tree_hashing {
private:
    static vector<long long> rnum[2];
    static constexpr long long mod[2] = {1000000007LL, 1000000009LL};
    int V;
    vector<vector<int> > G;
public:
    tree_hashing(int node_size){
        V = node_size;
        G.resize(V);
        random_device rnd;
        mt19937 mt(rnd());
        uniform_int_distribution<> rand1(0,mod[0]-1),rand2(0,mod[1]-1);
        while((int)rnum[0].size() < V){
            rnum[0].push_back(rand1(mt)), rnum[1].push_back(rand2(mt));
        }
    }
    void add_edge(int a, int b){
        G[a].push_back(b),G[b].push_back(a);
    }
    pair<long long,long long> hash_dfs(int u,int p,int d)
    {
        long long res[] = {1LL,1LL};
        for(auto v : G[u]){
            if(v != p){
                auto val = hash_dfs(v,u,d+1);
                res[0] = (res[0] * (rnum[0][d] + val.first)) % mod[0];
                res[1] = (res[1] * (rnum[1][d] + val.second)) % mod[1];
            }
        }
        return make_pair(res[0],res[1]);
    }
    pair<long long,long long> hash(int root=0)
    {
        return hash_dfs(root,-1,0);
    }
};
vector<long long> tree_hashing::rnum[2];
