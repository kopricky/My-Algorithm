template <typename T> class RBST
{
public:
    struct node{
        T val;
        int st_size;   // 部分木のサイズ
        node* left; node* right;
        using pnn = pair<node*,node*>;
        node(T v) : val(v), left(nullptr), right(nullptr), st_size(1){}
        ~node() { delete left; delete right; }
        static int size(node* t) { return t ? t->st_size : 0; }
        static node* update(node *t) {
            node* l = t->left; node* r = t->right;
            t->st_size = size(l) + size(r) + 1;
            return t;
        }
        static unsigned rnd(){
            static unsigned x = 123456789, y = 362436069, z = 521288629, w = 86675123;
            unsigned t = (x^(x<<11));
            x = y,y = z,z = w;
            return (w = (w^(w>>19))^(t^(t>>8)));
        }
        static node* merge(node* l, node* r){
            if (!l || !r) return (!l) ? r : l;
            update(l); update(r);
            if(rnd() % (size(l) + size(r)) < (unsigned)size(l)){
                l->right = merge(l->right, r);
                return update(l);
            }else{
                r->left = merge(l, r->left);
                return update(r);
            }
        }
        static pnn split(node* t, int k){   //木のサイズが(k,n-k)となるように分割する
            if(!t) return pnn(nullptr, nullptr);
            update(t);
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
        static node* insert(node* t, int k, node* u){
            pnn s = split(t, k);
            return merge(merge(s.first, u), s.second);
        }
        static pnn erase(node* t, int k){
            pnn sr = split(t, k+1);
            pnn sl = split(sr.first, k);
            return pnn(merge(sl.first, sr.second), sl.second);
        }
    };
    node *root;
    RBST() : root(nullptr) {}
    ~RBST() {}
    void insert(int k, T val){
        root = node::insert(root, k, new node(val));
    }
    void erase(int k) {
        node *p;
        tie(root, p) = node::erase(root, k);
        p->left = p->right = nullptr;
        delete p;
    }
    T get(int k){
        node* v = root;
        while(v) {
            int s = node::size(v->left);
            if(s > k){
                 v = v->left;
            }else if(s == k){
                return v->val;
            }else{
                v = v->right;
                k -= s+1;
            }
        }
        return v->val;
    }
    //cri以下の要素数(値が昇順に並んでいる場合のみ使用可)
    int index(T cri){
        int res = 0;
        node* v = root;
        while(v){
            if(cri < v->val){
                v = v->left;
            }else{
                res += node::size(v->left) + 1;
                v = v->right;
            }
        }
        return res;
    }
    //k番目の値をvalに変更
    void set(int k, T val) {
        auto sr = node::split(root, k+1);
        auto sl = node::split(sr.first, k);
        auto lr = sl.second;
        lr->val = val;
        root = node::merge(node::merge(sl.first, lr), sr.second);
    }
};
