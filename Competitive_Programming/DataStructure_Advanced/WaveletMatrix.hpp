#include "../header.hpp"

// selectは実装をやりたくないので実装していない(愚直なにぶたんだとO(log(n))かかってしまうのでうまくブロック分けすることで高速化)
// というわけでselectがないので"矩形内の点を報告する"などの操作をフォローしてません
// https://code.google.com/p/shellinford/
// https://www.dropbox.com/sh/9lknvq4xay709cn/AAAX61z5W1m20MPkJ8V53l5ma/%23WaveletMatrix.cpp?dl=0
// などを参照
// WaveletMatrix は基数ソートをするので深さがlog(σ)と文字の種類数依存
// のせるデータがアルファベットなどなら深さは浅いが, 数字(特にnより大きい数字)をのせると深さが深くなる
// そのため直交領域内の点の数を返す OrthogonalRangeCount は前もって座圧をおこなっている

#define MAX_BIT 64

struct BitRank {
    // block: bit 列を管理, count: block ごとに立っている 1 の数を管理
    vector<unsigned long long> block;
    vector<int> count;
    BitRank(){}
    void resize(int num) {
        block.resize((num + 1) / MAX_BIT + 1, 0);
        count.resize((int)block.size(), 0);
    }
    // i ビット目を val(0,1) にセット
    inline void set(int i, unsigned long long val) {
        block[i / MAX_BIT] |= (val << (i % MAX_BIT));
    }
    void build() {
        for(int i = 1; i < (int)block.size(); i++){
            count[i] = count[i-1] + __builtin_popcountll(block[i-1]);
        }
    }
    // [0, i) ビットの 1 の数
    inline int rank1(int i) {
        int j = i / MAX_BIT, k = i % MAX_BIT;
        return count[j] + (k ? __builtin_popcountll(block[j] << (MAX_BIT-k)) : 0);
    }
    // [i, j) ビットの 1 の数
    inline int rank1(int i, int j) {
        return rank1(j) - rank1(i);
    }
    // [0, i) ビットの 0 の数
    inline int rank0(int i) {
        return i - rank1(i);
    }
    // [i, j) ビットの 0 の数
    inline int rank0(int i, int j) {
        return rank0(j) - rank0(i);
    }
};

class WaveletMatrix
{
private:
    int height;
    vector<BitRank> B;
    vector<int> pos;
public:
    WaveletMatrix(){}
    WaveletMatrix(vector<int>& vec) :
        WaveletMatrix(vec, *max_element(vec.begin(), vec.end()) + 1) {}
    // sigma:文字の種類数
    WaveletMatrix(vector<int>& vec, int sigma){
        init(vec, sigma);
    }
    void init(vector<int>& vec, int sigma){
        height = (sigma == 1) ? 1 : (MAX_BIT - __builtin_clzll(sigma-1));
        B.resize(height), pos.resize(height);
        for(int i = 0; i < height; i++){
            B[i].resize((int)vec.size());
            for(int j = 0; j < (int)vec.size(); j++) {
                B[i].set(j, get(vec[j], height - i - 1));
            }
            B[i].build();
            auto it = stable_partition(vec.begin(), vec.end(), [&](int c) {
                return !get(c, height - i - 1);
            });
            pos[i] = it - vec.begin();
        }
    }
    // val の i ビット目の値を返す(0,1)
    inline int get(int val, int i) {
        return val >> i & 1;
    }
    // [l, r) の間に現れる値 val の数
    int rank(int val, int l, int r) {
        return rank(val, r) - rank(val, l);
    }
    // [0, i) の間に現れる値 val の数
    int rank(int val, int i) {
        int p = 0;
        for(int j = 0; j < height; j++){
            if(get(val, height - j - 1)){
                p = pos[j] + B[j].rank1(p);
                i = pos[j] + B[j].rank1(i);
            }else{
                p = B[j].rank0(p);
                i = B[j].rank0(i);
            }
        }
        return i - p;
    }
    // [l, r) の k(0,1,2...) 番目に小さい値を返す
    int quantile(int k, int l, int r){
        int res = 0;
        for(int i = 0; i < height; i++){
            int j = B[i].rank0(l, r);
            if(j > k){
                l = B[i].rank0(l);
                r = B[i].rank0(r);
            }else{
                l = pos[i] + B[i].rank1(l);
                r = pos[i] + B[i].rank1(r);
                k -= j;
                res |= (1 << (height - i - 1));
            }
        }
        return res;
    }
    int rangefreq(int i, int j, int a, int b, int l, int r, int x) {
        if(i == j || r <= a || b <= l) return 0;
        int mid = (l + r) >> 1;
        if(a <= l && r <= b){
            return j - i;
        }else{
            int left = rangefreq(B[x].rank0(i),B[x].rank0(j),a,b,l,mid,x+1);
            int right = rangefreq(pos[x]+B[x].rank1(i),pos[x]+B[x].rank1(j),a,b,mid,r,x+1);
            return left + right;
        }
    }
    // [l,r) で値が [a,b) 内に含まれる数を返す
    int rangefreq(int l, int r, int a, int b) {
        return rangefreq(l, r, a, b, 0, 1 << height, 0);
    }
};

// 長方形領域内の点の数を数えたい場合
template<typename T> class OrthogonalRangeCount
{
private:
    using ptt = pair<T, T>;
    vector<T> X, Y;
    WaveletMatrix wm;

public:
    // 座標を引数に渡す
    OrthogonalRangeCount(vector<ptt> candidate) {
        int n = (int)candidate.size();
        sort(candidate.begin(), candidate.end());
        X.resize(n), Y.resize(n);
        vector<int> vec(n);
        for(int i = 0; i < n; i++){
            X[i] = candidate[i].first, Y[i] = candidate[i].second;
        }
        sort(Y.begin(), Y.end());
        Y.erase(unique(Y.begin(), Y.end()), Y.end());
        for(int i = 0; i < n; i++){
            vec[i] = lower_bound(Y.begin(), Y.end(), candidate[i].second) - Y.begin();
        }
        wm.init(vec, (int)Y.size());
    }
    //[lx,rx)×[ly,ry)の長方形領域に含まれる点の数を答える
    int query(T lx, T ly, T rx, T ry) {
        int lxid = lower_bound(X.begin(), X.end(), lx) - X.begin();
        int rxid = upper_bound(X.begin(), X.end(), rx-1) - X.begin();
        int lyid = lower_bound(Y.begin(), Y.end(), ly) - Y.begin();
        int ryid = upper_bound(Y.begin(), Y.end(), ry-1) - Y.begin();
        if(lxid >= rxid || lyid >= ryid) return 0;
        return wm.rangefreq(lxid, rxid, lyid, ryid);
    }
};
