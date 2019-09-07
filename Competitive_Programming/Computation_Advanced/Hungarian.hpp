#include "../header.hpp"

// グラフが完全二部グラフ(K_n,n) であることを仮定しています.
template<typename T>
class Hungarian
{
private:
    int V, matching_size;
    vector<vector<int> > graph;
    vector<T> potential;
    vector<int> allocate, reverse_alloc;
    T total_cost;
    inline T diff(const vector<vector<T> >& cost, int _i, int _j){
        return cost[_i][_j-V] - potential[_i] - potential[_j];
    }
    void initial_feasible_solution(const vector<vector<T> >& cost){
        for(int i = 0; i < V; i++){
            potential[i] = 0;
            for(int j = V; j < 2*V; j++){
                potential[j] = min(potential[j], cost[i][j-V]);
            }
        }
    }
    void inital_graph_construction(const vector<vector<T> >& cost){
        for(int i = 0; i < V; i++){
            for(int j = V; j < 2*V; j++){
                graph[i][j-V] = (diff(cost, i, j) == 0);
            }
        }
    }
    void dfs(const int cur, const int start, vector<int>& visit){
        visit[cur] = start;
        if(cur >= V){
            if(reverse_alloc[cur-V] >= 0){
                dfs(reverse_alloc[cur-V], start, visit);
            }
        }else{
            for(int i = 0; i < V; i++){
                if(graph[cur][i] && visit[i+V] < 0){
                    dfs(i+V, start, visit);
                }
            }
        }
    }
    void update_potential(const vector<int>& visit, const T delta){
        for(int i = 0; i < V; i++){
            if(visit[i] >= 0){
                potential[i] += delta;
            }
        }
        for(int j = V; j < 2*V; j++){
            if(visit[j] >= 0){
                potential[j] -= delta;
            }
        }
    }
    void update_graph(const vector<vector<T> >& cost, const vector<pair<int, int> >& vec, const vector<int>& visit){
        for(auto p : vec){
            graph[p.first][p.second] = 1;
        }
        for(int i = 0; i < V; i++){
            if(visit[i] >= 0) continue;
            for(int j = 0; j < V; j++){
                graph[i][j] = ((diff(cost, i, j+V) == 0) && (reverse_alloc[j] != i));
            }
        }
    }
    void replace_matching(const vector<int>& path){
        matching_size++;
        for(int j = 0; j < (int)path.size() - 1; j++){
            if(j % 2){
                graph[path[j+1]][path[j]-V] = 1;
            }else{
                graph[path[j]][path[j+1]-V] = 0;
                allocate[path[j]] = path[j+1]-V;
                reverse_alloc[path[j+1]-V] = path[j];
            }
        }
    }
    void init_matching(){
        for(int i = 0; i < V; i++){
            if(allocate[i] < 0){
                vector<int> visit(2*V, -1), path;
                if(find_augmenting_path(i, i, visit, path)){
                    replace_matching(path);
                }
            }
        }
    }
    bool increment_matching(const vector<pair<int, int> >& vec, const vector<int>& visit){
        for(auto p : vec){
            if(reverse_alloc[p.second] < 0){
                vector<int> _visit(2*V, -1), path;
                find_augmenting_path(visit[p.first], visit[p.first], _visit, path);
                replace_matching(path);
                return true;
            }
        }
        return false;
    }
    bool find_augmenting_path(const int cur, const int start, vector<int>& visit, vector<int>& path){
        path.push_back(cur);
        visit[cur] = start;
        if(cur >= V){
            if(reverse_alloc[cur-V] < 0) return true;
            if(find_augmenting_path(reverse_alloc[cur-V], start, visit, path)){
                return true;
            }
        }else{
            for(int i = 0; i < V; i++){
                if(graph[cur][i] && visit[i+V] < 0){
                    if(find_augmenting_path(i+V, start, visit, path)){
                        return true;
                    }
                }
            }
        }
        path.pop_back();
        return false;
    }
public:
    Hungarian() : matching_size(0), total_cost(0){}
    pair<T, vector<int> > solve(const vector<vector<T> >& cost){
        V = (int)cost.size();
        graph.resize(V, vector<int>(V, 0));
        potential.resize(2*V, std::numeric_limits<T>::max());
        allocate.resize(V, -1);
        reverse_alloc.resize(V, -1);
        initial_feasible_solution(cost);
        inital_graph_construction(cost);
        vector<pair<int, int> > vec;
        vector<int> visit(2*V, -1);
        bool cont = false;
        while(matching_size < V){
            if(cont){
                for(auto p : vec){
                    vector<int> path;
                    if(find_augmenting_path(p.second + V, visit[p.first], visit, path)){
                        cont = false;
                        break;
                    }
                }
                if(!cont) continue;
            }else{
                init_matching();
                for(int i = 0; i < 2*V; i++) visit[i] = -1;
                for(int i = 0; i < V; i++){
                    if(allocate[i] < 0 && visit[i] < 0) dfs(i, i, visit);
                }
            }
            T delta = std::numeric_limits<T>::max();
            for(int i = 0; i < V; i++){
                if(visit[i] < 0) continue;
                for(int j = V; j < 2*V; j++){
                    if(visit[j] < 0){
                        if(delta > diff(cost, i, j)){
                            delta = diff(cost, i, j);
                            vec = {{i, j-V}};
                        }else if(delta == diff(cost, i, j)){
                            vec.emplace_back(i, j-V);
                        }
                    }
                }
            }
            update_potential(visit, delta);
            update_graph(cost, vec, visit);
            cont = !increment_matching(vec, visit);
        }
        for(int i = 0; i < V; i++){
            total_cost += cost[i][allocate[i]];
        }
        return make_pair(total_cost, allocate);
    }
};
