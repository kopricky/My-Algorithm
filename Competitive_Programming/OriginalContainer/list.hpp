#include "../header.hpp"

template<typename T> class ListIterator;

// 常に不変なイテレーターが欲しい場合に使う
template<typename T>
class List {
public:
    using iterator = ListIterator<T>;

private:
    struct block {
        int prev, next;
        T value;
        block(){}
        block(T value_) : value(value_){}
        block(int prev_, int next_) : prev(prev_), next(next_){}
    };
    int N, sz;
    vector<block> container;
    friend ListIterator<T>;

public:
    // 添字 next_index の要素の前に添字 now_index の要素を挿入
    iterator insert(int next_index, int now_index){
        sz++;
        auto& nw = container[now_index]; auto& nx = container[next_index];
        nw.prev = nx.prev, nw.next = next_index;
        container[nx.prev].next = now_index, nx.prev = now_index;
        return iterator(this, now_index);
    }
    // 添字 index の値を削除 → 次の要素の iterator を返す
    iterator erase(int index){
        sz--;
        auto& cur = container[index];
        container[cur.prev].next = cur.next, container[cur.next].prev = cur.prev;
        return iterator(this, cur.next);
    }

public:
    friend ostream& operator<< (ostream& os, List& ls){
        for(auto& val : ls){
            os << val << ' ';
        }
        return os;
    }
    const T& operator[](size_t index) const {
        return container[index].value;
    }
    T& operator[](size_t index){
        return container[index].value;
    }
    size_t size(){
        return sz;
    }
    bool empty(){
        return size() == 0;
    }
    T& front(){
        return container[container[N].next].value;
    }
    T& back(){
        return container[container[N].prev].value;
    }
    iterator begin(){
        return iterator(this, container[N].next);
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
        return insert(N, container[N].prev+1);
    }
    void pop_back(){
        erase(container[N].prev);
    }
    void index_clear(){
        N = sz = 0, container = {(block){0, 0}};
    }
    void clear(){
        index_clear(); container.clear();
    }

public:
    List() : N(0), sz(0), container({(block){0, 0}}){}
    List(int _N) : container(_N + 1){
        build();
    }
    List(const List& ls):
        N(ls.N), sz(ls.sz), container(ls.container){}
    List& operator=(const List& ls){
        container = ls.container;
        build();
        return *this;
    }
    List& operator=(const List&& ls) noexcept {
        container = ls.container;
        build();
        return *this;
    }
    iterator push_back(T val){
        N++, sz++;
        container[0].prev = container.back().next = N, container.back().value = val;
        container.emplace_back(N-1, 0);
        return iterator(this, N-1);
    }
	void build(){
        N = sz = (int)container.size() - 1;
        for(int i = 0; i <= N; i++) container[i].prev = i-1, container[i].next = i+1;
        container[0].prev = N, container[N].next = 0;
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
    explicit operator int() const noexcept { return index; }
    ListIterator& operator=(const ListIterator& itr) & {
        ls_ptr = itr.ls_ptr, index = itr.index;
        return *this;
    }
    ListIterator& operator=(ListIterator&& itr) & noexcept {
        ls_ptr = itr.ls_ptr, index = itr.index;
        return *this;
    }
    T& operator*() const {
        return ls_ptr->container[index].value;
    }
    T* operator->() const {
        return &ls_ptr->container[index].value;
    }
    ListIterator& operator++(){
        index = ls_ptr->container[index].next;
        return *this;
    }
    ListIterator operator++(int){
        ListIterator res = *this;
        index = ls_ptr->container[index].next;
        return res;
    }
    ListIterator& operator--(){
        index = ls_ptr->container[index].prev;
        return *this;
    };
    ListIterator operator--(int){
        ListIterator res = *this;
        index = ls_ptr->container[index].prev;
        return res;
    };
    bool operator==(const ListIterator& itr) const {
        return !(*this != itr);
    };
    bool operator!=(const ListIterator& itr) const {
        return this->ls_ptr != itr.ls_ptr || this->index != itr.index;
    }
};
