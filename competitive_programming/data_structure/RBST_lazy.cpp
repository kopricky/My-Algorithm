//RAQ,RMQの場合
template <typename T> class RBST {
public:
    struct node{
        T val, lazy, al;
        int st_size;
        node* left; node* right;
        bool lazy_flag;
        node(){};
        node(T v) : val(v), al(v), left(nullptr), right(nullptr), st_size(1), lazy(0), lazy_flag(false){}
        ~node() { delete left; delete right; }
    };
    using pnn = pair<node*,node*>;
    node* root;
    T id1,id2;  //opr1,opr2の単位元
    //クエリに答える演算(minに対応)
    T opr1(T l, T r) {
    	return min(l,r);
    }
    //遅延処理の演算(addに対応)
    T opr2(T l, T r) {
    	return l + r;
    }
    RBST() : root(nullptr), id1(INF), id2(0){}
    RBST(vector<T>& vec) : root(nullptr), id1(INF), id2(0){
    	rep(i,len(vec)){
    		insert(i,vec[i]);
    	}
    }
    int size(node* t) { return t ? t->st_size : 0; }
    T que(node* t) { push(t); return t ? opr2(t->al,t->lazy*size(t)) : id1; }
    //遅延伝播
    node* push(node* t){
        if(!t) return t;
        if(t->lazy_flag){
            t->val = opr2(t->val, t->lazy);
            t->al += t->lazy;
            if(t->left){
                t->left->lazy_flag = true;
                t->left->lazy = opr2(t->left->lazy, t->lazy);
            }
            if(t->right){
                t->right->lazy_flag = true;
                t->right->lazy = opr2(t->right->lazy,t->lazy);
            }
            t->lazy = id2;
            t->lazy_flag = false;
        }
        return t;
    }
    node* update(node *t){
        push(t);
        node* l = t->left; node* r = t->right;
        t->st_size = size(l) + size(r) + 1;
        t->al = opr1(opr1(que(l),que(r)),t->val);
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
        push(l),push(r);
        if(rnd() % (size(l) + size(r)) < (unsigned)size(l)){
            l->right = merge(l->right, r);
            return update(l);
        }else{
            r->left = merge(l, r->left);
            return update(r);
        }
    }
    pnn split(node* t, int k){
        if(!t) return pnn(nullptr, nullptr);
        push(t);
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
    node* insert(node* t, int k, node* u){
        auto s = split(t, k);
        return merge(merge(s.first, u), s.second);
    }
    void insert(int k, T val){
        root = insert(root, k, new node(val));
    }
    node* erase(node* t, int k){
        auto sl = split(t, k);
        auto sr = split(sl.second, 1);
        delete sr.first;
        return merge(sl.first, sr.second);
    }
    void erase(int k){
        root = erase(root, k);
    }
    T get(int k){
        auto sr = split(root, k+1);
        auto sl = split(sr.first, k);
        auto lr = sl.second;
        T res = lr->val;
        root = merge(merge(sl.first, lr), sr.second);
        return res;
    }
    void range(int l, int r, T val){
        if(l >= r)  return;
        auto sr = split(root, r);
        auto sl = split(sr.first, l);
        auto lr = sl.second;
        lr->lazy = val;
        lr->lazy_flag = true;
        root = merge(merge(sl.first,lr),sr.second);
    }
    T query(int l,int r){
        if(l >= r)  return 0;
        auto sr = split(root,r);
        auto sl = split(sr.first,l);
        auto lr = sl.second;
        T res = que(lr);
        root = merge(merge(sl.first, lr), sr.second);
        return res;
    }
};

int main()
{
    int n,q;
    scanf("%d",&q);
    string s;
    cin >> s;
    n = len(s);
    RBST<int> rb;
    int sm = 0;
    rep(i,n){
        if(s[i] == '('){
            rb.insert(i,sm);
            ++sm;
        }else{
            rb.insert(i,sm);
            --sm;
        }
    }
    rb.insert(n,sm);
    rep(i,q){
        char c;
        int a,b;
        scanf("%c",&c);
        scanf("%c%d%d",&c,&a,&b);
        // vi vec;
        // rep(i,rb.size(rb.root)){
        //     spair(rb.get(i));
        //     vec.pb(rb.get(i).se);
        // }
        // rep(i,len(vec)){
        //     if(vec[i] == 1){
        //         cout << '(';
        //     }else{
        //         cout << ')';
        //     }
        // }
        // cout << "\n";
        if(c == '('){
            int res = rb.get(a-1);
            rb.insert(a,res);
            rb.range(a,rb.size(rb.root),1);
        }else if(c == ')'){
            int res = rb.get(a-1);
            rb.insert(a,res);
            rb.range(a,rb.size(rb.root),-1);
        }else if(c == 'D'){
            int res1 = rb.get(a-1);
            int res2 = rb.get(a);
            rb.erase(a);
            if(res1 < res2){
                rb.range(a,rb.size(rb.root),-1);
            }else{
                rb.range(a,rb.size(rb.root),1);
            }
        }else{
            int res = rb.get(a-1);
            cout << rb.get(b) - res - 2*(rb.query(a-1,b+1) - res) << "\n";
        }
    }
    return 0;
}
