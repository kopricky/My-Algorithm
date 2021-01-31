#include "../header.hpp"

// 入力のグラフは連結であることを仮定
// 平面グラフは 2 次元座標に埋め込んだ形で与える
// 点が重なったり, 辺が交差したり, 辺が重なったりしないものとする.
class DualGraph {
private:
    struct info {
        int x, y, z;
        info(const int _x, const int _y, const int _z) : x(_x), y(_y), z(_z){}
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
    struct eda {
        int to, rev, face;
        eda(const int _to, const int _rev) : to(_to), rev(_rev), face(-1){}
    };
    int V;
    vector<vector<info> > pos;
    vector<vector<eda> > G;
    vector<pair<int, int> > pts;
    void sort_edges(){
        for(int i = 0; i < V; ++i) sort(pos[i].begin(), pos[i].end());
        for(int i = 0; i < V; ++i){
            for(int j = 0; j < (int)pos[i].size(); ++j){
                const info& e = pos[i][j];
                const info arg(pts[i].first - pts[e.z].first, pts[i].second - pts[e.z].second, -1);
                const int rev = (int)(lower_bound(pos[e.z].begin(), pos[e.z].end(), arg) - pos[e.z].begin());
                G[i].emplace_back(e.z, rev);
            }
        }
    }
    void search_outside_face(){
        int mxy = numeric_limits<int>::min(), mnx = numeric_limits<int>::max(), id = -1;
        for(int i = 0; i < V; ++i){
            if(mxy < pts[i].second){
                mxy = pts[i].second, mnx = pts[i].first, id = i;
            }else if(mxy == pts[i].second && pts[i].first < mnx){
                mnx = pts[i].first, id = i;
            }
        }
        search(G[id][0].to, G[id][0].rev, 0, {id, 0});
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
            for(const eda& e : G[i]){
                DG[e.face].emplace_back(G[e.to][e.rev].face, make_pair(i, e.to));
            }
        }
    }
public:
    // vector<vector<edge> > DG: 双対グラフ
    // faces[u]: 双対グラフの頂点 u に対応する元のグラフの面(点集合は右回りにならぶ)
    // faces[0] は外側の面に対応する(見た目上は左回りに並んでいるように見える).
    struct edge {
        int to;
        // 元のグラフ上での対応する辺 e = (u, v) を格納
        // 正確に u → v のベクトルの右側が from の面, 左側が to の面に対応する
        pair<int, int> e;
        edge(const int _to, const pair<int, int>& _e) : to(_to), e(_e){}
    };
    vector<vector<edge> > DG;
    vector<vector<int> > faces;
    DualGraph(const vector<pair<int, int> >& points)
        : V((int)points.size()), pos(V), G(V), pts(points){}
    void add_edge(const int u, const int v){
        pos[u].emplace_back(pts[v].first - pts[u].first, pts[v].second - pts[u].second, v);
        pos[v].emplace_back(pts[u].first - pts[v].first, pts[u].second - pts[v].second, u);
    }
    // 双対グラフの頂点数を返す.
    int build(){
        sort_edges();
        faces.push_back(vector<int>());
        search_outside_face();
        int cnt = 1;
        for(int i = 0; i < V; ++i){
            for(int j = 0; j < (int)G[i].size(); ++j){
                const eda& e = G[i][j];
                if(e.face < 0){
                    faces.push_back(vector<int>());
                    search(e.to, e.rev, cnt++, {i, j});
                }
            }
        }
        construct_dgraph(cnt);
        return cnt;
    }
};