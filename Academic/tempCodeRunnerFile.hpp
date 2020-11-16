void uniformly_random_tree(const int node_size, vector<vector<int> >& graph){
    random_device rnd;
    mt19937 mt(rnd());
    uniform_int_distribution<> _rand(0, node_size - 1);
    vector<int> prufer_code(node_size - 2);
    vector<int> used(node_size, 0);
    set<int> unused;
    for(int i = 0; i < node_size - 2; ++i){
        prufer_code[i] = _rand(mt);
        ++used[prufer_code[i]];
    }
    for(int i = 0; i < node_size; ++i){
        if(!used[i]) unused.insert(i);
    }
    for(int i = 0; i < node_size - 2; ++i){
        const int ver = *unused.begin();
        graph[prufer_code[i]].push_back(ver);
        graph[ver].push_back(prufer_code[i]);
        unused.erase(unused.begin());
        if(--used[prufer_code[i]] == 0){
            unused.insert(prufer_code[i]);
        }
    }
}