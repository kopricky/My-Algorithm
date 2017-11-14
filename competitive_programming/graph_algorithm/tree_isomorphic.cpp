//これはverifiedされてません
typedef pair<long long,long long> P;

int hash_dp[MAX_N];

class hashing
{
private:
    vector<ll> rnum1,rnum2;
    ll mod1,mod2;
    int node_size;
public:
    hashing(int arg1){
        node_size = arg1;
        mod1=1000000007,mod2=1000000009;
        random_device rnd;
        mt19937 mt(rnd());
        uniform_int_distribution<> rand1(0,mod1-1),rand2(0,mod2-1);
        rnum1.resize(node_size),rnum2.resize(node_size);
        rep(i,node_size){
            rnum1[i] = rand1(mt),rnum2[i] = rand2(mt);
        }
    }
    void hash_dfs(int u,int p,int k,const ll mod,const vector<ll>& num,vector<ll>& hdp,const vector<int>* g)
    {
        if((int)g[u].size() == 1 && g[u][0] >= 0){
            hdp[u] = 1;
            return;
        }
        ll val = 1;
        rep(i,g[u].size()){
            if(g[u][i] != p){
                hash_dfs(g[u][i],u,k+1,mod,num,hdp,g);
                val = (val*hdp[g[u][i]]) % mod;
            }
        }
        hdp[u] = (val + num[k]) % mod;
        return;
    }
    P comp_hash(vector<int>* graph,int node)
    {
        vector<ll> vec1(node),vec2(node);
        hash_dfs(0,-1,0,mod1,rnum1,vec1,graph);
        hash_dfs(0,-1,0,mod2,rnum2,vec2,graph);
        return P(vec1[0],vec2[0]);
    }
};
