#include "./header.hpp"

// 単純グラフと仮定する
class BSTNode {
public:
    const int from, to;
    int sz;
    bool subtree_edge, subofftree_edge, exact_level;
    BSTNode *left, *right, *par;
    unordered_set<int> adjacent;
    BSTNode(const int _ver) noexcept :
        from(_ver), to(_ver), sz(0), subtree_edge(false), subofftree_edge(false),
            exact_level(false), left(nullptr), right(nullptr), par(nullptr){}
    BSTNode(const int _from, const int _to, const bool _flag) noexcept :
        from(_from), to(_to), sz(0), subtree_edge(false), subofftree_edge(false),
            exact_level((from < to) && _flag), left(nullptr), right(nullptr), par(nullptr){}
    inline bool IsRoot() const noexcept { return !par; }
    inline bool IsVertex() const noexcept { return (from == to); }
    inline void eval() noexcept {
        if(IsVertex()) sz = 1, subtree_edge = false, subofftree_edge = !adjacent.empty();
        else sz = 0, subtree_edge = exact_level, subofftree_edge = false;
        if(left){
            sz += left->sz, subtree_edge |= left->subtree_edge, subofftree_edge |= left->subofftree_edge;
        }
        if(right){
            sz += right->sz, subtree_edge |= right->subtree_edge, subofftree_edge |= right->subofftree_edge;
        }
    }
    inline void subtree_edge_eval(){
        subtree_edge = exact_level;
        if(left) subtree_edge |= left->subtree_edge;
        if(right) subtree_edge |= right->subtree_edge;
    }
    inline void subofftree_edge_eval(){
        subofftree_edge = !adjacent.empty();
        if(left) subofftree_edge |= left->subofftree_edge;
        if(right) subofftree_edge |= right->subofftree_edge;
    }
    inline bool subofftree_check(){
        return !adjacent.empty() ||
                (left ? left->subofftree_edge : false) || (right ? right->subofftree_edge : false);
    }
    inline bool offtree_check(){
        return adjacent.empty() ||
                (left ? left->subofftree_edge : false) || (right ? right->subofftree_edge : false);
    }
    void rotate(const bool right_) noexcept {
        BSTNode *p = par, *g = p->par;
        if(right_){
            if((p->left = right)) right->par = p;
            right = p, p->par = this;
        }else{
            if((p->right = left)) left->par = p;
            left = p, p->par = this;
        }
        p->eval(), eval();
        if(!(par = g)) return;
        if(g->left == p) g->left = this;
        if(g->right == p) g->right = this;
        g->eval();
    }
};

BSTNode *splay(BSTNode *u) noexcept {
    if(!u) return nullptr;
    while(!(u->IsRoot())){
        BSTNode *p = u->par, *gp = p->par;
        if(p->IsRoot()){ // zig
            u->rotate((u == p->left));
        }else{
            bool flag = (u == p->left);
            if((u == p->left) == (p == gp->left)){ // zig-zig
                p->rotate(flag), u->rotate(flag);
            }else{ // zig-zag
                u->rotate(flag), u->rotate(!flag);
            }
        }
    }
    return u;
}

BSTNode *join(BSTNode *root1, BSTNode *root2) noexcept {
    if(!root1 || !root2) return root1 ? root1 : root2;
    BSTNode *cur = nullptr, *nx = root1;
    do{ cur = nx, nx = cur->right; }while(nx);
    BSTNode *ver = splay(cur);
    ver->right = root2, ver->eval(), root2->par = ver;
    return ver;
}

