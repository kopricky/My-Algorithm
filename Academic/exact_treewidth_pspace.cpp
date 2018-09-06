//Bodlaender et al. 2006 を参照
#include<iostream>
#include<vector>
#include<algorithm>
#include<time.h>
#define INF 1000000005

const int MAX_SIZE = 25;

std::vector<int> graph[MAX_SIZE];

void solve_Q_size(const int v, int cur_vertex, const int subset, int& Q_size, int& visited)
{
    visited |= (1 << cur_vertex);
    for(int i = 0; i < (int)graph[cur_vertex].size(); i++){
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

int Recursive_Treewidth(int set1, int set2)
{
    int set2_size = __builtin_popcount(set2);
    if(set2_size == 1){
        int Q_size = 0;
        int visited = 0;
        int v = __builtin_ffs(set2) - 1;
        solve_Q_size(v, v, set1, Q_size, visited);
        return Q_size;
    }
    int ans = INF;
    int subset_size = set2_size / 2;
    std::vector<int> set2_element(set2_size, 0);
    int temp = set2;
    int shift_count = 0, add_count = 0;
    while(temp){
        if(temp & 1){
            set2_element[add_count++] = shift_count;
        }
        shift_count++;
        temp >>= 1;
    }
    std::vector<bool> bitmask(set2_size, false);
    std::fill(bitmask.end() - subset_size, bitmask.end(), true);
    do {
        int subset = 0;
        for(int i = 0; i < set2_size; i++){
            if(bitmask[i]){
                subset += (1 << set2_element[i]);
            }
        }
        int cand1 = Recursive_Treewidth(set1, subset);
        int cand2 = Recursive_Treewidth(set1 | subset, set2 ^ subset);
        ans = std::min(ans, std::max(cand1, cand2));
    } while (std::next_permutation(bitmask.begin(), bitmask.end()));
    return ans;
}

int solve_treewidth(int node_size)
{
    int V = (1 << node_size) -  1;
    return Recursive_Treewidth(0, V);
}

int main()
{
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
