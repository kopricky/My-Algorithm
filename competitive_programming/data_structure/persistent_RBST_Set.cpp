//verifyしてません
const int POOL_SIZE = 20000000;
//~番目は0_indexed
template <typename T> class RBST {
public:
    RBST() : root(nullptr){}
    //k以上の数の最小インデックス
    int lower_bound(const T k){ return lower_bound(root,k); }
    //k以上の最小の数(存在しなければINFを返す)
    T lower_value(const T k){
        T res = numeric_limits<T>::max();
        lower_value(root,k,res);
        return res;
    }
    //kを超える数の最大インデックス
    int upper_bound(const T k){ return upper_bound(root,k); }
    //kを超える最小の数(存在しなければINFを返す)
    T upper_value(const T k){
        T res = numeric_limits<T>::max();
        upper_value(root, k, res);
        return res;
    }
    //値valを挿入
    void insert(T val){
        root = insert(root, upper_bound(val), new node(val));
    }
    //値valを削除(実装上はlower_bound(val)を削除している)
    void erase(T val){
        root = erase(root, lower_bound(val));
    }
    //k番目の値を返す
    T get(int k){ return get(root,k); }
    void print(){
        int sz = size(root);
        rep(i,sz) cout << get(i) << " ";
        cout << "\n";
    }
private:
    struct node{
        T val;
        node *left, *right;
        int st_size;   // 部分木のサイズ
        node(){}
        node(T v) : val(v), left(nullptr), right(nullptr), st_size(1){}
        void Set(T v, node* l, node* r){
            val = v, left = l, right = r, st_size = 1;
        }
    };
    using pnn = pair<node*,node*>;
    node* fix(){
        assert(nw_size < POOL_SIZE);
        return &pool[nw_size++];
    }
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
        node* x = fix();
        if(rnd() % (size(l) + size(r)) < (unsigned)size(l)){
            x->Set(l->val,l->left,merge(l->right, r));
        }else{
            x->Set(r->val,merge(l,r->left),r->right);
        }
        return update(x);
    }
    pnn split(node* t, int k){   //木のサイズが(k,n-k)となるように分割する
        if(!t) return pnn(nullptr, nullptr);
        if(k <= size(t->left)){
            pnn s = split(t->left, k);
            node* x = fix();
            x->Set(t->val,s.second,t->right);
            return pnn(s.first,update(x));
        }else{
            pnn s = split(t->right, k-size(t->left)-1);
            node* x = fix();
            x->Set(t->val,t->left,s.first);
            return pnn(update(x), s.second);
        }
    }
    int lower_bound(node *t, const T k){
        if(!t) return 0;
        if(t->val < k){
            return size(t->left) + lower_bound(t->right,k) + 1;
        }else{
            return lower_bound(t->left,k);
        }
    }
    void lower_value(node *t, const T k, T& res){
        if(!t) return;
        if(t->val < k){
            lower_value(t->right,k,res);
        }else{
            res = t->val;
            lower_value(t->left,k,res);
        }
    }
    int upper_bound(node *t, const T k){
        if(!t) return 0;
        if(t->val <= k){
            return size(t->left) + upper_bound(t->right,k) + 1;
        }else{
            return upper_bound(t->left,k);
        }
    }
    void upper_value(node *t, const T k, T& res){
        if(!t) return;
        if(t->val <= k){
            upper_value(t->right,k,res);
        }else{
            res = t->val;
            upper_value(t->left,k,res);
        }
    }
    T get(node *t, int k){
        if(!t) assert(false);
        int s = size(t->left);
        if(s > k)       return get(t->left,k);
        else if(s < k)  return get(t->right,k-s-1);
        else            return t->val;
    }
    node* insert(node* t, int k, node* u){
        pnn s = split(t, k);
        node* x = fix();
        x->Set(x->val,nullptr,nullptr);
        return merge(merge(s.first, u), s.second);
    }
    node* erase(node* t, int k){
        pnn sr = split(t, k+1);
        pnn sl = split(sr.first, k);
        return merge(sl.first, sr.second);
    }

    node* root;
    static int nw_size;
    static node pool[POOL_SIZE];
};
template<typename T> int RBST<T>::nw_size = 0;
template<typename T> class RBST<T>::node RBST<T>::pool[POOL_SIZE];
