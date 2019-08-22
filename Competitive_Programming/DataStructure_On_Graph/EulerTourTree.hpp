#include "../header.hpp"

template<typename _Tp> class BSTNode {
public:
    int from, to, sz;
    _Tp value, al, lazy;
    BSTNode *left, *right, *par;
    BSTNode(const int _from, const int _to) noexcept
        : from(_from), to(_to), sz(0), value(id1), al(id2), lazy(id1),
            left(nullptr), right(nullptr), par(nullptr){}
    BSTNode(const int _from, const int _to, const _Tp& _value) noexcept
        : from(_from), to(_to), sz(1), value(_value), al(value), lazy(id1),
            left(nullptr), right(nullptr), par(nullptr){}
    static const _Tp id1 = (_Tp)0;
    static const _Tp id2 = (_Tp)0;
    static void opr1(_Tp& arg1, const _Tp arg2) noexcept { arg1 += arg2; }
    static _Tp opr2(const _Tp arg1, const _Tp arg2) noexcept { return arg1 + arg2; }
    inline bool IsRoot() const noexcept { return !par; }
    inline bool IsVertex() const noexcept { return from == to; }
    void push() noexcept {
        if(lazy != id1){
            if(IsVertex()) opr1(value, lazy);
            al += lazy * sz;
            if(left) opr1(left->lazy, lazy);
            if(right) opr1(right->lazy, lazy);
            lazy = id1;
        }
    }
    void eval() noexcept {
        sz = IsVertex(), al = value;
        if(left) left->push(), sz += left->sz, al = opr2(left->al, al);
        if(right) right->push(), sz += right->sz, al = opr2(al, right->al);
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

template<typename _Tp>
BSTNode<_Tp>* splay(BSTNode<_Tp> *u) noexcept {
    if(!u) return nullptr;
    while(!(u->IsRoot())){
        BSTNode<_Tp> *p = u->par, *gp = p->par;
        if(p->IsRoot()){ // zig
            p->push(), u->push();
            u->rotate((u == p->left));
        }else{
            gp->push(), p->push(), u->push();
            bool flag = (u == p->left);
            if((u == p->left) == (p == gp->left)){ // zig-zig
                p->rotate(flag), u->rotate(flag);
            }else{ // zig-zag
                u->rotate(flag), u->rotate(!flag);
            }
        }
    }
    u->push();
    return u;
}

template<typename _Tp>
BSTNode<_Tp>* join(BSTNode<_Tp> *root1, BSTNode<_Tp> *root2) noexcept {
    if(!root1 || !root2) return root1 ? root1 : root2;
    BSTNode<_Tp> *cur = nullptr, *nx = root1;
    do{ cur = nx, cur->push(), nx = cur->right; }while(nx);
    BSTNode<_Tp> *ver = splay(cur);
    ver->right = root2, ver->eval(), root2->par = ver;
    return ver;
}

template<typename _Tp>
pair<BSTNode<_Tp>*, BSTNode<_Tp>*> split_lower_bond(BSTNode<_Tp> *ver) noexcept {
    BSTNode<_Tp> *res = splay(ver)->left;
    ver->left = nullptr, ver->eval();
    if(res) res->par = nullptr;
    return make_pair(res, ver);
}

template<typename _Tp>
pair<BSTNode<_Tp>*, BSTNode<_Tp>*> split_upper_bond(BSTNode<_Tp> *ver) noexcept {
    BSTNode<_Tp> *res = splay(ver)->right;
    ver->right = nullptr, ver->eval();
    if(res) res->par = nullptr;
    return make_pair(ver, res);
}

template<typename T> class EulerTourTree {
private:
    struct pair_hash {
        template <class T1, class T2>
        size_t operator() (const pair<T1, T2>& p) const {
            size_t lhs = hash<T1>()(p.first), rhs = hash<T2>()(p.second);
            return lhs^(rhs+0x9e3779b9+(lhs<<6)+(lhs>>2));
        }
    };
    BSTNode<T>** vertex_set;
    unordered_map<pair<int, int>, pair<BSTNode<T>*, BSTNode<T>*>, pair_hash> edge_set;
    BSTNode<T> *reroot(BSTNode<T> *ver) noexcept {
        BSTNode<T> *res = splay(ver)->left;
        if(!res) return ver;
        ver->left = nullptr, ver->eval();
        while(ver->right) ver->push(), ver = ver->right;
        splay(ver), ver->right = res, ver->eval(), res->par = ver;
        return ver;
    }
    void link(BSTNode<T> *ver1, BSTNode<T> *ver2) noexcept {
        BSTNode<T>* e1 = new BSTNode<T>(ver1->from, ver2->from);
        BSTNode<T>* e2 = new BSTNode<T>(ver2->from, ver1->from);
        edge_set[{ver1->from, ver2->from}] = {e1, e2};
        join(join(reroot(ver1), e1), join(reroot(ver2), e2));
    }
    void cut(BSTNode<T> *edge1, BSTNode<T> *edge2) noexcept {
        splay(edge1), splay(edge2);
        BSTNode<T> *p = edge1->par;
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
    bool IsConnected(BSTNode<T> *ver1, BSTNode<T> *ver2) noexcept {
        splay(ver1), splay(ver2);
        return ver1->par;
    }
    T& get(BSTNode<T> *ver) noexcept { return splay(ver)->value; }
    void range(BSTNode<T> *edge1, BSTNode<T> *edge2, const T& val) noexcept {
        auto res1 = split_lower_bond(edge1);
        auto res2 = split_upper_bond(edge2);
        BSTNode<T>::opr1(res2.first->lazy, val);
        join(join(res1.first,res2.first), res2.second);
    }
    T query(BSTNode<T> *edge1, BSTNode<T> *edge2) noexcept {
        auto res1 = split_lower_bond(edge1);
        auto res2 = split_upper_bond(edge2);
        T res = res2.first->al;
        return join(join(res1.first,res2.first), res2.second), res;
    }
public:
    const int V;
    EulerTourTree(const vector<T>& ver_value, bool helper=false) noexcept : V((int)ver_value.size()){
        vertex_set = new BSTNode<T>*[V];
        for(int i = 0; i < V; i++) vertex_set[i] = new BSTNode<T>(i, i, ver_value[i]);
        if(helper) G.resize(V);
    }
    // ~EulerTourTree(){
    //     for(auto it : edge_set){
    //         delete (it.second).first;
    //         delete (it.second).second;
    //     }
    //     for(int i = 0; i < V; ++i) delete vertex_set[i];
    //     delete[] vertex_set;
    // }
    // 根を node_id にする
    void reroot(const int node_id) noexcept { reroot(vertex_set[node_id]); }
    // 辺(node1_id, node2_id) を追加
    void link(int node1_id, int node2_id) noexcept {
        if(node1_id > node2_id) swap(node1_id, node2_id);
        link(vertex_set[node1_id], vertex_set[node2_id]);
    }
    // 辺(node1_id, node2_id) を削除(逆向きでも問題ない)
    void cut(int node1_id, int node2_id){
        if(node1_id > node2_id) swap(node1_id, node2_id);
        auto it = edge_set.find({node1_id, node2_id});
        assert(it != edge_set.end());
        BSTNode<T> *edge1 = (it->second).first, *edge2 = (it->second).second;
        edge_set.erase(it);
        cut(edge1, edge2);
    }
    // node1_id と node2_id が同じ木(連結成分)に属するか
    bool IsConnected(const int node1_id, const int node2_id) noexcept {
        if(node1_id == node2_id) return true;
        return IsConnected(vertex_set[node1_id], vertex_set[node2_id]);
    }
    // 頂点 ver_id の値を取得
    T& get(int ver_id) noexcept { return get(vertex_set[ver_id]); }
    // 頂点 ver_id の存在する連結成分内の頂点全体に val を加える
    void component_range(const int ver_id, const T& val){ range(ver_id, -1, val); }
    // 親が par_id であるような頂点 ver_id の部分木内に存在する頂点全体に val を加える
    void range(const int ver_id, const int par_id, const T& val){
        if(par_id < 0) return BSTNode<T>::opr1(splay(vertex_set[ver_id])->lazy, val);
        if(ver_id < par_id){
            auto it = edge_set.find({ver_id, par_id});
            assert(it != edge_set.end());
            range((it->second).second, (it->second).first, val);
        }else{
            auto it = edge_set.find({par_id, ver_id});
            assert(it != edge_set.end());
            range((it->second).first, (it->second).second, val);
        }
    }
    // 頂点 ver_id の存在する連結成分内の頂点全体の総和を取得
    T component_query(const int ver_id){ return query(ver_id, -1); }
    // 親が par_id であるような頂点 ver_id の部分木内に存在する頂点全体の総和を取得
    T query(const int ver_id, const int par_id){
        if(par_id < 0) return splay(vertex_set[ver_id])->al;
        if(ver_id < par_id){
            auto it = edge_set.find({ver_id, par_id});
            assert(it != edge_set.end());
            return query((it->second).second, (it->second).first);
        }else{
            auto it = edge_set.find({par_id, ver_id});
            assert(it != edge_set.end());
            return query((it->second).first, (it->second).second);
        }
    }

// ヘルパー関数
    vector<vector<int> > G;
    void add_edge(const int u, const int v) noexcept { G[u].push_back(v), G[v].push_back(u); }
    void dfs(const int u, const int p, const BSTNode<T> *cur,
        vector<int>& parent, vector<BSTNode<T>*>& nodes) noexcept {
        parent[u] = p;
        nodes.push_back(vertex_set[u]);
        for(auto& v : G[u]){
            if(v != p){
                BSTNode<T>* e1 = new BSTNode<T>(u, v);
                nodes.push_back(e1);
                dfs(v, u, cur, parent, nodes);
                BSTNode<T>* e2 = new BSTNode<T>(v, u);
                if(u < v) edge_set[{u, v}] = {e1, e2};
                else edge_set[{v, u}] = {e2, e1};
                nodes.push_back(e2);
            }
        }
    }
    void bst_build(vector<BSTNode<T>* >& nodes) noexcept {
        int i, n = (int)nodes.size(), st = 2, isolate = ((n % 4 == 1) ? (n-1) : -1);
        while(st <= n){
            for(i = st-1; i < n; i += 2*st){
                nodes[i]->left = nodes[i-st/2], nodes[i-st/2]->par = nodes[i];
                if(i+st/2 < n) nodes[i]->right = nodes[i+st/2], nodes[i+st/2]->par = nodes[i];
                else if(isolate >= 0) nodes[i]->right = nodes[isolate], nodes[isolate]->par = nodes[i];
                nodes[i]->eval();
            }
            isolate = ((n % (4*st) >= st && (n % (4*st) < 2*st)) ? (i-2*st): isolate);
            st <<= 1;
        }
    }
    // _root を根とする木を構築(親の vector を返す(reroot しなければ range や query で使える))
    vector<int> build_tree(const int _root) noexcept {
        vector<int> parent(V, -1);
        vector<BSTNode<T>* > nodes;
        BSTNode<T> *cur = nullptr;
        dfs(_root, -1, cur, parent, nodes);
        bst_build(nodes);
        return parent;
    }
    // 森を構築
    vector<int> build_forest() noexcept {
        vector<int> parent(V, -1);
        for(int i = 0; i < V; i++){
            if(parent[i] < 0){
                vector<BSTNode<T>* > nodes;
                BSTNode<T> *cur = nullptr;
                dfs(i, -1, cur, parent, nodes);
                bst_build(nodes);
            }
        }
        return parent;
    }

private:
    void check_dfs(const BSTNode<T>* cur) const noexcept {
        cur->push();
        if(cur->left) check_dfs(cur->left);
        if(cur->IsVertex()) cout <<"{" << (cur->from) << "," << (cur->value) << "} ";
        if(cur->right) check_dfs(cur->right);
    }
};
