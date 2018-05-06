#include "../header.hpp"

// selectは実装をやりたくないので実装していない(愚直なにぶたんだとO(log(n))かかってしまうのでうまくブロック分けすることで高速化)
// というわけでselectがないので"矩形内の点を報告する"などの操作をフォローしてません
// https://code.google.com/p/shellinford/
// https://www.dropbox.com/sh/9lknvq4xay709cn/AAAX61z5W1m20MPkJ8V53l5ma/%23WaveletMatrix.cpp?dl=0
// などを参照
// WaveletMatrix は基数ソートのようなことをするのでWaveletTreeとは違い深さがlog(σ)と文字の種類数依存
// のせるデータがアルファベットなどならWaveletTreeに比べて深さは浅いが, 数字(特にnより大きい数字)をのせると深さが深くなる
// そのため直交領域内の点の数を返す OrthogonalRegionCount は前もって座圧をおこなっている
// 直交領域内の点の数は WaveletTree の方が良いかもしれない

#define MAX_BIT 32

struct BitRank {
    // block:bit列を管理, count:blockごとに立っている1の数を管理
    vector<uint> block;
    vector<int> count;
    BitRank(){}
    void resize(int num) {
        block.resize((num+MAX_BIT-1)/MAX_BIT, 0);
        count.resize((int)block.size(), 0);
    }
    // 立っているbitの数を返す
    inline int popcount(uint x) {
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        return (((x + (x >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;
    }
    // iビット目をval(0,1)にセット
    inline void set(int i, int val) {
        block[i/MAX_BIT] |= (val << (i%MAX_BIT));
    }
    void build() {
        for(int i = 1; i < (int)block.size(); i++){
            count[i] = count[i-1] + popcount(block[i-1]);
        }
    }
    // iビット以下の1の数
    int rank1(int i) {
        return (i < 0)?0:(count[i/MAX_BIT]+popcount(block[i/MAX_BIT]<<(MAX_BIT-i%MAX_BIT-1)));
    }
    // jビット目からiビット目までの1の数
    int rank1(int i,int j) {
        return rank1(j) - rank1(i-1);
    }
    // iビット以下の0の数
    int rank0(int i) {
        return (i<0)?0:(i-rank1(i)+1);
    }
    // jビット目からiビット目までの0の数
    int rank0(int i, int j) {
        return rank0(j) - rank0(i-1);
    }
};

class WaveletMatrix
{
private:
    uint height;
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
        height = MAX_BIT - __builtin_clz(sigma-1);
        if(sigma == 1) height = 1;
        B.resize(height), pos.resize(height);
        for(uint i = 0; i < height; i++){
            B[i].resize((int)vec.size());
            for(int j = 0; j < (int)vec.size(); j++) {
                B[i].set(j, access(vec[j], height - i - 1));
            }
            B[i].build();
            auto it = stable_partition(vec.begin(), vec.end(), [&](int c) {
                return !access(c, height - i - 1);
            });
            pos[i] = it - vec.begin();
        }
    }
    // valのiビット目の値を返す(0,1)
    inline int access(int val, int i) {
        return (val >> i) & 1;
    }
    // [l,r]の間に現れる値cの数
    int rank(int val,int l,int r) {
        return rank(val, r) - rank(val, l - 1);
    }
    int rank(int val, int i) {
        int p = -1;
        for(uint j = 0; j < height; j++){
            if(access(val, height - j - 1)){
                p = pos[j] + B[j].rank1(p) - 1;
                i = pos[j] + B[j].rank1(i) - 1;
            }else{
                p = B[j].rank0(p) - 1;
                i = B[j].rank0(i) - 1;
            }
        }
        return i - p;
    }
    // [l,r]のk(1,2,3...)番目に小さい値を返す
    int quantile(int k, int l, int r) {
        int res = 0;
        for(uint i = 0; i < height; i++){
            int j = B[i].rank0(l, r);
            if(j >= k){
                l = B[i].rank0(l-1);
                r = B[i].rank0(r) - 1;
            }else{
                l = pos[i] + B[i].rank1(l-1);
                r = pos[i] + B[i].rank1(r) - 1;
                k -= j;
                res |= (1 << (height - i - 1));
            }
        }
        return res;
    }
    // [l,r]で値が[a,b]内に含まれる数を返す
    int rangefreq(int l, int r, int a, int b) {
        return rangefreq(l, r, a, b, 0, (1 << height) - 1, 0);
    }
    int rangefreq(int i, int j, int a, int b, int l, int r, int x) {
        if(b < l || r < a) return 0;
        int mid = l + (r - l) / 2;
        if(a <= l && r <= b){
            return j - i + 1;
        }else{
            int left = rangefreq(B[x].rank0(i-1),B[x].rank0(j)-1,a,b,l,mid,x+1);
            int right = rangefreq(pos[x]+B[x].rank1(i-1),pos[x]+B[x].rank1(j)-1,a,b,mid+1,r,x+1);
            return left + right;
        }
    }
};

// 2次元領域の数を数えたい場合
template<typename T> class OrthogonalRangeCount
{
private:
    using ptt = pair<T, T>;
    vector<T> X, Y;
    WaveletMatrix wm;
    int n;
public:
    // 座標を引数に渡す
    OrthogonalRangeCount(vector<ptt> candidate) {
        int n = (int)candidate.size();
        sort(candidate.begin(), candidate.end());
        X.resize(n), Y.resize(n);
        vector<T> vec(n);
        rep(i,n){
            X[i] = candidate[i].first, Y[i] = candidate[i].second;
        }
        sort(Y.begin(), Y.end());
        Y.erase(unique(Y.begin(), Y.end()), Y.end());
        rep(i,n){
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
        return wm.rangefreq(lxid, rxid-1, lyid, ryid-1);
    }
};
