#include "../header.hpp"

//非順序木の同型性判定(ハッシュ値を比較)
class tree_hashing
{
private:
    static vector<int> rnum[2];
    static const int mod[2];
    int V;
    vector<vector<int> > G;
public:
    tree_hashing(int node_size) : V(node_size), G(V){
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
    pair<int, int> hash_dfs(int u,int p,int d)
    {
        int res[] = {1,1};
        for(auto v : G[u]){
            if(v != p){
                auto val = hash_dfs(v,u,d+1);
                res[0] = (long long)(res[0] * val.first) % mod[0];
                res[1] = (long long)(res[1] * val.second) % mod[1];
            }
        }
        for(int i = 0; i < 2; i++){
            res[i] = (res[i] + rnum[i][d]) % mod[i];
        }
        return make_pair(res[0],res[1]);
    }
    pair<int, int> hash(int root=0)
    {
        return hash_dfs(root,-1,0);
    }
};
vector<int> tree_hashing::rnum[2];
const int tree_hashing::mod[] = {1000000007, 1000000009};
