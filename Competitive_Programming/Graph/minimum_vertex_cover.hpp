#include "../header.hpp"

// 最小頂点被覆問題に対する FPT アルゴリズム(頂点数が 127 以下の場合)
// 多重辺が存在しないことを仮定している

template<typename T> class ListIterator;

template<typename T>
class List {
public:
    using iterator = ListIterator<T>;

private:
    struct block {
        block *prev, *next;
        T data;
        block() : prev(this), next(this){}
        block(T _data) : prev(this), next(this), data(_data){}
        block(block *_prev, block *_next) : prev(_prev), next(_next){}
    };
    int N;
    size_t sz;
    vector<block> container;
    friend ListIterator<T>;
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
    iterator insert(const iterator nx, const iterator cur){ return insert(nx.ls_ptr, cur.ls_ptr); }
    iterator insert(const iterator nx, int cur_index){ return insert(nx.ls_ptr, &container[cur_index]); }
    iterator insert(int next_index, int cur_index){ return iterator(&container[next_index], &container[cur_index]); }
    iterator erase(const iterator cur){ return erase(cur.ls_ptr); }
    iterator erase(int cur_index){ return erase(&container[cur_index]); }

public:
    List() : N(0), sz(0){}
    List(int _N) : N(_N), sz(N), container(_N+1){}
    List(const vector<T>& _data) : List((int)_data.size()){ build(_data); }
    List(const List& ls) : N(ls.N), sz(ls.sz), container(ls.container){}
    void build(const vector<T>& _data){
        for(int i = 0; i <= N; i++){
            container[i].prev = &container[(i+N)%(N+1)];
            container[i].next = &container[(i+1)%(N+1)];
            if(i < N) container[i].data = _data[i];
        }
    }
    void push_back(const T _data){ ++N, ++sz, container.push_back(_data); }
    void build(){
        container.push_back(T());
        for(int i = 0; i <= N; i++){
            container[i].prev = &container[(i+N)%(N+1)];
            container[i].next = &container[(i+1)%(N+1)];
        }
    }
    friend ostream& operator<< (ostream& os, List& ls){
        for(auto& val : ls) os << val << ' ';
        return os;
    }
    const T& operator[](size_t index) const { return container[index].data; }
    T& operator[](size_t index){ return container[index].data; }
    size_t size() const { return sz; }
    bool empty() const { return (size() == 0); }
    T& front(){ return container[N].next->data; }
    T& back(){ return container[N].prev->data; }
    iterator begin(){ return iterator(container[N].next); }
    iterator end(){ return iterator(&container[N]); }
};

template<typename T>
class ListIterator {
private:
    friend List<T>;
    typename List<T>::block *ls_ptr;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

private:
    ListIterator(typename List<T>::block *ls) : ls_ptr(ls){}

public:
    ListIterator(){}
    ListIterator(const ListIterator& itr) : ls_ptr(itr.ls_ptr){}
    ListIterator& operator=(const ListIterator& itr) & { return ls_ptr = itr.ls_ptr, *this; }
    ListIterator& operator=(ListIterator&& itr) & { return ls_ptr = itr.ls_ptr, *this; }
    reference operator*() const { return ls_ptr->data; }
    pointer operator->() const { return &(ls_ptr->data); }
    ListIterator& operator++(){ return ls_ptr = ls_ptr->next, *this; }
    ListIterator operator++(int){
        ListIterator res = *this;
        return res.ls_ptr = ls_ptr->next, res;
    }
    ListIterator& operator--(){ return ls_ptr = ls_ptr->prev, *this; }
    ListIterator operator--(int){
        ListIterator res = *this;
        return res.ls_ptr = ls_ptr->prev, res;
    };
    bool operator==(const ListIterator& itr) const { return !(*this != itr); };
    bool operator!=(const ListIterator& itr) const { return ls_ptr != itr.ls_ptr; }
};

// 多重辺はなし
class MVC {
private:
    struct edge {
        int to, rev;
    };
    struct info {
        // from から to 番目に出ている辺が(を) next の前から消えた(に入れる)
        int from, to;
        List<edge>::iterator next;
        info(int ag1, int ag2, List<edge>::iterator ag3) :
            from(ag1), to(ag2), next(ag3){}
    };

    using LL = unsigned __int128;

    int V, rem_edge_size, fp_size;
    vector<List<edge> > G;
    List<int> rem_ver;
    vector<int> cand;
    vector<vector<info> > edge_info;
    LL small_deg_ver, use_ver;

