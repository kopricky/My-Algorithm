template<typename V> class segtree{
private:
    int n,sz;
    vector<V> node, lazy;

public:
    segtree(vector<V>& v){
        sz = (int)v.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.resize(2*n-1, 0);
        lazy.resize(2*n-1, 0);
        rep(i,sz){
            node[i+n-1] = v[i];
        }
        for(int i=n-2; i>=0; i--){
            node[i] = node[i*2+1] + node[i*2+2];
        }
    }
    void eval(int k, int l, int r){
        if(lazy[k] != 0){
            node[k] += lazy[k]; //kを根とするsubtreeについての情報を更新
            if(r - l > 1){
                lazy[2*k+1] += lazy[k] / 2;
                lazy[2*k+2] += lazy[k] / 2;
            }
            lazy[k] = 0;
        }
    }
    void range(int a, int b, V x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a){
            return;
        }
        if(a <= l && r <= b){
            lazy[k] += (r - l) * x;
            eval(k, l, r);
        }else{
            range(a, b, x, 2*k+1, l, (l+r)/2);
            range(a, b, x, 2*k+2, (l+r)/2, r);
            node[k] = node[2*k+1] + node[2*k+2];
        }
    }
    V query(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a){
            return 0;
        }
        if(a <= l && r <= b){
            return node[k];
        }
        V vl = query(a, b, 2*k+1, l, (l+r)/2);
        V vr = query(a, b, 2*k+2, (l+r)/2, r);
        return vl + vr;
    }
    void print(){rep(i,sz)cout<<query(i,i+1)<< " ";cout<<endl;}
};
