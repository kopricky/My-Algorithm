//行きがけ、通りがけ、帰りがけのすべてを記録
vector<int> G[MAX_N];
vector<int> ord;
int lb[MAX_N],rb[MAX_N],id[MAX_N];

void dfs(int u,int p)
{
    id[u] = ord.size();
    lb[u] = (int)ord.size();
    ord.push_back(u);
    rep(i,G[u].size()){
        if(G[u][i] != p){
            dfs(G[u][i],u);
            ord.push_back(u);
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
    ord.push_back(u);
    rep(i,G[u].size()){
        if(G[u][i] != p){
            dfs(G[u][i],u);
        }
    }
    ord.push_back(u);
    rb[u] = (int)ord.size() - 1;
}

//行きがけのみを記録
vector<int> G[MAX_N];
vector<int> ord;
int lb[MAX_N],rb[MAX_N],id[MAX_N];
//uを根とする部分木は[lb[u],rb[u])で表せる

void dfs(int u,int p)
{
    id[u] = ord.size();
    lb[u] = (int)ord.size();
    ord.push_back(u);
    rep(i,G[u].size()){
        if(G[u][i] != p){
            dfs(G[u][i],u);
        }
    }
    rb[u] = (int)ord.size();
}
