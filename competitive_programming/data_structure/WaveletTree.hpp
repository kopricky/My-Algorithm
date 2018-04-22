#include "../header.hpp"

//(注)ビットベクターを用いていないので正確にはWaveletTreeではない?
//というかそもそもrankの実装しか(lcに対応)してない
//[l,r)のk番目の値などを求める(一次元)
//計算量は構築O(nlongn),クエリO(logn)
template<typename T> class WaveletTree {
public:
    using pti = pair<T, int>;
    vector<pti> x;
    //d[i][j]:深さiでのj番目の数(元の配列のインデックス)
    //lc[i][j]:深さiでのj番目の数が左の子の何番目にいくか
    vector<vector<int> > d, lc;
    int sz, n, logn;
    void built(int l, int r, int k) {
        if(k == logn) return;
        int pl = l, mid = (l+r)/2, pr = mid;
        for(int i = l; i < r; i++){
            if(d[k][i] < mid){
                d[k+1][pl++] = d[k][i];
            }else{
                d[k+1][pr++] = d[k][i];
            }
            lc[k][i] = pl-l;
        }
        built(l, mid, k+1);
        built(mid, r, k+1);
    }
    int get(int l, int r, int ord, int k, int id) {
        if(k == logn) return d[k][l];
        int ls = (l > id ? lc[k][l-1] : 0);
        int c = lc[k][r-1] - ls;
        if(ord < c){
            return get(id+ls, id+ls+c, ord, k+1, id);
        }else{
            int cnt = 1 << (logn-k-1);
            return get(l+cnt-ls, r+cnt-(ls+c), ord-c, k+1, id+cnt);
        }
        return -1;
    }
    int find(int l, int r, int ord, int k, int id){
        if(r <= l) return 0;
        if(k == logn) return (ord <= l ? 0 : 1);
        int ls = (l > id ? lc[k][l-1] : 0);
        int c = lc[k][r-1] - ls;
        int cnt = 1 << (logn-k-1);
        int mid = id + cnt;
        if(ord <= mid){
            return find(id+ls, id+ls+c, ord, k+1, id);
        }else{
            return find(l+cnt-ls, r+cnt-(ls+c), ord, k+1, id+cnt) + c;
        }
    }
public:
    WaveletTree(vector<T>& vec) {
        sz = (int)vec.size();
        n = 1, logn = 0;
        while(n < sz){
            n *= 2, logn++;
        }
        x.resize(n);
        d.resize(logn+1, vector<int>(n, -1)), lc.resize(logn, vector<int>(n, -1));
        rep(i,sz){
            x[i] = pti(vec[i], i);
        }
        rep(i,n-sz){
            x[i+sz].second = i+sz;
        }
        sort(x.begin(), x.end());
        //いわゆる座圧
        rep(i,n){
            d[0][x[i].second] = i;
        }
        built(0, n, 0);
    }
    //[l,r)でord番目の値を求める(ordは0番目スタート)
    T get(int l, int r, int ord) {
        return x[get(l, r, ord, 0, 0)].first;
    }
    //[l,r)でcriが小さい方から何番目か(正確にはlower_bound)(返り値は0番目スタート)
    int find(int l, int r, T cri){
        return find(l, r, lower_bound(x.begin(), x.end(), P(cri, -1)) - x.begin(), 0, 0);
    }
    //[l,r)でcriがいくつ含まれるか
    int count(int l, int r, T cri){
        return find(l, r, cri+1) - find(l, r, cri);
    }
};