class EulerTourTree {
public:
    struct pair_hash {
        template <class T1, class T2>
        size_t operator() (const pair<T1, T2>& p) const {
            size_t lhs = hash<T1>()(p.first), rhs = hash<T2>()(p.second);
            return lhs^(rhs+0x9e3779b9+(lhs<<6)+(lhs>>2));
        }
    };
    BSTNode** vertex_set;
    unordered_map<pair<int, int>, pair<BSTNode*, BSTNode*>, pair_hash> edge_set;
private:
    BSTNode *reroot(BSTNode *ver) noexcept {
        BSTNode *res = splay(ver)->left;
        if(!res) return ver;
        ver->left = nullptr, ver->eval();
        while(ver->right) ver = ver->right;
        splay(ver), ver->right = res, ver->eval(), res->par = ver;
        return ver;
    }
    void link(BSTNode *ver1, BSTNode *ver2, const bool flag) noexcept {
        BSTNode *e1 = new BSTNode(ver1->from, ver2->from, flag);
        BSTNode *e2 = new BSTNode(ver2->from, ver1->from, flag);
        edge_set[{ver1->from, ver2->from}] = {e1, e2};
        join(join(reroot(ver1), e1), join(reroot(ver2), e2));
    }
    void cut(BSTNode *edge1, BSTNode *edge2) noexcept {
        splay(edge1), splay(edge2);
        BSTNode *p = edge1->par;
        bool _right = (edge1 == edge2->right);
        if(p != edge2){
            _right = (p == edge2->right);
            p->par = nullptr, edge1->rotate((edge1 == p->left));
        }
        if(edge1->left) edge1->left->par = nullptr;
        if(edge1->right) edge1->right->par = nullptr;
        if(_right){
            if(edge2->left) edge2->left->par = nullptr;
            join(edge2->left, edge1->right);
        }else{
            if(edge2->right) edge2->right->par = nullptr;
            join(edge1->left, edge2->right);
        }
        delete edge1; delete edge2;
    }
    bool IsConnected(BSTNode *ver1, BSTNode *ver2) noexcept {
        splay(ver1), splay(ver2);
        return ver1->par;
    }
    int component_size(BSTNode *ver) noexcept { return splay(ver)->sz; }
public:
    int V;
    EulerTourTree(){}
    // ~EulerTourTree(){
    //     for(auto it : edge_set){
    //         delete (it.second).first;
    //         delete (it.second).second;
    //     }
    //     for(int i = 0; i < V; ++i) delete vertex_set[i];
    //     delete[] vertex_set;
    // }
    void resize(const int node_size) noexcept {
        V = node_size, vertex_set = new BSTNode*[V];
        for(int i = 0; i < V; i++) vertex_set[i] = new BSTNode(i);
    }
    void reroot(const int node_id) noexcept { reroot(vertex_set[node_id]); }
    void link(int node1_id, int node2_id, bool flag=true) noexcept {
        if(node1_id > node2_id) swap(node1_id, node2_id);
        link(vertex_set[node1_id], vertex_set[node2_id], flag);
    }
    void cut(int node1_id, int node2_id){
        if(node1_id > node2_id) swap(node1_id, node2_id);
        auto it = edge_set.find({node1_id, node2_id});
        assert(it != edge_set.end());
        BSTNode *edge1 = (it->second).first, *edge2 = (it->second).second;
        edge_set.erase(it);
        cut(edge1, edge2);
    }
    bool IsConnected(const int node1_id, const int node2_id) noexcept {
        if(node1_id == node2_id) return true;
        return IsConnected(vertex_set[node1_id], vertex_set[node2_id]);
    }
    int component_size(int node_id) noexcept { return component_size(vertex_set[node_id]); }
    void check_dfs(const BSTNode* cur) const noexcept {
        if(cur->left) check_dfs(cur->left);
        cout << "{" << (cur->from) << "," << (cur->to) << "} ";
        if(cur->right) check_dfs(cur->right);
    }
};

