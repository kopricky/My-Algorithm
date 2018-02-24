//~番目は0_indexed
template <typename T> class RBST {
public:
    RBST() : root(nullptr) {}
    //k番目に値を挿入
    void insert(int k, T val){
        root = insert(root, k, new node(val));
    }
    //k番目の値の削除
    void erase(int k) {
        node *p;
        tie(root, p) = erase(root, k);
        p->left = p->right = nullptr;
        delete p;
    }
    //k番目の値の参照を返す
    T& operator[](int k){ return get(root,k); }
    void print(){
        int sz = size(root);
        rep(i,sz) cout << (*this)[i] << " ";
        cout << "\n";
    }
private:
    struct node{
        T val;
        node *left, *right;
        int st_size;   // 部分木のサイズ
        node(){}
        node(T v) : val(v), left(nullptr), right(nullptr), st_size(1){}
        ~node() { delete left; delete right; }
    };
    node *root;
    using pnn = pair<node*,node*>;
    int size(node* t) { return t ? t->st_size : 0; }
    node* update(node *t) {
        node* l = t->left; node* r = t->right;
        t->st_size = size(l) + size(r) + 1;
        return t;
    }
    unsigned rnd(){
        static unsigned x = 123456789, y = 362436069, z = 521288629, w = 86675123;
        unsigned t = (x^(x<<11));
        x = y,y = z,z = w;
        return (w = (w^(w>>19))^(t^(t>>8)));
    }
    node* merge(node* l, node* r){
        if (!l || !r) return (!l) ? r : l;
        if(rnd() % (size(l) + size(r)) < (unsigned)size(l)){
            l->right = merge(l->right, r);
            return update(l);
        }else{
            r->left = merge(l, r->left);
            return update(r);
        }
    }
    pnn split(node* t, int k){   //木のサイズが(k,n-k)となるように分割する
        if(!t) return pnn(nullptr, nullptr);
        if(k <= size(t->left)){
            pnn s = split(t->left, k);
            t->left = s.second;
            return pnn(s.first,update(t));
        }else{
            pnn s = split(t->right, k-size(t->left)-1);
            t->right = s.first;
            return pnn(update(t), s.second);
        }
    }
    T& get(node *t, int k){
        if(!t) assert(false);
        int s = size(t->left);
        if(s > k)       return get(t->left,k);
        else if(s < k)  return get(t->right,k-s-1);
        else            return t->val;
    }
    node* insert(node* t, int k, node* u){
        pnn s = split(t, k);
        return merge(merge(s.first, u), s.second);
    }
    pnn erase(node* t, int k){
        pnn sr = split(t, k+1);
        pnn sl = split(sr.first, k);
        return pnn(merge(sl.first, sr.second), sl.second);
    }
};
