#include "../../header.hpp"

template<typename T> class segtree {
private:
    int n, sz, h;
    const T inf;
    vector<T> node, min_val, second_val, max_val, lazy, lazy_add;
    vector<int> count;
    void update(int id){
        node[id] = node[2*id] + node[2*id+1];
        max_val[id] = max(max_val[2*id], max_val[2*id+1]);
        if(min_val[2*id] < min_val[2*id+1]){
            min_val[id] = min_val[2*id];
            second_val[id] = min(second_val[2*id], min_val[2*id+1]);
            count[id] = count[2*id];
        }else if(min_val[2*id] > min_val[2*id+1]){
            min_val[id] = min_val[2*id+1];
            second_val[id] = min(min_val[2*id], second_val[2*id+1]);
            count[id] = count[2*id+1];
        }else{
            min_val[id] = min_val[2*id];
            second_val[id] = min(second_val[2*id], second_val[2*id+1]);
            count[id] = count[2*id] + count[2*id+1];
        }
    }
public:
    segtree(const vector<T>& v)
        : n(1), sz((int)v.size()), h(0), inf(numeric_limits<T>::max() / 2){
        while(n < sz) n *= 2, ++h;
        node.resize(2*n, 0), min_val.resize(2*n, inf), second_val.resize(2*n, inf);
        max_val.resize(2*n, -inf), lazy.resize(2*n, inf), lazy_add.resize(2*n, 0);
        count.resize(2*n, 1);
        for(int i = 0; i < sz; ++i){
            node[i+n] = min_val[i+n] = max_val[i+n] = v[i];
        }
        for(int i = n-1; i >= 1; --i) update(i);
    }
    void eval(int k, int length){
        if(lazy_add[k] != 0){
            node[k] += lazy_add[k] * length;
            min_val[k] += lazy_add[k], second_val[k] += lazy_add[k], max_val[k] += lazy_add[k];
            if(k < n){
                lazy[2*k] += lazy_add[k], lazy[2*k+1] += lazy_add[k];
                lazy_add[2*k] += lazy_add[k], lazy_add[2*k+1] += lazy_add[k];
            }
            lazy_add[k] = 0;
        }
        if(lazy[k] < (inf >> 1)){
            if(lazy[k] > min_val[k]){
                node[k] += (lazy[k] - min_val[k]) * count[k];
                min_val[k] = lazy[k], max_val[k] = max(max_val[k], lazy[k]);
                if(k < n){
                    lazy[2*k] = lazy[2*k+1] = lazy[k];
                }
            }
            lazy[k] = inf;
        }
    }
    void range_maxupdate(int a, int b, T x, int k=1, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, r-l);
        if(b <= l || r <= a || min_val[k] >= x) return;
        if(a <= l && r <= b && second_val[k] > x){
            lazy[k] = x, eval(k, r-l);
        }else{
            range_maxupdate(a, b, x, 2*k, l, (l+r)>>1);
            range_maxupdate(a, b, x, 2*k+1, (l+r)>>1, r);
            update(k);
        }
    }
    void range_add(int a, int b, T x, int k=1, int l=0, int r=-1){
        if(r < 0) r = n;
        eval(k, r-l);
        if(b <= l || r <= a) return;
        if(a <= l && r <= b){
            lazy_add[k] += x, eval(k, r-l);
        }else{
            range_add(a, b, x, 2*k, l, (l+r)>>1);
            range_add(a, b, x, 2*k+1, (l+r)>>1, r);
            update(k);
        }
    }
    T query_min(int a, int b){
        int length = n;
        a += n, b += n - 1;
        for(int i = h; i > 0; i--, length >>= 1){
            eval(a >> i, length), eval(b >> i, length);
        }
        b++;
        T res1 = inf, res2 = inf;
        while(a < b){
            if(a & 1) eval(a, length), res1 = min(res1, min_val[a++]);
            if(b & 1) eval(--b, length), res2 = min(res2, min_val[b]);
            a >>= 1, b >>= 1, length <<= 1;
        }
        return min(res1, res2);
    }
    T query_max(int a, int b){
        int length = n;
        a += n, b += n - 1;
        for(int i = h; i > 0; i--, length >>= 1){
            eval(a >> i, length), eval(b >> i, length);
        }
        b++;
        T res1 = -inf, res2 = -inf;
        while(a < b) {
            if(a & 1) eval(a, length), res1 = max(res1, max_val[a++]);
            if(b & 1) eval(--b, length), res2 = max(res2, max_val[b]);
            a >>= 1, b >>= 1, length <<= 1;
        }
        return max(res1, res2);
    }
    T query_sum(int a, int b){
        int length = n;
        a += n, b += n - 1;
        for(int i = h; i > 0; i--, length >>= 1){
            eval(a >> i, length), eval(b >> i, length);
        }
        b++;
        T res1 = 0, res2 = 0;
        while(a < b) {
            if(a & 1) eval(a, length), res1 += node[a++];
            if(b & 1) eval(--b, length), res2 += node[b];
            a >>= 1, b >>= 1, length <<= 1;
        }
        return res1 + res2;
    }
    void print()
    {
        for(int i = 0; i < sz; i++){
            cout << query_sum(i,i+1) << " ";
        }
        cout << endl;
    }
};
