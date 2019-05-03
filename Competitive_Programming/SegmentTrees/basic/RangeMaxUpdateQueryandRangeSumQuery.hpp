#include "../../header.hpp"

//更新 a_i → max(a_i,x) (l <= i < r)
//クエリ sum ([l]...a[r-1])
//計算量はならしO(log(n))
//verifyしていません(自作テストケースでは合っています)

template<typename T> class segtree {
private:
    int n,sz;
    vector<T> node, min_val, second_val, lazy;
    vector<int> count;
    vector<bool> lazyFlag;
    void update(int id){
        node[id] = node[2*id+1] + node[2*id+2];
        if(min_val[2*id+1] < min_val[2*id+2]){
            min_val[id] = min_val[2*id+1];
            second_val[id] = min(second_val[2*id+1],min_val[2*id+2]);
            count[id] = count[2*id+1];
        }else if(min_val[2*id+1] > min_val[2*id+2]){
            min_val[id] = min_val[2*id+2];
            second_val[id] = min(min_val[2*id+1],second_val[2*id+2]);
            count[id] = count[2*id+2];
        }else{
            min_val[id] = min_val[2*id+1];
            second_val[id] = min(second_val[2*id+1],second_val[2*id+2]);
            count[id] = count[2*id+1]+count[2*id+2];
        }
    }
public:
    segtree(const vector<T>& v) : n(1), sz((int)v.size()){
        while(n < sz) n *= 2;
        node.resize(2*n-1, 0);
        lazy.resize(2*n-1, 0);
        lazyFlag.resize(2*n-1,false);
        min_val.resize(2*n-1,numeric_limits<T>::max());
        second_val.resize(2*n-1,numeric_limits<T>::max());
        count.resize(2*n-1,1);
        for(int i = 0; i < sz; i++){
            node[i+n-1] = min_val[i+n-1] = v[i];
        }
        for(int i=n-2; i>=0; i--){
            update(i);
        }
    }
    void eval(int k, int l, int r) {
        if(lazyFlag[k]){
            if(lazy[k] > min_val[k]){
                node[k] += (lazy[k] - min_val[k]) * count[k];
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
    T query(int a, int b, int k=0, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a){
            return 0;
        }
        if(a <= l && r <= b){
            return node[k];
        }
        T vl = query(a, b, 2*k+1, l, (l+r)/2);
        T vr = query(a, b, 2*k+2, (l+r)/2, r);
        return vl + vr;
    }
    void print()
    {
        for(int i = 0; i < sz; i++){
            cout << query(i,i+1) << " ";
        }
        cout << endl;
    }
};
