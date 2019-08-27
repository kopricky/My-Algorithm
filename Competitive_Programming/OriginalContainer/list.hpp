#include "../header.hpp"

template<class C> class ListIterator;

template<class C>
class List {
public:
    using iterator = ListIterator<C>;

private:
    struct block {
        block *prev, *next;
        C data;
        block() : prev(this), next(this){}
        block(const C& _data) : prev(this), next(this), data(_data){}
        block(C&& _data) : prev(this), next(this), data(move(_data)){}
        block(block *_prev, block *_next) : prev(_prev), next(_next){}
        block(const block&) = delete;
        block(block&& another) : data(move(another.data)){
            prev = another.prev, next = another.next;
            another.prev = another.next = nullptr;
        }
        block& operator=(const block&) = delete;
        block& operator=(block&& another){
            this->~block();
            data = move(another.data);
            prev = another.prev, next = another.next;
            another.prev = another.next = nullptr;
        }
    };
    int N;
    size_t sz;
    vector<block> container;
    friend ListIterator<C>;
    // cur を nx の前に挿入
    iterator insert(block* const nx, block* const cur){
        ++sz;
        cur->prev = nx->prev, cur->next = nx;
        nx->prev->next = cur, nx->prev = cur;
        return iterator(cur);
    }
    // cur を削除
    iterator erase(block* const cur){
        --sz;
        cur->prev->next = cur->next, cur->next->prev = cur->prev;
        return iterator(cur->next);
    }

public:
    iterator insert(const iterator nx, const iterator cur){ return insert(nx.block_ptr, cur.block_ptr); }
    iterator insert(const iterator nx, int cur_index){ return insert(nx.block_ptr, &container[cur_index]); }
    iterator insert(int next_index, int cur_index){ return iterator(&container[next_index], &container[cur_index]); }
    iterator erase(const iterator cur){ return erase(cur.block_ptr); }
    iterator erase(int cur_index){ return erase(&container[cur_index]); }

public:
    List() : N(0), sz(0){}
    List(int _N) : N(_N), sz(N), container(_N+1){}
    List(const vector<C>& _data) : List((int)_data.size()){ build(_data); }
    List(vector<C>&& _data) : List((int)_data.size()){ build(move(_data)); }
    List(const List&) = delete;
    List(List&& another) = default;
    List& operator=(const List&) = delete;
    List& operator=(List&&) = default;
    friend ostream& operator<< (ostream& os, List& ls){
        for(auto& val : ls) os << val << ' ';
        return os;
    }
    template<typename Data>
    void build(Data&& _data){
        vector<C> new_data = forward<Data>(_data);
        for(int i = 0; i <= N; i++){
            container[i].prev = &container[(i+N)%(N+1)];
            container[i].next = &container[(i+1)%(N+1)];
            if(i < N) container[i].data = move(new_data[i]);
        }
    }
    void build(){
        container.emplace_back();
        for(int i = 0; i <= N; i++){
            container[i].prev = &container[(i+N)%(N+1)];
            container[i].next = &container[(i+1)%(N+1)];
        }
    }
    void push_back(const C& _data){ ++N, ++sz, container.push_back(_data); }
    template<typename... Args>
    void emplace_back(Args&&... _data){
        ++N, ++sz, container.emplace_back(forward<Args>(_data)...);
    }
    C& operator[](size_t index){ return container[index].data; }
    const C& operator[](size_t index) const { return container[index].data; }
    size_t size() const { return sz; }
    bool empty() const { return (size() == 0); }
    C& front(){ return container[N].next->data; }
    C& back(){ return container[N].prev->data; }
    iterator begin(){ return iterator(container[N].next); }
    iterator end(){ return iterator(&container[N]); }
};

template<class C>
class ListIterator {
private:
    friend List<C>;
    typename List<C>::block *block_ptr;
    using iterator_category = bidirectional_iterator_tag;
    using value_type = C;
    using difference_type = ptrdiff_t;
    using pointer = C*;
    using reference = C&;

private:
    ListIterator(typename List<C>::block *ls) : block_ptr(ls){}

public:
    ListIterator(){}
    ListIterator(const ListIterator& itr) : block_ptr(itr.block_ptr){}
    ListIterator& operator=(const ListIterator& itr) & { return block_ptr = itr.block_ptr, *this; }
    ListIterator& operator=(ListIterator&& itr) & { return block_ptr = itr.block_ptr, *this; }
    reference operator*() const { return block_ptr->data; }
    pointer operator->() const { return &(block_ptr->data); }
    ListIterator& operator++(){ return block_ptr = block_ptr->next, *this; }
    ListIterator operator++(int){
        ListIterator res = *this;
        return res.block_ptr = block_ptr->next, res;
    }
    ListIterator& operator--(){ return block_ptr = block_ptr->prev, *this; }
    ListIterator operator--(int){
        ListIterator res = *this;
        return res.block_ptr = block_ptr->prev, res;
    };
    bool operator==(const ListIterator& itr) const { return !(*this != itr); };
    bool operator!=(const ListIterator& itr) const { return block_ptr != itr.block_ptr; }
};
