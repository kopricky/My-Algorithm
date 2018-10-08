#include "header.hpp"

// 無向グラフ G に長さ3の単純閉路が含まれるかどうかを返す(O(だいたいm^1.5))
// 愚直に行うと隣接行列の3乗を計算するのでO(n^3/64)とかになる
// alpha は threshold だがコメントアウトしたものとどちらが良いかはケースによりそう
// Alon, Yuster, Zwick 1997 を参照

template <int MAX_E> class C3_Find {
private:

    // static const int alpha = floor(sqrt(MAX_E/log2((double)MAX_E+1)))+1;
    static const int alpha = floor(sqrt(MAX_E/2.0))+1;
    static const int MAX_N = ceil(2.0*MAX_E / alpha);

    class mat {
    public:
        bitset<MAX_N>* a;
        mat(){ a = new bitset<MAX_N>[MAX_N]; }
        mat operator*(const mat& m) const {
            mat ans;
            for(int i = 0; i < MAX_N; i++){
                for(int j = 0; j < MAX_N; j++){
                    if(this->a[i][j] == 0) continue;
                    ans.a[i] |= m.a[j];
                }
            }
            return ans;
        }
    };

    bool low_degree() {
        for(int u = 0; u < V; u++){
            if(unzip[u] < 0){
                for(int j = 0; j < deg[u]-1; j++){
                    int v = G[u][j];
                    if(unzip[v] < unzip[u]) continue;
                    for(int k = j+1; k < deg[u]; k++){
                        int w = G[u][k];
                        if(unzip[w] < unzip[u]) continue;
                        if(es.find(make_pair(min(v, w), max(v, w))) != es.end()) return true;
                    }
                }
            }
        }
        return false;
    }

    bool high_degree() {
        mat A;
        for(auto& p : es){
            int u = unzip[p.first], v = unzip[p.second];
            if(u >= 0 && v >= 0) A.a[u].set(v), A.a[v].set(u);
        }
        mat B = A * A * A;
        for(int i = 0; i < high_num; i++){
            if(B.a[i].test(i)) return true;
        }
        return false;
    }

    int V, high_num;
    vector<vector<int> > G;
    set<pair<int, int> > es;
    vector<int> deg, unzip;

public:
    C3_Find(int node_size) : V(node_size), high_num(0), G(V), deg(V, 0), unzip(V){
        iota(unzip.begin(), unzip.end(), -V);
    }

    void add_edge(int u, int v){
        G[u].push_back(v), G[v].push_back(u);
        deg[u]++, deg[v]++;
        es.insert(make_pair(min(u, v), max(u, v)));
    }

    bool solve(){
        for(int i = 0; i < V; i++){
            if(deg[i] > alpha){
                unzip[i] = high_num++;
            }
        }
        return low_degree() || high_degree();
    }
};
