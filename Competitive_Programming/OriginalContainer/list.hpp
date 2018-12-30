#include "../header.hpp"

template<typename T> class ListIterator;

// 常に不変なイテレーターが欲しい場合に使う
template<typename T>
class List {
public:
    using iterator = ListIterator<T>;
    
private:
    int N, sz;
    vector<int> prev, next;
    vector<T> container;
    friend ListIterator<T>;
    
public:
    // 添字 next_index の要素の前に添字 now_index の要素を挿入
    iterator insert(int next_index, int now_index){
        sz++;
        prev[now_index] = prev[next_index], next[now_index] = next_index;
        next[prev[next_index]] = now_index, prev[next_index] = now_index;
        return iterator(this, now_index);
    }
    // 添字 index の値を削除 → 次の要素の iterator を返す
    iterator erase(int index){
        sz--;
        next[prev[index]] = next[index], prev[next[index]] = prev[index];
        return iterator(this, next[index]);
    }
	
public:
    friend ostream& operator<< (ostream& os, List& ls){
        for(auto& val : ls){
            os << val << ' ';
        }
        return os;
    }
    const T& operator[](size_t index) const {
        return container[index];
    }
    T& operator[](size_t index){
        return container[index];
    }
    size_t size(){
        return sz;
    }
    bool empty(){
        return size() == 0;
    }
    T& front(){
        return container[next[N]];
    }
    T& back(){
        return container[prev[N]];
    }
    iterator begin(){
        return iterator(this, next[N]);
    }
    iterator end(){
        return iterator(this, N);
    }
    iterator insert(const iterator& itr, int index){
        return insert(itr.index, index);
    }
    iterator erase(const iterator& itr){
        return erase(itr.index);
    }
    iterator push_back(){
        return insert(N, prev[N]+1);
    }
    void pop_back(){
        erase(prev[N]);
    }
    void index_clear(){
        N = sz = 0, prev = next = {0};
    }
    void clear(){
        index_clear();
        container.clear();
    }
	
public:
    List() : N(0), sz(0), prev({0}), next({0}){}
    List(int _N) : container(_N){
        build();
    }
    List(int _N, T val) : container(_N, val){
        build();
    }
    List(const List& ls):
        N(ls.N), sz(ls.sz), prev(ls.prev), next(ls.next), container(ls.container){}
    List(const vector<T>& vec) : container(vec){
        build();
    }
    List& operator=(const List& ls){
        container = ls.container;
        build();
        return *this;
    }
    List& operator=(const vector<T>& vec){
        container = vec;
        build();
        return *this;
    }
    List& operator=(const List&& ls) noexcept {
        container = ls.container;
        build();
        return *this;
    }
    List(initializer_list<T> vals) : container(vals.begin(), vals.end()){
        build();
    }
    iterator push_back(T val){
        N++, sz++;
        prev.push_back(N-1), prev[0] = N;
        next.back() = N, next.push_back(0);
        container.push_back(val);
        return iterator(this, N-1);
    }
	void build(){
        N = sz = (int)container.size();
        prev.resize(N+1), next.resize(N+1);
        iota(prev.begin(), prev.end(), -1), iota(next.begin(), next.end(), 1);
        prev[0] = N, next[N] = 0;
    }
};

template<typename T>
class ListIterator : public std::iterator<bidirectional_iterator_tag, T, ptrdiff_t, T*, T&>{
private:
    friend List<T>;
    List<T>* ls_ptr;
    int index;

private:
    ListIterator(List<T>* ls, int _index) : ls_ptr(ls), index(_index){}
	
public:
    ListIterator(){}
    ListIterator(const ListIterator& itr){
        ls_ptr = itr.ls_ptr, index = itr.index;
    }
    operator int() const noexcept { return index; }
    ListIterator& operator=(const ListIterator& itr) & {
        ls_ptr = itr.ls_ptr, index = itr.index;
        return *this;
    }
    ListIterator& operator=(ListIterator&& itr) & noexcept {
        ls_ptr = itr.ls_ptr, index = itr.index;
        return *this;
    }
    T& operator*() const {
        return ls_ptr->container[index];
    }
    T* operator->() const {
        return &ls_ptr->container[index];
    }
    ListIterator& operator++(){
        index = ls_ptr->next[index];
        return *this;
    }
    ListIterator operator++(int){
        ListIterator res = *this;
        index = ls_ptr->next[index];
        return res;
    }
    ListIterator& operator--(){
        index = ls_ptr->prev[index];
        return *this;
    };
    ListIterator operator--(int){
        ListIterator res = *this;
        index = ls_ptr->prev[index];
        return res;
    };
    bool operator==(const ListIterator& itr) const {
        return !(*this != itr);
    };
    bool operator!=(const ListIterator& itr) const {
        return this->ls_ptr != itr.ls_ptr || this->index != itr.index;
    }
    friend void swap(const ListIterator<T>& itr1, const ListIterator<T>& itr2){
        ListIterator<T> tmp = itr1;
        itr1 = itr2, itr2 = itr1;
    };
};