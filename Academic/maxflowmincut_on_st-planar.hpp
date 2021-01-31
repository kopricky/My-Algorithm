#include "./header.hpp"

// 入力のグラフは連結であることを仮定
// 平面グラフは 2 次元座標に埋め込んだ形で与える
// 点が重なったり, 辺が交差したり, 辺が重なったりしないものとする.
template<typename T> class MaxFlowMinCut_on_STPlanar {
private:
    struct info {
        int x, y, z;
        T cap;
        info(const int _x, const int _y, const int _z, const T _cap)
			 : x(_x), y(_y), z(_z), cap(_cap){}
        bool operator<(const info& s) const {
            if((long long)y * s.y <= 0){
                if(y == 0 && s.y == 0) return (x >= 0 && s.x < 0);
                if(y == 0 && s.y > 0) return (x >= 0);
                if(y > 0 && s.y == 0)  return (s.x < 0);
                return (y < s.y);
            }else if(((long long)x * s.x <= 0)){
                return (x == s.x) ? false : ((y > 0) ? (x > s.x) : (x < s.x));
            }else{
                return ((long long)y * s.x < (long long)x * s.y);
            }
        }
    };
    struct dual_edge {
        int to;
        T cost;
        pair<int, int> memo;
        dual_edge(const int _to, const T _cost, const pair<int, int>& _memo)
             : to(_to), cost(_cost), memo(_memo){}
    };
    int V;
    vector<vector<info> > pos;
    vector<vector<dual_edge> > DG;
    vector<pair<int, int> > pts;
    void sort_dual_edges(){
        for(int i = 0; i < V; ++i) sort(pos[i].begin(), pos[i].end());
        for(int i = 0; i < V; ++i){
            for(int j = 0; j < (int)pos[i].size(); ++j){
                const info& e = pos[i][j];
                const info arg(pts[i].first - pts[e.z].first, pts[i].second - pts[e.z].second, -1, T());
                const int rev = (int)(lower_bound(pos[e.z].begin(), pos[e.z].end(), arg) - pos[e.z].begin());
                G[i].emplace_back(e.z, rev, e.cap);
            }
        }
    }
    void search_outside_face(const int s, const int t){
        int mxy = numeric_limits<int>::min(), mnx = numeric_limits<int>::max(), id = -1;
        for(int i = 0; i < V; ++i){
            if(mxy < pts[i].second) mxy = pts[i].second, mnx = pts[i].first, id = i;
            else if(mxy == pts[i].second && pts[i].first < mnx) mnx = pts[i].first, id = i;
        }
        pair<int, int> goal(id, 0);
        int cur = G[id][0].to, index = G[id][0].rev;
        while(cur != s){
            	index = (index < (int)G[cur].size() - 1) ? (index + 1) : (0);
        	const auto& e = G[cur][index];
        	goal = {cur, index}, cur = e.to, index = e.rev;
        }
        while(cur != t){
            	index = (index < (int)G[cur].size() - 1) ? (index + 1) : (0);
        	auto& e = G[cur][index];
        	e.face = 0, cur = e.to, index = e.rev;
        }
        search(cur, index, 1, goal);
    }
    void search(int cur, int id, const int f, const pair<int, int>& goal){
        while(true){
        	id = (id < (int)G[cur].size() - 1) ? (id + 1) : (0);
            auto& e = G[cur][id];
            e.face = f;
            if(cur == goal.first && id == goal.second) break;
            cur = e.to, id = e.rev;
        }
    }
    void construct_dgraph(const int node_size){
        DG.resize(node_size);
        for(int i = 0; i < V; ++i){
            for(int j = 0; j < (int)G[i].size(); ++j){
				const flow_edge& e = G[i][j];
                DG[e.face].emplace_back(G[e.to][e.rev].face, e.flow, make_pair(i, j));
            }
        }
    }
	pair<T, vector<pair<int, int> > > solve(vector<T>& dist){
		const int N = (int)DG.size();
		vector<array<int, 3> > prev(N);
		priority_queue<pair<T, int>, vector<pair<T, int> >, greater<pair<T, int> > > que;
		dist[0] = 0, que.emplace(0, 0);
		while(!que.empty()){
			pair<T, int> p = que.top();
			que.pop();
			const int v = p.second;
			if(dist[v] < p.first) continue;
			for(const auto& e : DG[v]){
				if(dist[e.to] > dist[v] + e.cost){
					dist[e.to] = dist[v] + e.cost;
					prev[e.to][0] = v, prev[e.to][1] = (e.memo).first, prev[e.to][2] = (e.memo).second;
					que.emplace(dist[e.to], e.to);
				}
			}
		}
		vector<pair<int, int> > res;
		for(int cur = 1; cur > 0; cur = prev[cur][0]){
			res.emplace_back(prev[cur][1], G[prev[cur][1]][prev[cur][2]].to);
		}
		return make_pair(dist[1], res);
	}
    void construct_flow_graph(vector<T>& dist){
            for(int i = 0; i < (int)dist.size(); ++i){
                for(const auto& e : DG[i]){
                    G[(e.memo).first][(e.memo).second].flow = max(dist[e.to] - dist[i], (T)0);
            }
        }
    }

public:
    struct flow_edge {
		// face:右側の面
        int to, rev, face;
        T flow;
        flow_edge(const int _to, const int _rev, const T _flow)
             : to(_to), rev(_rev), face(-1), flow(_flow){}
    };
    
    vector<vector<flow_edge> > G;

    MaxFlowMinCut_on_STPlanar(const vector<pair<int, int> >& candidate)
        : V((int)candidate.size()), pos(V), pts(candidate), G(V){}
    void add_edge(const int u, const int v, const T cap){
        pos[u].emplace_back(pts[v].first - pts[u].first, pts[v].second - pts[u].second, v, cap);
        pos[v].emplace_back(pts[u].first - pts[v].first, pts[u].second - pts[v].second, u, cap);
    }
    // (最小カットのコスト, 最小カットを構成する辺集合)
    // flow graph G を構築する場合はコメントアウトを外す
    pair<T, vector<pair<int, int> > > minimum_cut(const int s, const int t){
        sort_dual_edges();
        search_outside_face(s, t);
        int cnt = 2;
        for(int i = 0; i < V; ++i){
            for(int j = 0; j < (int)G[i].size(); ++j){
                const flow_edge& e = G[i][j];
                if(e.face < 0) search(e.to, e.rev, cnt++, {i, j});
            }
        }
        construct_dgraph(cnt);
        vector<T> dist(cnt, numeric_limits<T>::max());
        const auto res = solve(dist);
        // construct_flow_graph(dist);
        return res;
    }
};