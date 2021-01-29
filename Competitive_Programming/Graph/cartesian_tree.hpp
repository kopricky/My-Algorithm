#include "../header.hpp"

template<typename T> class CartesianTree {
public:
    struct node {
        node *left, *right;
        T value;
        int index;
        node(const T _value, const int _index)
             : left(nullptr), right(nullptr), value(_value), index(_index){}
    };
    void clear(node *cur){
        if(!cur) return;
        clear(cur->left), clear(cur->right);
        delete cur;
    }
    node *root;
    CartesianTree(const vector<T>& vec) : root(nullptr){
        stack<node*> st;
        node *cur, *prev;
        const int n = (int)vec.size();
        for(int i = 0; i < n; ++i){
            cur = new node(vec[i], i), prev = nullptr;
            while(!st.empty() && st.top()->value > cur->value){
                st.top()->right = prev, prev = st.top(), st.pop();
            }
            cur->left = prev, st.push(cur);
        }
        while(!st.empty()){
            st.top()->right = root, root = st.top(), st.pop();
        }
    }
    // ~CartesianTree(){ clear(root); }
    node *get_root(){ return root; }
};
