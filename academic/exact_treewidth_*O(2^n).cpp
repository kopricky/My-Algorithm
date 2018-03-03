//Bodlaender et al. 2006 を参照
#include<iostream>
#include<vector>
#include<algorithm>
#include<set>
#define INF 1000000005

const int MAX_SIZE = 25;

std::vector<int> graph[MAX_SIZE];

std::pair<int, int> maximum_clique(int node_size)
{
    std::vector<std::vector<bool> > connected(node_size, std::vector<bool>(node_size, false));
    for(int i = 0; i < node_size; i++){
        for(int j = 0; j < (int)graph[i].size(); j++){
            connected[i][graph[i][j]] = true;
        }
    }
    int ans_value = 0;
    int ans_set = 0;
    for(int S = (1 << node_size) - 1; S >= 1; S--){
        int S_size = __builtin_popcount(S);
        if(S_size <= ans_value){
            continue;
        }
        bool ok = true;
        for(int i = 0; i < node_size; i++){
            for(int j = 0; j < i; j++){
                if(((S >> i) & (S >> j) & 1) && !connected[i][j]){
                    ok = false;
                }
            }
        }
        if(ok){
            ans_value = S_size;
            ans_set = S;
        }
    }
    return std::make_pair(ans_value, ans_set);
}

void solve_Q_size(const int v, int cur_vertex, const int subset, int& Q_size, int& visited)
{
    visited |= (1 << cur_vertex);
    for(int i = 0; i < graph[cur_vertex].size(); i++){
        int next_vertex = graph[cur_vertex][i];
        //一度訪れた点はたどらない
        if(visited & (1 << next_vertex)){
            continue;
        }
        //subsetに含まれていたらさらにたどる,含まれていなかったらQ_sizeを増やして終了
        if(subset & (1 << next_vertex)){
            solve_Q_size(v, next_vertex, subset, Q_size, visited);
        }else{
            visited |= (1 << next_vertex);
            Q_size++;
        }
    }
    return;
}

int solve_treewidth(int node_size)
{
    int V = (1 << node_size) - 1;
    std::pair<int, int> res = maximum_clique(node_size);
    int max_clique_size = res.first, max_clique = res.second;
    int upper_bound = node_size - 1;
    std::set<std::pair<int, int> > TW[MAX_SIZE];
    TW[0].insert(std::make_pair(0, -INF) );
    for(int i = 1; i <= node_size - max_clique_size; i++){
        for(const std::pair<int, int> element: TW[i-1]){
            int subset = element.first;
            int TW_value = element.second;
            for(int v = 0; v < node_size; v++){
                if(!(subset & (1 << v))){
                    int next_subset = subset ^ (1 << v);
                    int Q_size = 0;
                    int visited = 0;
                    solve_Q_size(v, v, subset, Q_size, visited);
                    int candidate = std::max(TW_value, Q_size);
                    if(candidate < upper_bound){
                        upper_bound = std::min(upper_bound, node_size - i);
                        std::set<std::pair<int, int> >::iterator it =
                            std::lower_bound(TW[i].begin(), TW[i].end(), std::make_pair(next_subset, -INF));
                        if(it == TW[i].end()){
                            TW[i].insert(std::make_pair(next_subset, candidate));
                            continue;
                        }
                        std::pair<int, int> it_value = *it;
                        //存在していたら
                        if(it_value.first == next_subset){
                            if(candidate < it_value.second){
                                TW[i].erase(it);
                                it_value.second = candidate;
                                TW[i].insert(it_value);
                            }
                        }else{
                            TW[i].insert(std::make_pair(next_subset, candidate));
                        }
                    }
                }
            }
        }
        TW[i-1].clear();
    }
    std::set<std::pair<int, int> >::iterator it =
        std::lower_bound(TW[node_size - max_clique_size].begin(), TW[node_size - max_clique_size].end(), std::make_pair(V ^ max_clique, -INF));
    if(it == TW[node_size - max_clique_size].end()){
        return upper_bound;
    }
    std::pair<int, int> it_value = *it;
    if(it_value.first == V ^ max_clique && it_value.first != 0){
        return it_value.second;
    }else{
        return upper_bound;
    }
}

int main(){
    int node_size, edge_size;
    std::cin >> node_size >> edge_size;
    for(int i = 0; i < edge_size; i++){
        int u,v;
        std::cin >> u >> v;
        graph[u].push_back(v), graph[v].push_back(u);
    }
    clock_t start = clock();    // スタート時間
    std::cout << solve_treewidth(node_size) << std::endl;
    clock_t end = clock();     // 終了時間
    std::cout << "duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";
}
