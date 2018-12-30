#include "../header.hpp"

//最大独立集合問題に対するアルゴリズム

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

// 多重辺はなし
class MIS {
private:
    struct edge {
        int to, rev;
    };
    struct info {
        int from, to, next;
        info(int ag1, int ag2, int ag3) : 
            from(ag1), to(ag2), next(ag3){}
    };
    
    using LL = unsigned __int128;
    
    int V;
    vector<List<edge> > G;
    List<int> rem_ver;
    vector<int> cand, deg, is_rem;
    vector<vector<info> > edge_info;
    LL not_use_ver;
    
    int get_first_element(LL num){
        int x = __builtin_ffsll(num & (((LL)1 << 64) - 1));
        int y = __builtin_ffsll((num >> 64) & (((LL)1 << 64) - 1));
        return x ? (x - 1) : (y + 63);
    }
    bool finish(){
        return (int)cand.size() + (int)rem_ver.size() <= ans_size;
    }
    bool erase(int u, bool use, bool alr_use, int& add_size, vector<pair<int, int> >& erase_ver){
        erase_ver.emplace_back(u, rem_ver.erase(u)), is_rem[u] = 0;
        if(use && !alr_use){
            add_size++, cand.push_back(u);
        }else{
            if(finish()) return false;
        }
        for(auto& e : G[u]){
            edge_info[u].emplace_back(e.to, e.rev, G[e.to].erase(e.rev)), deg[e.to]--;
            if(deg[e.to] == 0) erase_ver.emplace_back(e.to, rem_ver.erase(e.to)), is_rem[e.to] = 0;
            if(use){
                if(deg[e.to] > 0) not_use_ver |= ((LL)1 << e.to);
            }else if((not_use_ver >> e.to & 1) == 0 && deg[e.to] == 0){
                add_size++, cand.push_back(e.to);
            }
        }
        return !finish();
    }
    bool erase_rec_set(bool flag, int& add_size, vector<pair<int, int> >& erase_ver){
        while(not_use_ver != 0){
            int v = get_first_element(not_use_ver);
            not_use_ver ^= ((LL)1 << v);
            if(deg[v] == 0) continue;
            if(!erase(v, flag, flag, add_size, erase_ver)) return false;
        }
        return true;
    }
    bool erase_rec(int u, bool use, int& add_size, vector<pair<int, int> >& erase_ver){
        if(!erase(u, use, false, add_size, erase_ver)){
            not_use_ver = 0;
            return false;
        }
        while(not_use_ver != 0){
            if(!erase_rec_set(false, add_size, erase_ver)){
                not_use_ver = 0;
                return false;
            }
        }
        return true;
    }
    bool preprocessing(int& add_size, vector<pair<int, int> >& erase_ver){
        for(int u : rem_ver){
            if(is_rem[u] == 1 && deg[u] <= 1){
                if(!erase_rec(u, true, add_size, erase_ver)) return false;
            }
        }
        return true;
    }
    void postprocessing(int& add_size, vector<pair<int, int> >& erase_ver){
        for(int i = 0; i < add_size; i++){
            cand.pop_back();
        }
        for(int i = (int)erase_ver.size() - 1; i >= 0; i--){
            auto& p = erase_ver[i];
            while(!edge_info[p.first].empty()){
                auto& dat = edge_info[p.first].back();
                G[dat.from].insert(dat.next, dat.to), deg[dat.from]++;
                edge_info[p.first].pop_back();
            }
            rem_ver.insert(p.second, p.first), is_rem[p.first] = 1;
        }
    }
    bool check(){
        if(rem_ver.empty()){
            if((int)cand.size() > ans_size){
                ans_size = (int)cand.size(), ans_set = cand;
            }
            return false;
        }
        return true;
    }
    void transition(int next_ver, bool use){
        int pl_add_size = 0;
        vector<pair<int, int> > pl_erase_ver;
        if(!erase_rec(next_ver, use, pl_add_size, pl_erase_ver)){
            return postprocessing(pl_add_size, pl_erase_ver);
        }
        if(check()) judge();
        return postprocessing(pl_add_size, pl_erase_ver);
    }
    int find_max_degree_ver(){
        int max_deg = -1, next_ver = -1;
        for(int v : rem_ver){
            if(deg[v] > max_deg) max_deg = deg[v], next_ver = v;
        }
        return next_ver;
    }
    void judge(){
        int add_size = 0;
        vector<pair<int, int> > erase_ver;
        if(!preprocessing(add_size, erase_ver)) return postprocessing(add_size, erase_ver);
        if(!check()) return postprocessing(add_size, erase_ver);
        int next_ver = find_max_degree_ver();
        transition(next_ver, true);
        if(deg[next_ver] > 1 && (int)cand.size() + (int)rem_ver.size() > ans_size + 1){
            transition(next_ver, false);
        }
        return postprocessing(add_size, erase_ver);
    }
public:
    vector<int> ans_set;
    int ans_size;
    
    MIS(int node_size) :
        V(node_size), G(V), rem_ver(V), deg(V, 0), is_rem(V, 1), edge_info(V), not_use_ver(0), ans_size(0){
            for(int i = 0; i < V; i++) rem_ver[i] = i;
    }
    void add_edge(int u, int v){
        G[u].push_back((edge){v, (int)G[v].size()});
        G[v].push_back((edge){u, (int)G[u].size() - 1});
        deg[u]++, deg[v]++;
    }
    int solve(){
        judge();
        return ans_size;
    }
};