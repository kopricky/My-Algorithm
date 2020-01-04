#include "../header.hpp"

//領域木(フラクショナルカスケーディングは実装していません)
//2次元上の一点更新および長方形領域のクエリに答えるデータ構造(この実装では一点更新および長方形領域の総和)
//遅延処理は多次元だとうまく回らないので範囲クエリは扱えない(多次元領域の更新をO(polylog(n))でできるデータ構造は知られていない)
//点数がn個の場合
//時間計算量:構築O(nlog(n)),クエリO(log^2(n))
//空間計算量:O(nlog(n))
//verifyはしていません(verify問題を知らない)

#define all(v) (v).begin(),(v).end()

template<typename T> class segtree {
private:
    int n, sz;
    vector<T> node;
public:
    void init(const vector<T>& v){
        sz = (int)v.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.assign(2*n, numeric_limits<T>::min());
        for(int i = 0; i < sz; i++){
            node[i+n] = v[i];
        }
        for(int i=n-1; i>=1; i--){
            node[i] = max(node[2*i], node[2*i+1]);
        }
    }
    void update(int k, const T a)
    {
    	node[k+=n] = a;
    	while(k>>=1){
    		node[k] = max(node[2*k], node[2*k+1]);
    	}
    }
    T query(int a, int b)
    {
        T res1 = numeric_limits<T>::min(), res2 = numeric_limits<T>::min();
        a += n, b += n;
        while(a != b){
            if(a % 2) res1 = max(res1, node[a++]);
            if(b % 2) res2 = max(res2, node[--b]);
            a >>= 1, b >>= 1;
        }
        return max(res1, res2);
    }
    void print(){
        for(int i = 0; i < sz; i++){
            cout << query(i, i+1) << " ";
        }
        cout << endl;
    }
};

//座標の型, 値の型
template<typename CandidateType, typename ValueType> class RangeTree
{
public:
    static_assert(std::is_integral<CandidateType>::value, "Integral required.");
private:
    using CT = CandidateType;
    using VT = ValueType;
    using pcc = pair<CT, CT>;
    using pci = pair<CT, int>;
    int n, sz;
    vector<segtree<VT> > seg;
    // y座標, x座標
    vector<vector<pcc> > yx;
    // y座標, x座標
    vector<pcc> sorted;
    void update_(int id, const CT x, const CT y, const VT val) {
        id += n-1;
        const int yid = lower_bound(all(yx[id]), pcc(y, x)) - yx[id].begin();
        seg[id].update(yid, val);
        while(id > 0){
            id = (id - 1) / 2;
            const int yid = lower_bound(all(yx[id]), pcc(y, x)) - yx[id].begin();
            seg[id].update(yid, val);
        }
    }
    VT query(const int lxid, const int rxid, const CT ly, const CT ry, const int k, const int l, const int r) {
        if(r <= lxid || rxid <= l) return numeric_limits<VT>::min();
        if(lxid <= l && r <= rxid){
            const int lyid = lower_bound(all(yx[k]), pcc(ly, numeric_limits<CT>::min())) - yx[k].begin();
            const int ryid = upper_bound(all(yx[k]), pcc(ry, numeric_limits<CT>::min())) - yx[k].begin();
            return (lyid >= ryid) ? numeric_limits<VT>::min() : seg[k].query(lyid, ryid);
        }else{
            return max(query(lxid, rxid, ly, ry, 2*k+1, l, (l+r)/2), query(lxid, rxid, ly, ry, 2*k+2, (l+r)/2, r));
        }
    }
public:
    // 座標, 点の値
    RangeTree(const vector<pcc>& cand, const vector<VT>& val) : n(1), sz((int)cand.size()), sorted(sz){
        while(n < sz) n *= 2;
        for(int i = 0; i < sz; ++i){
            sorted[i] = {cand[i].first, i};
        }
        sort(all(sorted), [&](const pcc& a, const pcc& b){
            return (a.first == b.first) ? (cand[a.second].second < cand[b.second].second) : (a.first < b.first);
        });
        yx.resize(2*n-1), seg.resize(2*n-1);
        for(int i = 0; i < sz; ++i){
            yx[i+n-1] = {{sorted[i].second, sorted[i].first}};
            vector<VT> arg = {val[sorted[i].second]};
            seg[i+n-1].init(arg);
            sorted[i].second = cand[sorted[i].second].second;
        }
        for(int i = n-2; i >= 0; --i){
            yx[i].resize((int)yx[2*i+1].size() + (int)yx[2*i+2].size());
            if(yx[i].empty()) continue;
            merge(all(yx[2*i+1]), all(yx[2*i+2]), yx[i].begin(), [&](const pcc& a, const pcc& b){
                return (cand[a.first].second == cand[b.first].second)
                        ? (a.second < b.second) : (cand[a.first].second < cand[b.first].second);
            });
            vector<VT> arg((int)yx[i].size());
            for(int j = 0; j < (int)yx[i].size(); ++j){
                arg[j] = val[yx[i][j].first];
            }
            seg[i].init(arg);
        }
        for(int i = 0; i < 2*n-1; ++i){
            for(pcc& e : yx[i]){
                e.first = cand[e.first].second;
            }
        }
    }
    // 点 (x,y) の更新を行う
    void update(const CT x, const CT y, const VT val){
        const int id = lower_bound(all(sorted), pcc(x, y)) - sorted.begin();
        return update_(id, x, y, val);
    }
    // [lx,rx) × [ly,ry) の長方形領域のクエリに答える
    VT query(const CT lx, const CT ly, const CT rx, const CT ry){
        const int lxid = lower_bound(all(sorted), pcc(lx, numeric_limits<CT>::min())) - sorted.begin();
        const int rxid = upper_bound(all(sorted), pcc(rx, numeric_limits<CT>::min())) - sorted.begin();
        return (lxid >= rxid) ? numeric_limits<VT>::min() : query(lxid, rxid, ly, ry, 0, 0, n);
    }
};
