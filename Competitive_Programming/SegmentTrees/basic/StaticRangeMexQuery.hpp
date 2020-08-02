#include "../../header.hpp"

class segtree {
private:
    int n, m;
    vector<array<int, 3> > ptr;
    vector<int> arr, st;
    int update(const int prv, const int id, const int val, const int l, const int r){
        const int cur = (int)ptr.size();
        ptr.push_back({0, 0, id});
        if(r - l == 1) return cur;
        const int mid = (l + r) / 2;
        if(val < mid){
            const int res = update(ptr[prv][0], id, val, l, mid);
            ptr[cur][0] = res, ptr[cur][1] = ptr[prv][1];
        }else{
            const int res = update(ptr[prv][1], id, val, mid, r);
            ptr[cur][0] = ptr[prv][0], ptr[cur][1] = res;
        }
        ptr[cur][2] = min(ptr[ptr[cur][0]][2], ptr[ptr[cur][1]][2]);
        return cur;
    }
    void preprocessing(const vector<int>& vec){
        arr[0] = 0;
        for(int i = 0; i < n; ++i) arr[2 * i + 1] = vec[i], arr[2 * i + 2] = vec[i] + 1;
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
        m = (int)arr.size(), st[0] = 0, ptr.push_back({0, 0, -1});
        for(int i = 0; i < n; ++i){
            const int val = (int)(lower_bound(arr.begin(), arr.end(), vec[i]) - arr.begin());
            st[i + 1] = update(st[i], i, val, 0, m);
        }
    }
    int query(const int cur, const int cri, const int l, const int r){
        if(cur == 0 || r - l == 1) return arr[l];
        const int mid = (l + r) / 2;
        if(ptr[ptr[cur][0]][2] < cri) return query(ptr[cur][0], cri, l, mid);
        else return query(ptr[cur][1], cri, mid, r);
    }
public:
    segtree(const vector<int>& vec) : n((int)vec.size()), arr(2 * n + 1), st(n + 1){
        preprocessing(vec);
    }
    int query(const int l, const int r){
        return query(st[r], l, 0, m);
    }
};

