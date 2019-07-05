#include "../header.hpp"

// 次数列 d とグラフ G (vector<vecotr<int> >型で予め頂点数分の配列を確保する), 連結であるべきかを与える.
// TIME は適宜設定する. (計算量 o(m^2))
class GraphGenerator {
private:
    int V;
    bool connected;
    vector<unordered_set<int> > G;
    vector<vector<pair<int, int> > > off_tree;
    vector<pair<int, int> > population;
    vector<int> visit, non_tree, tree;
    chrono::high_resolution_clock::time_point start;
    random_device rnd;
    mt19937 mt;
    const int TIME = 1000; // (ms)
    static constexpr double UP = 1.13108324944;
    static constexpr double DOWN = 0.92371260217;
    int _time() const noexcept {
        chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
        return (int)chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }
    bool check() const noexcept {
        if(*max_element(degree.begin(), degree.end()) >= V) return false;
        if(*min_element(degree.begin(), degree.end()) <= -(!connected)) return false;
        long long edge_size = accumulate(degree.begin(), degree.end(), 0LL);
        if(edge_size % 2 || (connected && edge_size < 2 * (V - 1))) return false;
        return true;
    }
    void add_edge(const int u, const int v) noexcept { G[u].insert(v), G[v].insert(u); }
    void erase_edge(const int u, const int v) noexcept { G[u].erase(v), G[v].erase(u); }
    void swap_edge(const pair<int, int>& p, const pair<int, int>& q) noexcept {
        erase_edge(p.first, p.second), erase_edge(q.first, q.second);
        add_edge(p.first, q.first), add_edge(p.second, q.second);
    }
    bool construct_init_graph() noexcept {
        int cur, ver, num, pos = *max_element(degree.begin(), degree.end());
        vector<stack<int> > bucket(V);
        for(int i = 0; i < V; i++) bucket[degree[i]].push(i);
        queue<pair<int, int> > memo;
        while(pos >= 1){
            if(bucket[pos].empty()){
                pos--;
                continue;
            }
            ver = bucket[pos].top(), num = pos;
            bucket[pos].pop();
            for(cur = pos; cur >= 1; cur--){
                while(num > 0 && !bucket[cur].empty()){
                    int tmp = bucket[cur].top();
                    bucket[cur].pop();
                    add_edge(ver, tmp);
                    memo.push({cur-1, tmp});
                    num--;
                }
                if(num == 0) break;
            }
            if(cur == 0) return false;
            while(!memo.empty()){
                pair<int, int> p = memo.front();
                memo.pop();
                if(p.first >= 1) bucket[p.first].push(p.second);
            }
        }
        return true;
    }
    void dfs(const int u, const int p, const int c) noexcept {
        visit[u] = 1;
        for(int v : G[u]){
            if(!visit[v]) dfs(v, u, c);
            else if(v != p && visit[v] == 1) off_tree[c].emplace_back(u, v);
        }
        visit[u] = 2;
    }
    void detect_component() noexcept {
        int comp_size = 0;
        for(int i = 0; i < V; i++){
            if(!visit[i]){
                off_tree.push_back(vector<pair<int, int> >());
                dfs(i, -1, comp_size++);
                if(off_tree.back().empty()){
                    off_tree.back().emplace_back(min(i, *G[i].begin()), -max(i, *G[i].begin()));
                }
            }
        }
        for(int i = 0; i < comp_size; i++){
            if(off_tree[i][0].second < 0) tree.push_back(i);
            else non_tree.push_back(i);
        }
    }
    void transform_to_connected_graph() noexcept {
        detect_component();
        while(!tree.empty()){
            int id = non_tree.back();
            while(!off_tree[id].empty() && !tree.empty()){
                pair<int, int> p = off_tree[id].back(), q = off_tree[tree.back()][0];
                q.second = -q.second;
                swap_edge(p, q);
                off_tree[id].pop_back(), tree.pop_back();
            }
            if(off_tree[id].empty()) non_tree.pop_back();
        }
        while((int)non_tree.size() >= 2){
            int q = non_tree.back(); non_tree.pop_back();
            int p = non_tree.back(); non_tree.pop_back();
            pair<int, int> e1 = off_tree[p].back(), e2 = off_tree[q].back();
            off_tree[p].pop_back(), off_tree[q].pop_back();
            swap_edge(e1, e2);
            off_tree[q].emplace_back(e1.first, e2.first);
            for(auto& e : off_tree[p]) off_tree[q].push_back(e);
            non_tree.push_back(q);
        }
    }
    void dfs(const int u, const int p, int& ver_sm, vector<bool>& _visit) const noexcept {
        _visit[u] = true, ver_sm++;
        for(int v : G[u]) if(!_visit[v]) dfs(v, u, ver_sm, _visit);
    }
    bool IsConnected() const noexcept {
        int ver_sm = 0;
        vector<bool> _visit(V, false);
        dfs(0, -1, ver_sm, _visit);
        return (ver_sm == V);
    }
    bool check_swap_edge(const pair<int, int>& p, const pair<int, int>& q) noexcept {
        if(G[p.first].find(q.first) != G[p.first].end()) return false;
        if(G[p.second].find(q.second) != G[p.second].end()) return false;
        erase_edge(p.first, p.second), erase_edge(q.first, q.second);
        add_edge(p.first, q.first), add_edge(p.second, q.second);
        return true;
    }
    bool transition(pair<int, int> p, pair<int, int> q, bool flag) noexcept {
        if(p.first == q.second || p.second == q.first || p.second == q.second) return false;
        if(flag) swap(q.first, q.second);
        return check_swap_edge(p, q);
    }
    void shuffle_graph() noexcept {
        for(int i = 0; i < V; i++) for(int j = 0; j < degree[i]; j++) population.emplace_back(i, j);
        uniform_int_distribution<> _rand(0, (int)population.size()-1);
        int window = 1;
        double ex_window = 1.0;
        while(true){
            vector<unordered_set<int> > tmpG;
            if(connected) tmpG = G;
            bool finish = false, done = false;
            for(int i = 0; i < window; i++){
                pair<int, int> id1 = {0, 0}, id2 = {0, 0};
                while(id1.first == id2.first) id1 = population[_rand(mt)], id2 = population[_rand(mt)];
                unordered_set<int>::iterator it1 = G[id1.first].begin(), it2 = G[id2.first].begin();
                advance(it1, id1.second), advance(it2, id2.second);
                done |= transition({id1.first, *it1}, {id2.first, *it2}, mt() % 2);
                if(_time() > TIME){ finish = true; break; }
            }
            if(!connected || !done || IsConnected()){
                loop_count += window, ex_window *= UP, window = round(ex_window);
            }
            else G = tmpG, ex_window *= DOWN, window = max((int)round(ex_window), 1);
            if(finish) break;
        }
    }
    void construct_graph() noexcept {
        vector<int> index(V);
        iota(index.begin(), index.end(), 0);
        shuffle(index.begin(), index.end(), mt);
        for(int i : index) for(int to : G[i]) graph[i].push_back(index[to]);
        for(int i = 0; i < V; i++) for(int to : G[i]) graph[i].push_back(to);
    }

public:
    int loop_count;
    vector<vector<int> >& graph;
    const vector<int>& degree;
    GraphGenerator(const vector<int>& d, vector<vector<int> >& g, bool _connected=false) noexcept
        : V((int)d.size()), connected(_connected), G(V), visit(V, 0),
            start(chrono::high_resolution_clock::now()), mt(rnd()), loop_count(0), graph{g}, degree{d}{
        assert(check());
        assert(construct_init_graph());
        if(connected) transform_to_connected_graph();
        shuffle_graph();
        construct_graph();
    }
};

