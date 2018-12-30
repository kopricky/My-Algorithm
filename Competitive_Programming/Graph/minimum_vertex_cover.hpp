#include "../header.hpp"

//最小頂点被覆問題に対する FPT アルゴリズム

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
class MVC {
private:
    struct edge {
        int to, rev;
    };
    struct info {
        // from から to 番目に出ている辺が(を) next の前から消えた(に入れる)
        int from, to, next;
        info(int ag1, int ag2, int ag3) : 
            from(ag1), to(ag2), next(ag3){}
    };
    int V, rem_edge_size, fp_size;
    vector<List<edge> > G;
    List<int> rem_ver;
    vector<int> cand, deg;
    vector<vector<info> > edge_info;
    set<int> small_deg_ver, use_ver;
    
    bool push_cand(int u, int& add_size){
        add_size++, cand.push_back(u);
        return (int)cand.size() < ans_size;
    }
    
    bool erase(int u, bool use, bool alr_use, int& add_size, vector<pair<int, int> >& erase_ver){
        if(use && !alr_use){
            if(!push_cand(u, add_size)) return false;
        }
        erase_ver.emplace_back(u, rem_ver.erase(u));
        for(auto& e : G[u]){
            edge_info[u].emplace_back(e.to, e.rev, G[e.to].erase(e.rev));
            deg[e.to]--, rem_edge_size--;
            if(deg[e.to] == 0) erase_ver.emplace_back(e.to, rem_ver.erase(e.to));
            if(!use){
                if(use_ver.count(e.to) == 1) continue;
                if(!push_cand(e.to, add_size)) return false;
                if(deg[e.to] > 0) use_ver.insert(e.to);
            }else{
                if(deg[e.to] == 1 && use_ver.count(e.to) == 0) small_deg_ver.insert(e.to);
            }
        }
        return true;
    }
    bool erase_rec_set(set<int>& st, bool flag, int& add_size, vector<pair<int, int> >& erase_ver){
        while(!st.empty()){
            int v = *st.begin();
            st.erase(v);
            if(deg[v] == 0) continue;
            if(!erase(v, flag, flag, add_size, erase_ver)) return false;
        }
        return true;
    }
    bool reset(){
        small_deg_ver.clear(), use_ver.clear();
        return false;
    }
    bool erase_rec(int u, bool use, int& add_size, vector<pair<int, int> >& erase_ver){
        if(!erase(u, use, false, add_size, erase_ver)) return reset();
        while(!small_deg_ver.empty() || !use_ver.empty()){
            if(!erase_rec_set(small_deg_ver, false, add_size, erase_ver)) return reset();
            if(!erase_rec_set(use_ver, true, add_size, erase_ver)) return reset();
        }
        return true;
    }
    bool preprocessing(int K, int& add_size, vector<pair<int, int> >& erase_ver){
        for(int u : rem_ver){
            if(deg[u] > K){
                if(!erase_rec(u, true, add_size, erase_ver)) return false;
            }
        }
        return true;
    }
    void postprocessing(int K, int add_size, vector<pair<int, int> >& erase_ver){
        if(K == fp_size) return;
        for(int i = 0; i < add_size; i++){
            cand.pop_back();
        }
        for(int i = (int)erase_ver.size() - 1; i >= 0; i--){
            auto& p = erase_ver[i];
            while(!edge_info[p.first].empty()){
                auto& dat = edge_info[p.first].back();
                G[dat.from].insert(dat.next, dat.to), deg[dat.from]++, rem_edge_size++;
                edge_info[p.first].pop_back();
            }
            rem_ver.insert(p.second, p.first);
        }
    }
    bool check(){
        if(rem_edge_size == 0){
            if((int)cand.size() < ans_size){
                ans_size = (int)cand.size(), ans_set = cand;
            }
            return false;
        }
        return (int)rem_ver.size() > 0;
    }
    void transition(int K, int next_ver, bool use){
        int pl_add_size = 0;
        vector<pair<int, int> > pl_erase_ver;
        if(!erase_rec(next_ver, use, pl_add_size, pl_erase_ver)){
            return postprocessing(K-1, pl_add_size, pl_erase_ver);
        }
        if(check()) judge(K-1);
        return postprocessing(K-1, pl_add_size, pl_erase_ver);
    }
    int find_max_degree_ver(){
        int max_deg = -1, next_ver = -1;
        for(int v : rem_ver){
            if(deg[v] > max_deg) max_deg = deg[v], next_ver = v;
        }
        if(2 * rem_edge_size > (long long)max_deg * (int)rem_ver.size()) return -1;
        return next_ver;
    }
    void judge(int K){
        int add_size = 0;
        vector<pair<int, int> > erase_ver;
        if(!preprocessing(K, add_size, erase_ver)) return postprocessing(K, add_size, erase_ver);
        if(!check()) return postprocessing(K, add_size, erase_ver);
        int next_ver = find_max_degree_ver();
        if(next_ver < 0) return postprocessing(K, add_size, erase_ver);
        transition(K, next_ver, true);
        if((int)cand.size() + (int)G[next_ver].size() >= ans_size){
            transition(K, next_ver, false);
        }
        return postprocessing(K, add_size, erase_ver);
    }
public:
    vector<int> ans_set;
    int ans_size;
    
    MVC(int node_size) :
        V(node_size), rem_edge_size(0), G(V), rem_ver(V), deg(V, 0), edge_info(V){
            for(int i = 0; i < V; i++) rem_ver[i] = i;
    }
    void add_edge(int u, int v){
        G[u].push_back((edge){v, (int)G[v].size()});
        G[v].push_back((edge){u, (int)G[u].size() - 1});
        deg[u]++, deg[v]++, rem_edge_size++;
    }
    bool solve(int K){
        fp_size = K, ans_size = fp_size + 1;
        judge(fp_size);
        return ans_size <= fp_size;
    }
};