class DynamicConnectivity {
private:
    BSTNode *level_up_dfs(BSTNode *cur, const int layer) noexcept {
        if(cur->exact_level){
            splay(cur)->exact_level = false, cur->subtree_edge_eval();
            detect_layer[{cur->from, cur->to}]++, et[layer+1].link(cur->from, cur->to);
            return cur;
        }
        if(cur->left && cur->left->subtree_edge) return level_up_dfs(cur->left, layer);
        if(cur->right && cur->right->subtree_edge) return level_up_dfs(cur->right, layer);
        return nullptr;
    }
    BSTNode *search_edge_dfs
        (BSTNode *cur, const int layer, const int another, bool& flag, pair<int, int>& rep_edge) noexcept {
        if(!cur->adjacent.empty()){
            bool state = et[layer+1].vertex_set[cur->from]->adjacent.empty();
            for(auto it = cur->adjacent.begin(); it != cur->adjacent.end();){
                pair<int, int> e = {min(cur->from, *it), max(cur->from, *it)};
                BSTNode *correspond = et[layer].vertex_set[*it];
                if(et[layer].IsConnected(another, *it)){
                    flag = true, rep_edge = e;
                    cur->adjacent.erase(it), correspond->adjacent.erase(cur->from);
                    if(!correspond->subofftree_check()){
                        splay(correspond)->subofftree_edge_eval();
                    }
                    break;
                }else{
                    if(!et[layer+1].vertex_set[*it]->subofftree_check()){
                        splay(et[layer+1].vertex_set[*it])->subofftree_edge = true;
                    }
                    et[layer+1].vertex_set[cur->from]->adjacent.insert(*it);
                    et[layer+1].vertex_set[*it]->adjacent.insert(cur->from);
                    detect_layer[e]++, it = cur->adjacent.erase(it);
                    correspond->adjacent.erase(cur->from);
                    if(!correspond->subofftree_check()){
                        splay(correspond)->subofftree_edge_eval();
                    }
                }
            }
            if(state && !et[layer+1].vertex_set[cur->from]->offtree_check()){
                splay(et[layer+1].vertex_set[cur->from])->subofftree_edge = true;
            }
            splay(cur)->subofftree_edge_eval();
            return cur;
        }
        if(cur->left && cur->left->subofftree_edge){
            return search_edge_dfs(cur->left, layer, another, flag, rep_edge);
        }
        if(cur->right && cur->right->subofftree_edge){
            return search_edge_dfs(cur->right, layer, another, flag, rep_edge);
        }
        return nullptr;
    }
    bool replace(const int from, const int to, const int layer) noexcept {
        if(layer < 0) return true;
        int u, v;
        if(et[layer].component_size(from) <= et[layer].component_size(to)) u = from, v = to;
        else u = to, v = from;
        BSTNode *ver = splay(et[layer].vertex_set[u]);
        while(ver->subtree_edge) ver = level_up_dfs(ver, layer);
        pair<int, int> rep_edge = {-1, -1};
        bool flag = false;
        while(ver->subofftree_edge){
            ver = search_edge_dfs(ver, layer, v, flag, rep_edge);
            if(flag) break;
        }
        if(flag){
            et[layer].link(rep_edge.first, rep_edge.second);
            for(int i = 0; i < layer; i++) et[i].link(rep_edge.first, rep_edge.second, false);
            return false;
        }else return replace(from, to, layer-1);
    }
public:
    const int V, layer_count;
    EulerTourTree* et;
    unordered_map<pair<int, int>, int, EulerTourTree::pair_hash> detect_layer;
    DynamicConnectivity(int node_size) noexcept : V(node_size), layer_count(ceil(log2(V))+1){
        et = new EulerTourTree[layer_count];
        for(int i = 0; i < layer_count; i++) et[i].resize(V);
    }
    bool link(int node1_id, int node2_id) noexcept {
        if(node1_id > node2_id) swap(node1_id, node2_id);
        detect_layer[{node1_id, node2_id}] = 0;
        if(et[0].IsConnected(node1_id, node2_id)){
            BSTNode *ver1 = et[0].vertex_set[node1_id], *ver2 = et[0].vertex_set[node2_id];
            splay(ver1)->subofftree_edge = true, ver1->adjacent.insert(node2_id);
            splay(ver2)->subofftree_edge = true, ver2->adjacent.insert(node1_id);
            return false;
        }else{
            et[0].link(node1_id, node2_id);
            return true;
        }
    }
    bool cut(int node1_id, int node2_id) noexcept {
        if(node1_id > node2_id) swap(node1_id, node2_id);
        auto it = detect_layer.find({node1_id, node2_id});
        assert(it != detect_layer.end());
        int layer = it->second;
        detect_layer.erase(it);
        auto& st = et[layer].vertex_set[node1_id]->adjacent;
        if(st.find(node2_id) == st.end()){
            for(int i = 0; i <= layer; i++) et[i].cut(node1_id, node2_id);
            return replace(node1_id, node2_id, layer);
        }else{
            et[layer].vertex_set[node1_id]->adjacent.erase(node2_id);
            if(!et[layer].vertex_set[node1_id]->subofftree_check()){
                splay(et[layer].vertex_set[node1_id])->subofftree_edge_eval();
            }
            et[layer].vertex_set[node2_id]->adjacent.erase(node1_id);
            if(!et[layer].vertex_set[node2_id]->subofftree_check()){
                splay(et[layer].vertex_set[node2_id])->subofftree_edge_eval();
            }
            return false;
        }
    }
    bool IsConnected(const int node1_id, const int node2_id) noexcept {
        return et[0].IsConnected(node1_id, node2_id);
    }
};
