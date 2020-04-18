#include "../header.hpp"

template<typename _Key> class TreapNode {
public:
    const _Key key;
    const unsigned int priority;
    TreapNode *left, *right;
    TreapNode(const _Key& _key) : key(_key), priority(xor128()), left(nullptr), right(nullptr){}
    static unsigned int xor128(){
        static unsigned int x = 123456789, y = 362436069, z = 521288629, w = 86675123;
        unsigned int t = (x ^ (x << 11));
        x = y, y = z, z = w;
        return (w = (w ^ (w >> 19)) ^ (t ^ ( t >> 8)));
    }
};

// [-inf, key) [key, +inf)
template<typename _Key>
pair<TreapNode<_Key>*, TreapNode<_Key>*> split(TreapNode<_Key> *cur, const _Key& key){
    if(!cur) return {nullptr, nullptr};
    pair<TreapNode<_Key>*, TreapNode<_Key>*> res;
    if(key <= cur->key){
        res = split(cur->left, key), cur->left = res.second;
        return {res.first, cur};
    }else{
        res = split(cur->right, key), cur->right = res.first;
        return {cur, res.second};
    }
}

template<typename _Key>
bool find(TreapNode<_Key> *cur, const _Key& key){
    while(cur){
        if(cur->key == key) return true;
        cur = (key < cur->key ? cur->left : cur->right);
    }
    return false;
}

template<typename _Key>
TreapNode<_Key> *insert(TreapNode<_Key> *cur, TreapNode<_Key> *new_node){
    if(!cur) return new_node;
    if(cur->priority < new_node->priority){
        pair<TreapNode<_Key>*, TreapNode<_Key>*> res = split(cur, new_node->key);
        new_node->left = res.first, new_node->right = res.second;
        return new_node;
    }else if(new_node->key < cur->key){
        cur->left = insert(cur->left, new_node);
    }else{
        cur->right = insert(cur->right, new_node);
    }
    return cur;
}

template<typename _Key>
TreapNode<_Key> *join(TreapNode<_Key> *left, TreapNode<_Key> *right){
    if(!left || !right) return (left ? left : right);
    if(left->priority < right->priority){
        return right->left = join(left, right->left), right;
    }else{
        return left->right = join(left->right, right), left;
    }
}

template<typename _Key>
pair<TreapNode<_Key>*, bool> erase(TreapNode<_Key> *cur, const _Key& key){
    if(!cur) return make_pair(cur, false);
    if(cur->key == key){
        TreapNode<_Key> *res = join(cur->left, cur->right);
        delete cur;
        return make_pair(res, true);
    }else if(key < cur->key){
        pair<TreapNode<_Key>*, bool> res = erase(cur->left, key);
        cur->left = res.first;
        return make_pair(cur, res.second);
    }else{
        pair<TreapNode<_Key>*, bool> res = erase(cur->right, key);
        cur->right = res.first;
        return make_pair(cur, res.second);
    }
}