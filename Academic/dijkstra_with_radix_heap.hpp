#include "./header.hpp"

class List {
public:
    struct node {
        int prev, next;
    };
    const int N, H;
    vector<node> dat;
    List(const int _N, const int _H) : N(_N), H(_H), dat(N+H){ clear(); }
    inline bool empty(const int h) const { return (dat[N+h].prev == N+h); }
    inline int back(const int h) const { return dat[N+h].prev; }
    inline void insert(const int h, const int u){
        dat[u].prev = dat[N+h].prev, dat[u].next = N+h;
        dat[dat[N+h].prev].next = u, dat[N+h].prev = u;
    }
    inline void erase(const int u){
        dat[dat[u].prev].next = dat[u].next, dat[dat[u].next].prev = dat[u].prev;
    }
    inline void clear(const int h){ dat[N+h].prev = dat[N+h].next = N+h; }
    inline void clear(){
        for(int i = N; i < N+H; ++i) dat[i].prev = dat[i].next = i;
    }
};

class RadixHeap{
public:
    const int V;
    int size, last;
    List bucket;
    #define impl(x) (x == last) ? 0 : 32 - __builtin_clz(x ^ last)

    RadixHeap(const int node_size) : V(node_size), size(0), last(0), bucket(V, 32){}
    inline bool empty(){ return (size == 0); }
    inline void decrease_key(int newd, int ver){
        bucket.erase(ver), bucket.insert(impl(newd), ver);
    }
    inline void push(int dist, int ver){
        ++size, bucket.insert(impl(dist), ver);
    }
    inline pair<int, int> pop(const vector<int>& dist){
        pair<int, int> res = {numeric_limits<int>::max(), -1};
        if(bucket.empty(0)){
            int id = 0;
            while(bucket.empty(++id));
            for(int i = bucket.dat[V+id].next; i < V; i = bucket.dat[i].next){
                if(dist[i] < res.first) res = {dist[i], i};
            }
            last = res.first, bucket.erase(res.second);
            for(int i = bucket.dat[V+id].next, j = bucket.dat[i].next; i < V;){
                bucket.insert(impl(dist[i]), i), j = bucket.dat[i = j].next;
            }
            bucket.clear(id);
        }else{
            res = {last, bucket.back(0)}, bucket.erase(res.second);
        }
        return --size, res;
    }
};

class Dijkstra {
public:
    struct edge{
        int to, cost;
    };
    const int V, inf;
    vector<vector<edge> > G;
    vector<int> d;
    Dijkstra(const int node_size)
        : V(node_size), inf(numeric_limits<int>::max()), G(V), d(V, inf){}
    // 無向グラフの場合
    void add_edge(const int u, const int v, const int cost){
        G[u].push_back((edge){v, cost}), G[v].push_back((edge){u, cost});
    }
    void solve(const int s){
        RadixHeap que(V);
        d[s] = 0;
        que.push(0, s);
        while(!que.empty()){
            pair<int, int> p = que.pop(d);
            const int v = p.second;
            for(auto& e : G[v]){
                if(d[e.to] == inf){
                    d[e.to] = d[v] + e.cost;
                    que.push(d[e.to], e.to);
                }else if(d[e.to] > d[v] + e.cost){
                    d[e.to] = d[v] + e.cost;
                    que.decrease_key(d[e.to], e.to);
                }
            }
        }
    }
};