struct pair_hash {
    template <class T1, class T2>
    size_t operator() (const pair<T1, T2>& p) const {
        size_t lhs = hash<T1>()(p.first), rhs = hash<T2>()(p.second);
        return lhs^(rhs+0x9e3779b9+(lhs<<6)+(lhs>>2));
    }
};

// (多重辺、自己ループも許した)ランダムグラフ (計算量 O(m))
void random_graph(const int node_size, const int edge_size, vector<vector<int> >& graph){
    random_device rnd;
    mt19937 mt(rnd());
    uniform_int_distribution<> _rand(0, node_size-1);
    for(int i = 0; i < edge_size; i++){
        int from = _rand(mt), to = _rand(mt);
        graph[from].push_back(to), graph[to].push_back(from);
    }
}

// G(n, p) (各辺が確率 p で存在) (平均計算量 O(np))
void given_probability_random_simple_graph(const int node_size, const double prob, vector<vector<int> >& graph){
    random_device rnd;
    mt19937 mt(rnd());
    uniform_real_distribution<double> _rand(0.0, 1.0);
    int v = 1, w = -1;
    while(v < node_size){
        w += 1 + floor(log(1.0 -_rand(mt))/log(1.0 - prob));
        while(w >= v && v < node_size) w -= v, v++;
        if(v < node_size) graph[v].push_back(w), graph[w].push_back(v);
    }
}

