#include "../header.hpp"

bool given_degree_simple_graph(const vector<int>& degree, vector<vector<int> >& graph){
    const int node_size = (int)degree.size();
    const int max_degree = *max_element(degree.begin(), degree.end());
    if(max_degree > node_size - 1) return false;
    if(accumulate(degree.begin(), degree.end(), 0LL) % 2 == 1) return false;
    graph.resize(node_size);
    vector<stack<int> > bucket(max_degree + 1);
    vector<int> deg_cnt(max_degree + 1, 0);
    for(int i = 0; i < node_size; ++i) bucket[degree[i]].push(i);
    vector<pair<int, int> > deg_seq;
    for(int i = max_degree; i >= 1; --i){
        deg_cnt[i] = (int)bucket[i].size();
        while(!bucket[i].empty()){
            deg_seq.emplace_back(i, bucket[i].top()), bucket[i].pop();
        }
    }
    while(!deg_seq.empty()){
        int pos = 0, rem = deg_seq.back().first, cur_degree;
        const int ver = deg_seq.back().second;
        --deg_cnt[rem], deg_seq.pop_back();
        stack<int> update;
        while(rem > 0){
            if(pos >= (int)deg_seq.size()) return false;
            cur_degree = deg_seq[pos].first;
            const int start = max(pos, pos + deg_cnt[cur_degree] - rem);
            for(int i = start; i < pos + deg_cnt[cur_degree]; ++i){
                graph[ver].push_back(deg_seq[i].second), graph[deg_seq[i].second].push_back(ver);
                --deg_seq[i].first, update.push(cur_degree);
            }
            pos += deg_cnt[cur_degree], rem -= deg_cnt[cur_degree];
        }
        while(!update.empty()){
            --deg_cnt[update.top()], ++deg_cnt[update.top() - 1], update.pop();
        }
        while(!deg_seq.empty() && deg_seq.back().first == 0) deg_seq.pop_back();
    }
    return true;
}
