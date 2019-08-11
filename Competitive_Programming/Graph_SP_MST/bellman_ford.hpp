#include "../header.hpp"

//頂点数がn,枝数がm
//bellman_ford bf(n,m);
//適宜add_edge
//bf.comp(出発点)でdに最短路長が入る
//負の有向閉路には3種類存在することに注意する

template<typename T> class bellman_ford {
public:
    struct edge{
        int from, to;
        T cost;
    };
    int V;
    T inf;
    vector<T> d;
    vector<edge> es;
    bellman_ford(int node_size) : V(node_size), inf(numeric_limits<T>::max()/2), d(V, inf){}
    void add_edge(int from, int to, T cost){
        es.push_back((edge){from, to, cost});
    }
    //sからの最短路長およびsからたどり着ける負の閉路の検出(trueなら負の閉路が存在する)
    bool solve(int s){
        int cnt = 0;
        d[s] = 0;
        while(cnt < V){
            bool update = false;
            for(auto& e : es){
                if(d[e.from] != inf && d[e.to] > d[e.from] + e.cost){
                    d[e.to] = d[e.from] + e.cost;
                    update = true;
                }
            }
            if(!update) break;
            cnt++;
        }
        return (cnt == V);
    }
    //すべての負の閉路の検出(trueなら負の閉路が存在する)
    bool find_negative_loop(){
        fill(d.begin(), d.end(), (T)0);
        int cnt = 0;
        while(cnt < V){
            bool update = false;
            for(auto& e : es){
                if(d[e.to] > d[e.from] + e.cost){
                    d[e.to] = d[e.from] + e.cost;
                    update = true;
                }
            }
            if(!update) break;
            cnt++;
        }
        return (cnt == V);
    }
    // s, t 間の最短距離が非有界
    bool shortest_path_infinite(int s, int t){
        d[s] = 0;
        for(int i = 0; i < 2*V-1; i++){
            for(auto& e : es){
                if(d[e.from] != inf && d[e.to] > d[e.from] + e.cost){
                    d[e.to] = d[e.from] + e.cost;
                    if(i >= V-1){
                        if(e.to == t) return true;
                        d[e.to] = -inf;
                    }
                }
            }
        }
        return false;
    }
};