// G(n, m) ランダムな単純グラフ (平均計算量 O(m))
void random_simple_graph(const int node_size, int edge_size, vector<vector<int> >& graph){
    assert(edge_size <= (long long)(node_size)*(node_size-1)/2);
    bool complement = false;
    if((edge_size > (long long)(node_size)*(node_size-1)/4)){
        complement = true;
        edge_size = (long long)(node_size)*(node_size-1)/2 - edge_size;
    }
    unordered_set<pair<int, int>, pair_hash> edge_set;
    random_device rnd;
    mt19937 mt(rnd());
    uniform_int_distribution<> rand1(0, node_size-1), rand2(0, node_size-2);
    for(int i = 0; i < edge_size; i++){
        while(true){
            int from = rand1(mt), to = rand2(mt);
            if(to < from) swap(from, to); else to++;
            if(edge_set.find({from, to}) != edge_set.end()) continue;
            if(!complement){
                graph[from].push_back(to), graph[to].push_back(from);
            }
            edge_set.insert({from, to});
            break;
        }
    }
    if(complement){
        for(int i = 0; i < node_size-1; i++){
            for(int j = i+1; j < node_size; j++){
                if(edge_set.find({i, j}) == edge_set.end()){
                    graph[i].push_back(j), graph[j].push_back(i);
                }
            }
        }
    }
}

// ランダムな単純連結グラフ (平均計算量 O(m))
void random_simple_connected_graph(const int node_size, int edge_size, vector<vector<int> >& graph){
    assert(edge_size >= node_size - 1);
    assert(edge_size <= (long long)(node_size)*(node_size-1)/2);
    unordered_set<pair<int, int>, pair_hash> edge_set;
    random_device rnd;
    mt19937 mt(rnd());
    vector<int> ver(node_size);
    iota(ver.begin(), ver.end(), 0);
    shuffle(ver.begin(), ver.end(), mt);
    for(int i = 1; i < node_size; i++){
        int u = mt() % i;
        graph[ver[u]].push_back(ver[i]), graph[ver[i]].push_back(ver[u]);
        edge_set.insert({min(ver[i], ver[u]), max(ver[i], ver[u])});
    }
    edge_size -= node_size - 1;
    bool complement = false;
    if((edge_size > (long long)(node_size)*(node_size-1)/4)){
        complement = true;
        edge_size = (long long)(node_size)*(node_size-1)/2 - edge_size;
    }
    uniform_int_distribution<> rand1(0, node_size-1), rand2(0, node_size-2);
    for(int i = 0; i < edge_size; i++){
        while(true){
            int from = rand1(mt), to = rand2(mt);
            if(to < from) swap(from, to); else to++;
            if(edge_set.find({from, to}) != edge_set.end()) continue;
            if(!complement){
                graph[from].push_back(to), graph[to].push_back(from);
            }
            edge_set.insert({from, to});
            break;
        }
    }
    if(complement){
        for(int i = 0; i < node_size-1; i++){
            for(int j = i+1; j < node_size; j++){
                if(edge_set.find({i, j}) == edge_set.end()){
                    graph[i].push_back(j), graph[j].push_back(i);
                }
            }
        }
    }
}

// ランダムな木 (計算量 O(n))
void random_tree(const int node_size, const int edge_size, vector<vector<int> >& graph){
    random_device rnd;
    mt19937 mt(rnd());
    vector<int> ver(node_size);
    iota(ver.begin(), ver.end(), 0);
    shuffle(ver.begin(), ver.end(), mt);
    for(int i = 1; i < node_size; i++){
        int u = mt() % i;
        graph[ver[u]].push_back(ver[i]), graph[ver[i]].push_back(ver[u]);
    }
}
