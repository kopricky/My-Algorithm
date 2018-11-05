#include "../header.hpp"

//RadixHeapを用いた Dijkstra の高速化
//コストがllのときは適宜変更する

#define impl(x, last) x == last ? 0 : 31 - __builtin_clz(x ^ last)

class RadixHeap{
public:
    int last, size_;
    vector<pair<int, int> > bucket_[32];
    
    RadixHeap() : last(0), size_(0){}
    
    inline pair<int, int> top(){
        return pop(false);
    }
    inline bool empty(){
        return !size_;
    }
    inline void push(int x, int val){
        size_++, bucket_[impl(x, last)].emplace_back(x, val);
    }
    inline pair<int, int> pop(bool flag = true){
        if(bucket_[0].empty()){
            int id = 1;
            while(bucket_[id].empty()) id++;
            last = min_element(bucket_[id].begin(), bucket_[id].end())->first;
            for(auto val : bucket_[id]){
                bucket_[impl(val.first, last)].push_back(val);
            }
            bucket_[id].clear();
        }
        pair<int, int> res = bucket_[0].back();
        if(flag) size_--, bucket_[0].pop_back();
        return res;
    }
};

template<typename T> class Dijkstra {
public:
	struct edge{
		int to; T cost;
	};
	int V;
	vector<vector<edge> > G;
	vector<T> d;
	using pti = pair<T,int>;
	Dijkstra(int node_size) : V(node_size), G(V), d(V, numeric_limits<T>::max()){}
	//無向グラフの場合
	void add_edge(int u,int v,T cost){
		G[u].pb((edge){v,cost});
	}
	void solve(int s){
		RadixHeap que;
		d[s] = 0;
		que.push(0,s);
		while(!que.empty()){
			pti p = que.top();
			que.pop();
			int v = p.second;
			if(d[v] < p.first) continue;
			for(auto& w : G[v]){
				if(d[w.to] > d[v] + w.cost){
					d[w.to] = d[v] + w.cost;
					que.push(d[w.to],w.to);
				}
			}
		}
	}
};
