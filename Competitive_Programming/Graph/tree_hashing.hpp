#include "../header.hpp"

//木のハッシュ値を全方位的に計算
class tree_hashing {
public:
    static vector<vector<unsigned int> > rnum;
    static constexpr unsigned int mod = 1000000007;
    const int V, RC;
    vector<vector<int> > G;
    vector<vector<vector<unsigned int> > > memo;
    vector<vector<pair<int, int> > > table;
    
    static unsigned int add(const unsigned int x, const unsigned int y){
        return (x + y >= mod) ? (x + y - mod) : (x + y);
    }
    static unsigned int mul(const unsigned int x, const unsigned int y){
        return (unsigned long long)x * y % mod;
    }
    pair<int, int> dfs(const int u, const int p, vector<vector<vector<unsigned int> > >& lmul){
        const int c = (int)G[u].size();
        vector<unsigned int> mult(RC, 1u);
        int id = 0;
        for(int i = 0; i < c; ++i){
            const int v = G[u][i];
            if(v != p){
                const pair<int, int> val = dfs(v, u, lmul);
                if(table[u][0].first < val.first + 1){
                    swap(table[u][0], table[u][1]);
                    table[u][0] = {val.first + 1, i};
                }else if(table[u][1].first < val.first + 1){
                    table[u][1] = {val.first + 1, i};
                }
                for(int j = 0; j < RC; ++j){
                    mult[j] = mul(mult[j], lmul[u][i + 1][j] = memo[v][val.second][j]);
                }
            }else{
                id = i;
            }
        }
        for(int i = 0; i < RC; ++i) memo[u][id][i] = add(rnum[table[u][0].first][i], mult[i]);
        return {table[u][0].first, id};
    }
    void redfs(const int u, const int p,
                vector<vector<vector<unsigned int> > >& lmul, const int d, const int id){
        const int c = (int)G[u].size();
        vector<vector<unsigned int> > rmul(c + 1, vector<unsigned int>(RC, 1u));
        for(int i = 0; i < RC; ++i) lmul[u][0][i] = rmul[c][i] = 1u;
        for(int i = c - 1; i >= 0; --i){
            if(G[u][i] != p){
                for(int j = 0; j < RC; ++j){
                    rmul[i][j] = mul(rmul[i + 1][j], lmul[u][i + 1][j]);
                }
            }else{
                if(table[u][0].first < d + 1){
                    swap(table[u][0], table[u][1]);
                    table[u][0] = {d + 1, i};
                }else if(table[u][1].first < d + 1){
                    table[u][1] = {d + 1, i};
                }
                for(int j = 0; j < RC; ++j){
                    rmul[i][j] = mul(rmul[i + 1][j], lmul[u][i + 1][j] = memo[p][id][j]);
                }
            }
        }
        for(int i = 0; i < c; ++i){
            const int v = G[u][i];
            if(v != p){
                const int newd = (table[u][0].second == i) ? table[u][1].first : table[u][0].first;
                for(int j = 0; j < RC; ++j){
                    memo[u][i][j] = add(rnum[newd][j], mul(lmul[u][i][j], rmul[i + 1][j]));
                }
                redfs(v, u, lmul, newd, i);
            }
            for(int j = 0; j < RC; ++j){
                lmul[u][i + 1][j] = mul(lmul[u][i + 1][j], lmul[u][i][j]);
            }
        }
        for(int i = 0; i < RC; ++i){
            memo[u][c][i] = add(rnum[table[u][0].first][i], lmul[u][c][i]);
        }
    }
    
public:
    tree_hashing(const int node_size, const int random_count=4)
        : V(node_size), RC(random_count), G(V), memo(V), table(V){
        mt19937 mt(random_device{}());
        uniform_int_distribution<> random(1, mod - 1);
        while((int)rnum.size() < V){
            vector<unsigned int> vec(RC);
            for(int i = 0; i < RC; ++i) vec[i] = random(mt);
            rnum.push_back(vec);
        }
        for(int i = 0; i < V; ++i){
            table[i] = {pair<int, int>(0, -1), pair<int, int>(0, -1)};
        }
    }
    void add_edge(const int a, const int b){
        G[a].push_back(b), G[b].push_back(a);
    }
    void build(){
        vector<vector<vector<unsigned int> > > lmul(V);
        for(int i = 0; i < V; ++i){
            sort(G[i].begin(), G[i].end());
            lmul[i] = vector<vector<unsigned int> >(G[i].size() + 1u, vector<unsigned int>(RC));
            memo[i] = vector<vector<unsigned int> >(G[i].size() + 1u, vector<unsigned int>(RC));
        }
        dfs(0, -1, lmul), redfs(0, -1, lmul, 0, 0u);
    }
    vector<unsigned int> hash(const int u, const int p){
        if(p == -1) return memo[u][G[u].size()];
        const int id = (int)(lower_bound(G[u].begin(), G[u].end(), p) - G[u].begin());
        return memo[u][id];
    }
};
vector<vector<unsigned int> > tree_hashing::rnum;