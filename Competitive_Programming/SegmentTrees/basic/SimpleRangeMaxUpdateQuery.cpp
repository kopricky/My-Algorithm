template<typename T> class segtree {
private:
    int n, sz;
    vector<T> min_val, second_val, lazy;
    vector<bool> lazyFlag;
    void update(int id){
        if(min_val[2*id+1] < min_val[2*id+2]){
            min_val[id] = min_val[2*id+1];
            second_val[id] = min(second_val[2*id+1], min_val[2*id+2]);
        }else if(min_val[2*id+1] > min_val[2*id+2]){
            min_val[id] = min_val[2*id+2];
            second_val[id] = min(min_val[2*id+1], second_val[2*id+2]);
        }else{
            min_val[id] = min_val[2*id+1];
            second_val[id] = min(second_val[2*id+1], second_val[2*id+2]);
        }
    }
public:
    segtree(const vector<T>& v) : n(1), sz((int)v.size()){
        while(n < sz) n *= 2;
        lazy.resize(2*n-1, 0);
        lazyFlag.resize(2*n-1, false);
        min_val.resize(2*n-1, numeric_limits<T>::max());
        second_val.resize(2*n-1, numeric_limits<T>::max());
        for(int i = 0; i < sz; i++){
            min_val[i+n-1] = v[i];
        }
        for(int i=n-2; i>=0; i--){
            update(i);
        }
    }
    void eval(int k, int l, int r) {
        if(lazyFlag[k]){
            if(lazy[k] > min_val[k]){
                min_val[k] = lazy[k];
                if(r - l > 1){
                    lazy[k*2+1] = lazy[k*2+2] = lazy[k];
                    lazyFlag[k*2+1] = lazyFlag[k*2+2] = true;
                }
            }
            lazyFlag[k] = false;
        }
    }
    void range(int a, int b, T x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a || min_val[k] >= x){
            return;
        }
        if(a <= l && r <= b && second_val[k] > x) {
            lazy[k] = x;
            lazyFlag[k] = true;
            eval(k, l, r);
        }else{
            range(a, b, x, 2*k+1, l, (l+r)/2);
            range(a, b, x, 2*k+2, (l+r)/2, r);
            update(k);
        }
    }
    T query(int a, int k=0, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k, l, r);
		if(r - l == 1) return min_val[k];
		if(a < (l + r) / 2) return query(a, 2*k+1, l, (l+r)/2);
		else return query(a, 2*k+2, (l+r)/2, r);
    }
};