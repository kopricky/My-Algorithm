#include "../header.hpp"

// 二部マッチング(辺追加が左側頂点 i と右側頂点集合 l ≦ j ≦ r となる (i, j), つまり範囲に辺を張る)
// (n x n) グラフ, m 本の範囲辺で計算量は O(m √n) (bitset の access や _Find_next を定数と考えた場合)
template<unsigned int VS> class BM {
private:
    struct edge {
        int l, r;
        edge(const int _l, const int _r) : l(_l), r(_r){}
    };
    const int U, V;
    vector<vector<edge> > G;
    vector<int> que, prv, rasg;
    bitset<VS> bs;
    int bfs(){
        int last = -1, qh = 0, qt = 0;
        for(int i = 0; i < U; ++i){
            if(asg[i] < 0) que[qt++] = i, prv[i] = -1;
        }
        bs.set();
        while(qh < qt){
            const int u = que[qh++];
            if(u >= U){
                const int v = rasg[u - U];
                if(v >= 0) que[qt++] = v, prv[v] = u;
                else last = u;
            }else{
                for(const edge& e : G[u]){
                    for(int v = bs._Find_next(e.l - 1); v < e.r; v = bs._Find_next(v)){
                        bs.reset(v), que[qt++] = U + v, prv[U + v] = u;
                    }
                }
            }
        }
        return last;
    }
    bool dfs(const int u){
        if(u >= U){
            bs.reset(u - U);
            if(rasg[u - U] < 0) return true;
            else return dfs(rasg[u - U]);
        }else{
            for(const edge& e : G[u]){
                for(int v = bs._Find_next(e.l - 1); v < e.r; v = bs._Find_next(v)){
                    if(dfs(U + v)){
                        asg[u] = v, rasg[v] = u;
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
            if(cur >= U) bs.reset(cur - U), asg[prv[cur]] = cur - U, rasg[cur - U] = prv[cur];
            cur = prv[cur];
        }
    }
public:
    BM(const int u, const int v)
         : U(u), V(v), G(U + V), que(U + V), prv(U + V), rasg(V, -1), asg(U, -1){}
    void add_edge(const int from, const int l, const int r){
        G[from].emplace_back(l, r);
    }
    // asg に左側頂点がどの右側頂点とマッチングされるかが格納される
    vector<int> asg;
    int solve(){
        int flow = 0;
        for(;;){
            const int cur = bfs();
            if(cur < 0) break;
            bs.set();
            hint_search(cur, flow);
            for(int i = 0; i < U; ++i){
                if(asg[i] < 0) flow += dfs(i);
            }
        }
        return flow;
    }
};
