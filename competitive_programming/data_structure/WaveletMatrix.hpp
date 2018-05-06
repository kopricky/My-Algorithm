#include "../header.hpp"

// 一部のみverify済み
// select はガチ実装がやばそうなので実装していない(O(log(n))とかの実装ならmerge_segtreeの方が使い勝手が良い)

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
    WaveletMatrix(vector<int>& vec) :
        WaveletMatrix(vec, *max_element(vec.begin(), vec.end()) + 1) {}
    // sigma:文字の種類数
    WaveletMatrix(vector<int>& vec, int sigma) :
        height(MAX_BIT - __builtin_clz(sigma-1)), B(height), pos(height) {
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
    // [l,r]のk(1,2,3...)番目に大きい値を返す
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
