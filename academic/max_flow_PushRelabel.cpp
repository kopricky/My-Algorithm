//実測は速くない
//O(n^3) Goldberg and Tarjan 1988 を参照(動的木により計算量は改善可能)
template<typename T> class PushRelabel{
private:
    struct edge{
        int to,rev;
        T f,cap;
    };
    vector<vector<edge> > G;
    int V;
public:
    PushRelabel(int node_size){
        V = node_size;
        G.resize(V);
    }
    void add_edge(int from,int to,T capacity){
        G[from].push_back((edge){to,(int)G[to].size(),(T)0,capacity});
        G[to].push_back((edge){from,(int)G[from].size()-1,(T)0,(T)0});
    }
    T solve(int s,int t){
        vector<int> d(V,0),mxd(V,0);
        vector<T> ex(V,0);
        d[s] = V-1;
        for(edge& e : G[s]){
            e.f = e.cap;
            G[e.to][e.rev].f = -e.cap;
            ex[e.to] += e.cap;
        }
        for(int sz = 0;;){
            if(sz == 0){
                rep(i,V){
                    if(i != s && i != t && ex[i] > 0){
                        if(sz != 0 && d[i] > d[mxd[0]]){
                            sz = 0;
                        }
                        mxd[sz++] = i;
                    }
                }
            }
            if(sz == 0) break;
            while(sz){
                int i = mxd[sz-1];
                bool push = false;
                for(int j = 0; j < (int)G[i].size() && ex[i] != 0; j++){
                    edge& e = G[i][j];
                    if(d[i] == d[e.to] + 1 && e.cap - e.f > 0){
                        //push操作
                        push = true;
                        T df = min(e.cap - e.f, ex[i]);
                        e.f += df, G[e.to][e.rev].f -= df;
                        ex[i] -= df, ex[e.to] += df;
                        if(ex[i] == 0){
                            sz--;
                        }
                    }
                }
                if(!push){
                    //relabel操作
                    d[i] = numeric_limits<int>::max();
                    for(edge& e : G[i]){
                        if(d[i] > d[e.to] + 1 && e.cap - e.f > 0){
                            d[i] = d[e.to] + 1;
                        }
                    }
                    if(d[i] > d[mxd[0]]){
                        sz = 0;
                        break;
                    }
                }
            }
        }
        T flow = 0;
        for(edge& e : G[s]){
            flow += e.f;
        }
        return flow;
    }
};
