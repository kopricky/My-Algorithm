#include "../header.hpp"

//RadixHeapを用いた Dijkstra の高速化
//コストがllのときは適宜変更する

#define impl(x, last) x == last ? 0 : 31 - __builtin_clz(x ^ last)
#define INF 1000000005

const int MAX_N = 100005;

class RadixHeap{
private:
    int last, size_;
    vector<P> bucket_[32];
public:
    RadixHeap() : last(0), size_(0){}
    inline void push(int x, int val){
        size_++, bucket_[impl(x, last)].emplace_back(x, val);
    }
    inline P pop(bool flag = true){
        if(bucket_[0].empty()){
            int id = 1;
            while(bucket_[id].empty()) id++;
            last = min_element(bucket_[id].begin(), bucket_[id].end())->first;
            for(auto val : bucket_[id]){
                bucket_[impl(val.first, last)].push_back(val);
            }
            bucket_[id].clear();
        }
        P res = bucket_[0].back();
        if(flag) size_--, bucket_[0].pop_back();
        return res;
    }
    inline P top(){
        return pop(false);
    }
    inline bool empty(){
        return !size_;
    }
};

struct edge{
    int to, cost;
};

vector<edge> G[MAX_N];
int d[MAX_N];
void solve(int node_size, int s){
    fill(d, d + node_size, INF);
    RadixHeap que;
    d[s] = 0;
    que.push(0, s);
    while(!que.empty()){
        P p = que.top();
        que.pop();
        int v = p.second;
        if(d[v] < p.first) continue;
        for(auto& e : G[v]){
            if(d[e.to] > d[v] + e.cost){
                d[e.to] = d[v] + e.cost;
                que.push(d[e.to], e.to);
            }
        }
    }
}
