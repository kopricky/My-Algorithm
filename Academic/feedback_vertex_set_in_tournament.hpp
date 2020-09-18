#include "header.hpp"

// トーナメントグラフのサイズ k の feedback vertex set を与えて minimum feedback vertex set を返す
// 計算量: O(n^2 + n * 2^k * (k + log n))
class FVS_in_Tournament {
private:
    const int V;
    vector<int> ord;
    bool *used, *unused;
    bool init_tsort(const vector<int>& fvs, const vector<vector<int> >& G){
        for(const int e : fvs) used[e] = true;
        vector<int> deg(V, 0);
        for(int i = 0; i < V; ++i){
            if(used[i]) continue;
            for(int j = 0; j < V; ++j) if(!used[j]) deg[j] += G[i][j];
        }
        queue<int> que;
        for(int i = 0; i < V; ++i){
            if(!used[i] && deg[i] == 0) que.push(i);
        }
        while(!que.empty()){
            const int p = que.front();
            que.pop();
            ord.push_back(p);
            for(int i = 0; i < V; ++i){
                if(!used[i] && G[p][i] && --deg[i] == 0) que.push(i);
            }
        }
        for(const int e : fvs) used[e] = false;
        return (*max_element(deg.begin(), deg.end()) == 0);
    }
    bool refine(const vector<int>& fvs, vector<int>& nfvs, const vector<vector<int> >& G){
        const int K = (int)fvs.size();
        if(K == 0) return false;
        vector<vector<int> > adj(1 << K), vord(1 << K);
        vector<int> fals(1 << K, 0), ver;
        auto clear = [&](){
            for(const int e : nfvs) used[e] = false;
            for(const int e : ver) unused[e] = false;
        };
        for(int i = 1; i < (1 << K); ++i){
            nfvs.clear(), ver.clear();
            bool ng = false;
            int mn = numeric_limits<int>::max(), id = -1;
            for(int j = 0; j < K; ++j){
                if(i >> j & 1){
                    if(fals[i ^ (1 << j)]){
                        ng = true;
                        break;
                    }
                    ver.push_back(fvs[j]), unused[fvs[j]] = true;
                    if((int)adj[i ^ (1 << j)].size() < mn) mn = (int)adj[i ^ (1 << j)].size(), id = j;
                }else nfvs.push_back(fvs[j]), used[fvs[j]] = true;
            }
            if(ng){
                fals[i] = 1, clear();
                continue;
            }
            adj[i] = adj[i ^ (1 << id)];
            for(const int e : adj[i]) nfvs.push_back(e), used[e] = true;
            bool ap = false;
            for(const int e : vord[i ^ (1 << id)]){
                if(ap && G[e][fvs[id]]){
                    ng = true;
                    break;
                }
                if(!ap && G[fvs[id]][e]){
                    ap = true;
                    vord[i].push_back(fvs[id]);
                }
                vord[i].push_back(e);
            }
            if(!ap) vord[i].push_back(fvs[id]);
            if(ng){
                fals[i] = 1, clear();
                continue;
            }
            id = fvs[id];
            const int L = (int)ver.size();
            for(int j = 0; j < V; ++j){
                if(used[j] || unused[j]) continue;
                if(G[id][j]){
                    for(const int e : ver){
                        if(G[j][e] && G[e][id]){
                            adj[i].push_back(j), nfvs.push_back(j), used[j] = true;
                            if((int)adj[i].size() >= K) ng = true;
                            break;
                        }
                    }
                }else{
                    for(const int e : ver){
                        if(G[id][e] && G[e][j]){
                            adj[i].push_back(j), nfvs.push_back(j), used[j] = true;
                            if((int)adj[i].size() >= K) ng = true;
                            break;
                        }
                    }
                }
                if(ng) break;
            }
            if(ng){
                fals[i] = 1, clear();
                continue;
            }
            if((int)nfvs.size() < K){
                vector<vector<int> > arr(L + 1);
                vector<int> A, pos((int)ord.size(), L);
                for(int j = 0; j < (int)ord.size(); ++j){
                    const int u = ord[j];
                    if(used[u] || unused[u]) continue;
                    for(int k = 0; k < L; ++k){
                        if(G[u][vord[i][k]]){
                            pos[j] = k;
                            break;
                        }
                    }
                    arr[pos[j]].push_back(j);
                }
                for(int j = 0; j <= L; ++j) for(const int e : arr[j]) A.push_back(e);
                vector<int> dp((int)A.size(), numeric_limits<int>::max()), prv((int)ord.size(), -1);
                for(const int e : A){
                    const int index = (int)(lower_bound(dp.begin(), dp.end(), e) - dp.begin());
                    if(index > 0) prv[e] = dp[index-1];
                    dp[index] = e;
                }
                const int N = lower_bound(dp.begin(), dp.end(), numeric_limits<int>::max()) - dp.begin();
                if(V - (L + N) < K){
                    vector<int> lis, nord;
                    if(N > 0){
                        for(int cur = dp[N-1]; cur >= 0; cur = prv[cur]){
                            lis.push_back(cur), unused[ord[cur]] = true;
                        }
                        reverse(lis.begin(), lis.end());
                    }
                    int pre = 0;
                    for(const int e : lis){
                        for(int j = pre; j < pos[e]; ++j) nord.push_back(vord[i][j]);
                        nord.push_back(ord[e]), pre = pos[e];
                    }
                    for(int j = pre; j < L; ++j) nord.push_back(vord[i][j]);
                    for(int j = 0; j < V; ++j){
                        if(used[j] || unused[j]) continue;
                        nfvs.push_back(j), used[j] = true;
                    }
                    for(const int e : lis) unused[ord[e]] = false;
                    ord = nord;
                    clear();
                    return true;
                }
            }
            clear();
        }
        return false;
    }
public:
    FVS_in_Tournament(const int node_size)
         : V(node_size), used(new bool[V]()), unused(new bool[V]()){}
    ~FVS_in_Tournament(){ delete[] used; delete[] unused; }
    vector<int> solve(vector<int> fvs, const vector<vector<int> >& G){
        assert(init_tsort(fvs, G)); // fvs is not valid
        vector<int> nfvs;
        while(refine(fvs, nfvs, G)) swap(fvs, nfvs);
        return fvs;
    }
};