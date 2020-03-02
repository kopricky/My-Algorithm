#include "../header.hpp"

//根つき木の同型性判定(ハッシュ値を比較)
class tree_hashing {
private:
    static vector<vector<unsigned int> > rnum;
    static constexpr unsigned int mod = 1000000007;
    const int V, RC;
    vector<vector<int> > G;
    
    static unsigned int add(const unsigned int x, const unsigned int y){
        return (x + y >= mod) ? (x + y - mod) : (x + y);
    }
    static unsigned int mul(const unsigned int x, const unsigned int y){
        return (unsigned long long)x * y % mod;
    }
    int hash_dfs(const int u, const int p, vector<unsigned int>& res){
        int depth = 0;
        for(auto v : G[u]){
            if(v != p){
                vector<unsigned int> vec(RC, 1u);
                const int d = hash_dfs(v, u, vec);
                depth = max(depth, d + 1);
                for(int i = 0; i < RC; ++i) res[i] = mul(res[i], vec[i]);
            }
        }
        for(int i = 0; i < RC; ++i) res[i] = add(res[i], rnum[depth][i]);
        return depth;
    }
public:
    tree_hashing(const int node_size, const int random_count=2)
        : V(node_size), RC(random_count), G(V){
        mt19937 mt(random_device{}());
        uniform_int_distribution<> random(1, mod - 1);
        while((int)rnum.size() < V){
            vector<unsigned int> vec(RC);
            for(int i = 0; i < RC; ++i) vec[i] = random(mt);
            rnum.push_back(vec);
        }
    }
    void add_edge(const int a, const int b){
        G[a].push_back(b), G[b].push_back(a);
    }
    vector<unsigned int> hash(const int root=0){
        vector<unsigned int> vec(RC, 1u);
        return hash_dfs(root, -1, vec), vec;
    }
};
vector<vector<unsigned int> > tree_hashing::rnum;