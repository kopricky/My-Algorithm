#include "../header.hpp"

//ハンガリアン法

// U <= V を仮定する
// u, v には割当が入る
template<typename T> class Hungarian {
public:
    int U, V;
    vector<vector<T> > cost;
    vector<int> u, v;
    vector<T> fu, fv;
    Hungarian(int unum, int vnum) : U(unum), V(vnum), cost(U, vector<T>(V, 0)),
        u(U, -1), v(V, -1), fu(U, numeric_limits<T>::max()/2), fv(V, 0){}
    void add_edge(int u, int v, T val){
        cost[u][v] = val;
    }
    T solve(){
        int p, q;
        for(int i = 0; i < U; i++){
            for(int j = 0; j < V; j++){
                fu[i] = max(fu[i], cost[i][j]);
            }
        }
        for(int i = 0; i < U; ){
            vector<int> s(U+1, i), t(V, -1);
            for(p = q = 0; p <= q && u[i] < 0; p++){
                for(int k = s[p], j = 0; j < V && u[i] < 0; j++){
                    if(fu[k] + fv[j] == cost[k][j] && t[j] < 0){
                        s[++q] = v[j], t[j] = k;
                        if(s[q] < 0){
                            for(p = j; p >= 0; j = p){
                                v[j] = k = t[j], p = u[k], u[k] = j;
                            }
                        }
                    }
                }
            }
            if(u[i] < 0){
                T d = numeric_limits<T>::max()/2;
                for(int k = 0; k <= q; k++){
                    for(int j = 0; j < V; j++){
                        if(t[j] < 0){
                            d = min(d, fu[s[k]] + fv[j] - cost[s[k]][j]);
                        }
                    }
                }
                for(int j = 0; j < V; j++){
                    fv[j] += (t[j] < 0 ? 0 : d);
                }
                for(int k = 0; k <= q; k++){
                    fu[s[k]] -= d;
                }
            }else{
                i++;
            }
        }
        T ret = 0;
        for(int i = 0; i < U; i++){
            ret += cost[i][u[i]];
        }
        return ret;
    }
};
