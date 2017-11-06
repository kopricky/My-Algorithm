//行きがけ、通りがけ、帰りがけのすべてを記録
vector<int> G[MAX_N];
vector<int> ord;
int lb[MAX_N],rb[MAX_N],id[MAX_N];

void dfs(int u,int p)
{
    id[u] = ord.size();
    lb[u] = (int)ord.size();
    ord.pb(u);
    rep(i,G[u].size()){
        if(G[u][i] != p){
            dfs(G[u][i],u);
            ord.pb(u);
        }
    }
    rb[u] = (int)ord.size() - 1;
}

//行きがけ、帰りがけのみを記録
vector<int> G[MAX_N];
vector<int> ord;
int lb[MAX_N],rb[MAX_N],id[MAX_N];

void dfs(int u,int p)
{
    id[u] = ord.size();
    lb[u] = (int)ord.size();
    ord.pb(u);
    rep(i,G[u].size()){
        if(G[u][i] != p){
            dfs(G[u][i],u);
        }
    }
    ord.pb(u);
    rb[u] = (int)ord.size() - 1;
}

//行きがけのみを記録
vector<int> G[MAX_N];
vector<int> ord;
int lb[MAX_N],rb[MAX_N],id[MAX_N];
//rb[u]はuを根とする部分木の中のordの最大id

void dfs(int u,int p)
{
    id[u] = ord.size();
    lb[u] = (int)ord.size();
    ord.pb(u);
    rep(i,G[u].size()){
        if(G[u][i] != p){
            dfs(G[u][i],u);
        }
    }
    rb[u] = (int)ord.size() - 1;
}
