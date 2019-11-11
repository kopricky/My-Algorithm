#include "./header.hpp"

template<typename _Key, class _Compare=less<_Key> > class BaseHeap {
private:
    multiset<_Key, _Compare> heap;

public:
    BaseHeap(){}
    bool empty() const {
        return heap.empty();
    }
    void insert(const _Key& key){
        heap.insert(key);
    }
    void erase(const _Key& key){
        heap.erase(heap.find(key));
    }
    const _Key& find_min() const {
        return *heap.begin();
    }
};

template<typename _Key, class _Compare=less<_Key> > class MeldableHeapNode {
private:
    BaseHeap<_Key, _Compare> bheap;
    MeldableHeapNode *par;
    unsigned int rank;

public:
    MeldableHeapNode(const _Key& key) : par(this), rank(0u){
        bheap.insert(key);
    }
    bool empty() const {
        return bheap.empty();
    }
    MeldableHeapNode*& get_parent(){
        return par;
    }
    MeldableHeapNode*& get_grandparent(){
        return par->get_parent();
    }
    unsigned int& get_rank(){
        return rank;
    }
    bool is_root() const {
        return this == par;
    }
    void insert(const _Key& key){
        bheap.insert(key);
    }
    void erase(const _Key& key){
        bheap.erase(key);
    }
    const _Key& find_min() const {
        return bheap.find_min();
    }
};

template<typename _Key, class _Compare=less<_Key> > class MeldableHeaps {
private:
    stack<MeldableHeapNode<_Key, _Compare>*> free_pointer;
    void cut_path(MeldableHeapNode<_Key, _Compare> *cur_heap){
        if(!cur_heap->is_root()){
            cut_path(cur_heap->get_parent());
            unhang(cur_heap, cur_heap->get_parent());
        }
    }
    void compress_path(MeldableHeapNode<_Key, _Compare> *cur_heap){
        if(!cur_heap->is_root()){
            compress_path(cur_heap->get_parent());
            hang(cur_heap, cur_heap->get_grandparent());
        }
    }
    void hang(MeldableHeapNode<_Key, _Compare> *heap1,
                                            MeldableHeapNode<_Key, _Compare> *heap2){
        if(!heap1->empty()) heap2->insert(heap1->find_min());
        heap1->get_parent() = heap2;
    }
    void unhang(MeldableHeapNode<_Key, _Compare> *heap1,
                                            MeldableHeapNode<_Key, _Compare> *heap2){
        heap2->erase(heap1->find_min());
    }

public:
    MeldableHeaps(){}
    ~MeldableHeaps(){
        while(!free_pointer.empty()){
            MeldableHeapNode<_Key, _Compare> *heap = free_pointer.top();
            free_pointer.pop();
            delete heap;
        }
    }
    MeldableHeapNode<_Key, _Compare> *make_heap(const _Key& key){
        free_pointer.push(new MeldableHeapNode<_Key, _Compare>(key));
        return free_pointer.top();
    }
    MeldableHeapNode<_Key, _Compare> *insert(MeldableHeapNode<_Key, _Compare> *heap,
                                                const _Key& key){
        assert(heap->is_root());
        MeldableHeapNode<_Key, _Compare> *return_heap = make_heap(key);
        meld(heap, return_heap);
        return return_heap;
    }
    void erase(MeldableHeapNode<_Key, _Compare> *key_heap, const _Key& key){
        cut_path(key_heap);
        key_heap->erase(key);
        compress_path(key_heap);
    }
    const _Key& find_min(MeldableHeapNode<_Key, _Compare> *heap){
        assert(heap->is_root() && !heap->empty());
        return heap->find_min();
    }
    MeldableHeapNode<_Key, _Compare> *meld(MeldableHeapNode<_Key, _Compare> *heap1,
                                            MeldableHeapNode<_Key, _Compare> *heap2){
        assert(heap1->is_root() && heap2->is_root());
        if(heap1->get_rank() < heap2->get_rank()) swap(heap1, heap2);
        hang(heap2, heap1);
        if(heap1->get_rank() == heap2->get_rank()) ++heap1->get_rank();
        return heap1;
    }
    MeldableHeapNode<_Key, _Compare> *find_root(MeldableHeapNode<_Key, _Compare> *heap){
        cut_path(heap);
        compress_path(heap);
    }
};
