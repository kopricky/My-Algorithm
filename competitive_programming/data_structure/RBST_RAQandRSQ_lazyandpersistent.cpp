//永続化遅延処理型平衡二分探索木
//区間コピペ 区間add 区間sum
const int max_size = 27000000;

template <typename T> class RBST {
public:
    struct node{
        T val, lazy, al;
        int st_size;   // 部分木のサイズ
        node* left; node* right;
        bool lazy_flag;
        node(){};
        node(T v) : val(v), al(v), left(nullptr), right(nullptr), st_size(1), lazy(0), lazy_flag(false){}
        ~node() { delete left; delete right; }
    };
    using pnn = pair<node*,node*>;
    node* memo; //永続配列
    int pool_size;
    node* root;
    T id1,id2;  //opr1,opr2の単位元
    //クエリに答える演算(sumに対応)
    T opr1(T l, T r) {
    	return l + r;
    }
    //遅延処理の演算(addに対応)
    T opr2(T l, T r) {
    	return l + r;
    }
    RBST(){
        memo = new node[max_size];
        id1 = id2 = 0;
    }
    //永続化
    node* fix(node* t){
        if(!t)  return t;
        if(pool_size >= max_size) assert(false);
        memo[pool_size] = *t;
        return &memo[pool_size++];
    }
    int size(node* t) { return t ? t->st_size : 0; }
    T que(node* t) { return t ? t->al + t->lazy*size(t) : id1; }
    //遅延伝播
    node* push(node* t){
        if(!t) return t;
        t = fix(t);
        if(t->lazy_flag){
            if(t->left){
                t->left = fix(t->left);
                t->left->lazy_flag = true;
                t->left->lazy = opr2(t->left->lazy, t->lazy);
            }
            if(t->right){
                t->right = fix(t->right);
                t->right->lazy_flag = true;
                t->right->lazy = opr2(t->right->lazy,t->lazy);
            }
            t->val = opr2(t->val, t->lazy);
            t->al = opr1(que(t->left), opr1(t->val, que(t->right)));
            t->lazy = id2;
            t->lazy_flag = false;
        }
        return t;
    }
    node* update(node *t){
        node* l = t->left; node* r = t->right;
        t->st_size = size(l) + size(r) + 1;
        t->al = que(l) + que(r) + t->val;
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
            l = push(l);
            l->right = merge(l->right, r);
            return update(l);
        }else{
            r = push(r);
            r->left = merge(l, r->left);
            return update(r);
        }
    }
    pnn split(node* t, int k){   //木のサイズが(k,n-k)となるように分割する
        if(!t) return pnn(nullptr, nullptr);
        t = push(t);
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
    vector<T> x;    //再構築の際にも用いる
    int node_size;
    int x_size;
    void build(){
        node_size = (int)x.size();
    	pool_size = 0;
    	node *res = nullptr;
    	for (int i = 0; i < node_size; i++) {
    		memo[pool_size] = node(x[i]);
    		res = merge(res, &memo[pool_size++]);
    	}
    	root = res;
    }
    void up(node *t) {
    	if (!t) return;
    	t = push(t);
    	up(t->left);
    	if(x_size >= node_size) return;
    	x[x_size++] = t->val;
    	up(t->right);
    }
    void rebuild(node* t) {
    	x_size = 0;
    	up(t);
        build();
    }
    void add(int l, int r, T val){
        auto sr = split(root, r);
        auto sl = split(sr.first, l);
        node* lr = sl.second;
        lr->lazy = val;
        lr->lazy_flag = true;
        root = merge(merge(sl.first,sl.second),sr.second);
    }
    //[r,s)を[p,q)にコピー
    void trans(int p, int q, int r, int s){
        auto sq = split(root,q);
        auto sp = split(sq.first,p);
        auto ss = split(root,s);
        auto sr = split(ss.first,r);
        root = merge(merge(sp.first,sr.second),sq.second);
    }
    T query(int l,int r){
        auto sr = split(root,r);
        auto sl = split(sr.first,l);
        T res = que(sl.second);
        return res;
    }
};