    int get_first_element(LL num){
        int x = __builtin_ffsll(num & 0xffffffffffffffff);
        int y = __builtin_ffsll((num >> 64) & 0xffffffffffffffff);
        return x ? (x - 1) : (y + 63);
    }
    bool push_cand(int u, int& add_size){
        add_size++, cand.push_back(u);
        return (int)cand.size() >= ans_size;
    }
    bool erase(int u, bool use, bool alr_use, int& add_size, vector<pair<int, List<int>::iterator> >& erase_ver){
        if(use && !alr_use){
            if(push_cand(u, add_size)) return true;
        }
        erase_ver.emplace_back(u, rem_ver.erase(u));
        for(auto& e : G[u]){
            edge_info[u].emplace_back(e.to, e.rev, G[e.to].erase(e.rev)), rem_edge_size--;
            if(G[e.to].empty()) erase_ver.emplace_back(e.to, rem_ver.erase(e.to));
            if(!use){
                if(use_ver >> e.to & 1) continue;
                if(push_cand(e.to, add_size)) return true;
                if(!G[e.to].empty()) use_ver |= (LL)1 << e.to;
            }else{
                if((int)G[e.to].size() == 1 && !(use_ver >> e.to & 1)) small_deg_ver |= (LL)1 << e.to;
            }
        }
        return false;
    }
    bool erase_rec_set(LL& st, bool flag, int& add_size, vector<pair<int, List<int>::iterator> >& erase_ver){
        while(st != 0){
            int v = get_first_element(st);
            st ^= (LL)1 << v;
            if(G[v].empty()) continue;
            if(erase(v, flag, flag, add_size, erase_ver)) return true;
        }
        return false;
    }
    bool reset(){
        small_deg_ver = use_ver = 0;
        return true;
    }
    bool erase_rec(int u, bool use, int& add_size, vector<pair<int, List<int>::iterator> >& erase_ver){
        if(erase(u, use, false, add_size, erase_ver)) return reset();
        while(small_deg_ver != 0 || use_ver != 0){
            if(erase_rec_set(small_deg_ver, false, add_size, erase_ver)) return reset();
            if(erase_rec_set(use_ver, true, add_size, erase_ver)) return reset();
        }
        return false;
    }
    bool preprocessing(int K, int& add_size, vector<pair<int, List<int>::iterator> >& erase_ver){
        for(int u : rem_ver){
            if((int)G[u].size() > K){
                if(!erase_rec(u, true, add_size, erase_ver)) return true;
            }
        }
        return false;
    }
    void postprocessing(int K, int add_size, vector<pair<int, List<int>::iterator> >& erase_ver){
        if(K == fp_size) return;
        for(int i = 0; i < add_size; i++){
            cand.pop_back();
        }
        for(int i = (int)erase_ver.size() - 1; i >= 0; i--){
            auto& p = erase_ver[i];
            while(!edge_info[p.first].empty()){
                auto& dat = edge_info[p.first].back();
                G[dat.from].insert(dat.next, dat.to), rem_edge_size++;
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
            return true;
        }
        return rem_ver.empty();
    }
    void transition(int K, int next_ver, bool use){
        int pl_add_size = 0;
        vector<pair<int, List<int>::iterator> > pl_erase_ver;
        if(erase_rec(next_ver, use, pl_add_size, pl_erase_ver)){
            return postprocessing(K-1, pl_add_size, pl_erase_ver);
        }
        if(!check()) judge(K-1);
        return postprocessing(K-1, pl_add_size, pl_erase_ver);
    }
    int find_max_degree_ver(){
        int max_deg = -1, next_ver = -1;
        for(int v : rem_ver){
            if((int)G[v].size() > max_deg) max_deg = (int)G[v].size(), next_ver = v;
        }
        if(2 * rem_edge_size > max_deg * (int)rem_ver.size()) return -1;
        return next_ver;
    }
    void judge(int K){
        int add_size = 0;
        vector<pair<int, List<int>::iterator> > erase_ver;
        if(preprocessing(K, add_size, erase_ver)) return postprocessing(K, add_size, erase_ver);
        if(check()) return postprocessing(K, add_size, erase_ver);
        int next_ver = find_max_degree_ver();
        if(next_ver < 0) return postprocessing(K, add_size, erase_ver);
        // 使う場合
        if((int)G[next_ver].size() > 1) transition(K, next_ver, true);
        // 使わない場合
        if((int)cand.size() + (int)G[next_ver].size() < ans_size) transition(K, next_ver, false);
        return postprocessing(K, add_size, erase_ver);
    }
public:
    vector<int> ans_set;
    int ans_size;

    MVC(int node_size) :
        V(node_size), rem_edge_size(0), G(V), rem_ver(V), edge_info(V), small_deg_ver(0), use_ver(0){
        vector<int> _init(V);
        iota(_init.begin(), _init.end(), 0);
        rem_ver.build(_init);
    }
    void add_edge(int u, int v){
        G[u].push_back((edge){v, (int)G[v].size()});
        G[v].push_back((edge){u, (int)G[u].size() - 1});
        rem_edge_size++;
    }
    bool solve(){
        for(int i = 0; i < V; ++i) G[i].build();
        fp_size = V, ans_size = fp_size + 1;
        judge(fp_size);
        return ans_size <= fp_size;
    }
};
