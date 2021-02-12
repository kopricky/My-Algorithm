#include "../header.hpp"

//二部グラフの頂点数がそれぞれn,m
//BM bm(n,m);
//適宜add_edge(２つの集合間のedgeのみaddする)
//BM.solve() で最大マッチングの数を計算
class BM {
private:
    int U, V;
    vector<vector<int> > G;
    vector<int> level, que, prv, rasg;
    pair<int, int> bfs(){
        int last = -1, dist = numeric_limits<int>::max();
        fill(level.begin(), level.end(), -1);
        int qh = 0, qt = 0;
        for(int i = 0; i < U; ++i){
            if(asg[i] < 0) level[i] = 0, que[qt++] = i, prv[i] = -1;
        }
        while(qh < qt){
            const int u = que[qh++];
            if(u >= U){
                const int v = rasg[u - U];
                if(v >= 0) level[v] = level[u] + 1, que[qt++] = v, prv[v] = u;
                else if(dist > level[u]) dist = level[u], last = u;
            }else{
                for(const int v : G[u]){
                    if(level[v] < 0) level[v] = level[u] + 1, que[qt++] = v, prv[v] = u;
                }
            }
        }
        return {last, dist};
    }
    bool dfs(const int u, const int dist){
        const int tmp = level[u];
        level[u] = -1;
        if(tmp > dist) return false;
        if(u >= U){
            if(rasg[u - U] < 0) return true;
            else return dfs(rasg[u - U], dist);
        }else{
            for(const int v : G[u]){
                if(tmp < level[v]){
                    if(dfs(v, dist)){
                        asg[u] = v - U, rasg[v - U] = u;
                        return true;
                    }
                }
            }
        }
        return false;
    }
    void hint_search(int cur, int& flow){
        ++flow;
        while(cur >= 0){
            level[cur] = -1;
            if(cur >= U) asg[prv[cur]] = cur - U, rasg[cur - U] = prv[cur];
            cur = prv[cur];
        }
    }
public:
    BM(const int u, const int v)
         : U(u), V(v), G(U + V), level(U + V), que(U + V), prv(U + V), rasg(V, -1), asg(U, -1){}
    void add_edge(const int from, const int to){
        G[from].push_back(U + to);
    }
    // asg に左側頂点がどの右側頂点とマッチングされるかが格納される
    vector<int> asg;
    int solve(){
        int flow = 0;
        for(;;){
            const pair<int, int> p = bfs();
            if(p.first < 0) break;
            hint_search(p.first, flow);
            for(int i = 0; i < U; ++i){
                if(asg[i] < 0) flow += dfs(i, p.second);
            }
        }
        return flow;
    }
    // solve() を呼び出した後に呼び出す(左側頂点 i の添字は U(右側頂点数) + i とする)
    vector<int> minimum_vertex_cover(){
        vector<int> mvc;
        for(int i = 0; i < U; ++i){
            if(level[i] < 0) mvc.push_back(i);
        }
        for(int i = U; i < U + V; ++i){
            if(level[i] >= 0) mvc.push_back(i);
        }
        return mvc;
    }
};
