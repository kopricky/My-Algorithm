#include "../header.hpp"

template<typename T> class Node {
public:
    pair<T, int> val, al;
    T lazy;
    Node *left, *right, *par;
    Node() : lazy(0), left(nullptr), right(nullptr), par(nullptr){}
    void reset(){
        left = right = par = nullptr;
    }
    bool isRoot() const {
        return (!par) || (par->left != this && par->right != this);
    }
    void push(){
        if(lazy){
            val.first += lazy, al.first += lazy;
            if(left) left->lazy += lazy;
            if(right) right->lazy += lazy;
            lazy = 0;
        }
    }
    void eval(){
        al = val;
        if(left){
            left->push();
            if((left->al).first < al.first) al = left->al;
        }
        if(right){
            right->push();
            if((right->al).first <= al.first) al = right->al;
        }
    }
    void rotate(bool right){
        Node *p = this->par, *g = p->par;
        if(right){
            if((p->left = this->right)) this->right->par = p;
            this->right = p, p->par = this;
        }else{
            if((p->right = this->left)) this->left->par = p;
            this->left = p, p->par = this;
        }
        p->eval(), this->eval(), this->par = g;
        if(!g) return;
        if(g->left == p) g->left = this;
        if(g->right == p) g->right = this;
        g->eval();
    }
    void splay(){
        while(!(this->isRoot())){
            Node *p = this->par, *gp = p->par;
            if(p->isRoot()){
                p->push(), this->push();
                this->rotate((this == p->left));
            }else{
                gp->push(), p->push(), this->push();
                bool flag = (this == p->left);
                if((this == p->left) == (p == gp->left)){
                    p->rotate(flag), this->rotate(flag);
                }else{
                    this->rotate(flag), this->rotate(!flag);
                }
            }
        }
        this->push();
    }
};

template<typename T> class Dinic {
private:
    const int V;
    vector<int> que, level, iter, used;
    vector<Node<T> > arr;
    void access(Node<T> *u){
        Node<T> *last = nullptr;
        for(Node<T>* v = u; v; v = v->par){
            if((v->val).second == -1){
                last->par = nullptr;
                break;
            }
            v->splay(), v->right = last, v->eval(), last = v;
        }
        u->splay();
    }
    void link(Node<T> *u, Node<T> *v){
        access(u), u->right = v, u->eval(), v->par = u;
    }
    void set(Node<T> *u, const T x, const int id){
        u->val = {x, id}, u->eval();
    }
    int prev(Node<T> *u){
        (u = u->right)->push();
        while(u->left){
            (u = u->left)->push();
        }
        u->splay();
        return (u->val).second;
    }
    int top(Node<T> *u){
        while(u->left){
            (u = u->left)->push();
        }
        u->splay();
        return (u->val).second;
    }
    void range(Node<T> *u, const T x){
        u->lazy += x, u->push();
    }
    void cut_and_reflect(const int ver){
        Node<T> *u = &arr[ver];
        u->splay();
        if(u->left) u->left->par = nullptr, u->left = nullptr, u->eval();
        edge& e = G[ver][iter[ver]++];
        G[e.to][e.rev].rev_cap += e.rev_cap - (u->val).first, e.rev_cap = (u->val).first;
    }
    void bfs(const int s, const int t){
        level[s] = 0, used[s] = 1;
        int qh = 0, qt = 0;
        for(que[qt++] = s; qh < qt;){
            int v = que[qh++];
            for(edge& e : G[v]){
                if(level[e.to] < 0 && G[e.to][e.rev].rev_cap > 0){
                    level[e.to] = level[v] + 1, used[e.to] = 1, que[qt++] = e.to;
                }
            }
        }
    }
    T block_flow_naive(const int s, int cur, T f){
        if(s == cur) return f;
        T flow = 0;
        for(int& i = iter[cur]; i < (int)G[cur].size(); ++i){
            edge& e = G[cur][i];
            if(e.rev_cap > 0 && level[e.to] < level[cur]){
                T d = block_flow_naive(s, e.to, min(f, e.rev_cap));
                if(d > 0){
                    e.rev_cap -= d, G[e.to][e.rev].rev_cap += d;
                    f -= d, flow += d;
                    if(f == 0) break;
                }
            }
        }
        return flow;
    }
    T block_flow(const int s, const int t){
        T flow = 0;
        bool find = false;
        int cur = t;
        while(true){
            if(find){
                const pair<T, int>& res = arr[cur].al;
                flow += res.first;
                range(&arr[cur], -res.first);
                cut_and_reflect(cur = res.second);
                find = false;
            }
            used[cur] = 2;
            bool update = false;
            for(int& i = iter[cur]; i < (int)G[cur].size(); ++i){
                edge& e = G[cur][i];
                if(e.rev_cap > 0 && level[e.to] < level[cur] && used[e.to] >= 1){
                    update = true;
                    set(&arr[cur], e.rev_cap, cur);
                    if(e.to == s){
                        find = true;
                    }else if(used[e.to] == 1){
                        arr[e.to].right = &arr[cur];
                        arr[cur].par = &arr[e.to], cur = e.to;
                    }else{
                        link(&arr[e.to], &arr[cur]);
                        const pair<T, int>& res = arr[e.to].al;
                        if(res.first == 0){
                            cut_and_reflect(cur = res.second);
                        }else{
                            if(level[cur = top(&arr[e.to])] == 1) find = true;
                            else cut_and_reflect(cur);
                        }
                    }
                    break;
                }
            }
            if(update) continue;
            used[cur] = -1, (arr[cur].val).second = -1;
            if(cur == t) break;
            cut_and_reflect(cur = prev(&arr[cur]));
        }
        return flow;
    }
    void update_dfs(Node<T>* u, auto& G, const vector<int>& iter, vector<int>& used){
        u->push();
        if(u->left) update_dfs(u->left, G, iter, used);
        if(u->right) update_dfs(u->right, G, iter, used);
        const int id = (u->val).second;
        if(iter[id] < (int)G[id].size()){
            auto& e = G[id][iter[id]];
            G[e.to][e.rev].rev_cap += e.rev_cap - (u->val).first;
            e.rev_cap = (u->val).first;
        }
        used[id] = 0, u->reset();
    }
    void update_and_clear(auto& G, const vector<int>& iter, vector<int>& used){
        for(int i = 0; i < V; ++i){
            if(used[i] == 2 && arr[i].isRoot()) update_dfs(&arr[i], G, iter, used);
            else if(used[i] == -1) arr[i].reset();
        }
    }

public:
    struct edge {
        const int to, rev;
        T rev_cap;
        edge(const int _to, const int _rev, const T _rev_cap) :
            to(_to), rev(_rev), rev_cap(_rev_cap){}
    };
    vector<vector<edge> > G;
    Dinic(const int node_size)
        : V(node_size), que(V), level(V), iter(V), used(V), arr(V), G(V){}
    void add_edge(const int from, const int to, const T cap){
        G[from].emplace_back(to, (int)G[to].size(), (T)0);
        G[to].emplace_back(from, (int)G[from].size() - 1, cap);
    }
    T solve(const int s, const int t){
        T flow = 0;
        while(true){
            fill(level.begin(), level.end(), -1);
            fill(used.begin(), used.end(), 0);
            bfs(s, t);
            if(level[t] < 0) break;
            fill(iter.begin(), iter.end(), 0);
            flow += block_flow(s, t);
            update_and_clear(G, iter, used);
        }
        return flow;
    }
};
