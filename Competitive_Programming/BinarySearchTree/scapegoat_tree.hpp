#include "../header.hpp"

template<typename _Key> class ScapegoatTree {
private:
    struct node {
        const _Key key;
        unsigned int size;
        node *left, *right;
        node(const _Key& _key) : key(_key), size(1), left(nullptr), right(nullptr){}
        void eval(){
            size = 1;
            if(left) size += left->size;
            if(right) size += right->size;
        }
    };
    bool find(const node *cur, const _Key& key) const {
        while(cur){
            if(cur->key == key) return true;
            cur = (key < cur->key ? cur->left : cur->right);
        }
        return false;
    }
    void subtree_dfs(node *cur, vector<node*>& euler_tour) const {
        if(cur->left) subtree_dfs(cur->left, euler_tour);
        euler_tour.push_back(cur);
        if(cur->right) subtree_dfs(cur->right, euler_tour);
    }
    node *build_pbbt_rec(const int l, const int r, const vector<node*>& euler_tour){
        if(r - l == 0) return nullptr;
        else if(r - l == 1){
            node * const cur = euler_tour[l];
            cur->left = cur->right = nullptr;
            return cur->eval(), cur;
        }
        const int mid = (l + r) / 2;
        node * const cur = euler_tour[mid];
        cur->left = build_pbbt_rec(l, mid, euler_tour);
        cur->right = build_pbbt_rec(mid + 1, r, euler_tour);
        return cur->eval(), cur;
    }
    node *build_perfect_balanced_binary_tree(node *cur){
        if(!cur) return nullptr;
        vector<node*> euler_tour;
        subtree_dfs(cur, euler_tour);
        return build_pbbt_rec(0, (int)euler_tour.size(), euler_tour);
    }
    node *insert(node *cur, node *new_node, int depth, bool& balanced){
        if(!cur){
            const int sz = size() + 1;
            balanced = (depth <= floor(log_val * log2(sz)));
            return new_node;
        }else if(cur->key <= new_node->key){
            cur->right = insert(cur->right, new_node, depth + 1, balanced);
            cur->eval();
            if(balanced || cur->right->size <= alpha * cur->size) return cur;
        }else{
            cur->left = insert(cur->left, new_node, depth + 1, balanced);
            cur->eval();
            if(balanced || cur->left->size <= alpha * cur->size) return cur;
        }
        balanced = true;
        return build_perfect_balanced_binary_tree(cur);
    }
    node *join(node *left, node *right){
        if(!left || !right) return left ? left : right;
        else if(left->size < right->size){
            right->left = join(left, right->left);
            return right->eval(), right;
        }else{
            left->right = join(left->right, right);
            return left->eval(), left;
        }
    }
    void check(){
        if(size() >= alpha * max_element_size) return;
        root = build_perfect_balanced_binary_tree(root);
        max_element_size = size();
    }
    pair<node*, bool> erase(node *cur, const _Key& key){
        if(!cur) return {nullptr, false};
        else if(cur->key == key){
            node *res = join(cur->left, cur->right);
            delete cur;
            return {res, true};
        }else if(key < cur->key){
            pair<node*, bool> res = erase(cur->left, key);
            cur->left = res.first;
            return cur->eval(), make_pair(cur, res.second);
        }else{
            pair<node*, bool> res = erase(cur->right, key);
            cur->right = res.first;
            return cur->eval(), make_pair(cur, res.second);
        }
    }
    void clear(node *cur){
        if(cur->left) clear(cur->left);
        if(cur->right) clear(cur->right);
        delete cur;
    }
    node *root;
public:
    const double alpha, log_val;
    size_t max_element_size;
    ScapegoatTree(const double _alpha=2.0/3.0)
        : root(nullptr), alpha(_alpha), log_val(-1.0 / log2(alpha)), max_element_size(0){}
    // ~ScapegoatTree(){ if(root) clear(root); }
    bool empty() const { return !root; }
    size_t size() const { return empty() ? 0 : root->size; }
    bool find(const _Key& key) const { return find(root, key); }
    void insert(const _Key& key){
        bool balanced = true;
        root = insert(root, new node(key), 0, balanced);
        max_element_size = max(max_element_size, size());
        assert(balanced);
    }
    bool erase(const _Key& key){
        pair<node*, bool> res = erase(root, key);
        root = res.first, check();
        return res.second;
    }
};