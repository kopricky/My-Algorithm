#include "../header.hpp"

//長方形領域内の(更新なしの)クエリに答える(この実装では長方形領域内に含まれる点の値の和を答える)
//時間計算量:構築O(nlog(n)),クエリO(log^2(n))
//空間計算量:O(nlog(n))

#define all(v) (v).begin(),(v).end()

template<typename CandidateType, typename ValueType> class segtree
{
private:
    using CT = CandidateType;
    using VT = ValueType;
    using pcc = pair<CT, CT>;
    using pci = pair<CT, int>;
    int n, sz;
    // x座標
    vector<CT> xs;
    // y座標
    vector<vector<pci> > ys;
    // 累積和
    vector<vector<VT> > sum;
    VT _query(int a, int b, const CT ly, const CT ry){
        VT res1 = 0, res2 = 0;
        a += n, b += n;
        while(a != b){
            if(a & 1){
                const int c = lower_bound(all(ys[a]), pci(ly, -1)) - ys[a].begin();
                const int d = upper_bound(all(ys[a]), pci(ry, -1)) - ys[a].begin();
                res1 += sum[a][d] - sum[a][c];
                ++a;
            }
            if(b & 1){
                --b;
                const int c = lower_bound(all(ys[b]), pci(ly, -1)) - ys[b].begin();
                const int d = upper_bound(all(ys[b]), pci(ry, -1)) - ys[b].begin();
                res2 += sum[b][d] - sum[b][c];
            }
            a >>= 1, b >>= 1;
        }
        return res1 + res2;
    }
public:
    segtree(const vector<pcc>& cand, const vector<VT>& val) : n(1), sz((int)cand.size()), xs(sz){
        while(n < sz) n *= 2;
        vector<pair<pcc, int> > sorted(sz);
        for(int i = 0; i < sz; ++i) sorted[i] = {cand[i], i};
        sort(all(sorted));
        ys.resize(2*n), sum.resize(2*n);
        for(int i = 0; i < sz; ++i){
            xs[i] = (sorted[i].first).first;
            ys[i+n] = {pci((sorted[i].first).second, sorted[i].second)};
            sum[i+n] = {0, val[sorted[i].second]};
        }
        for(int i = n - 1; i >= 1; --i){
            ys[i].resize((int)ys[2*i].size() + (int)ys[2*i+1].size());
            merge(all(ys[2*i]), all(ys[2*i+1]), ys[i].begin(), [&](pci& a, pci& b){
                return a.first < b.first;
            });
            sum[i].resize((int)ys[i].size() + 1, 0);
            for(int j = 0; j < (int)ys[i].size(); ++j){
                sum[i][j+1] = sum[i][j] + val[ys[i][j].second];
            }
        }
    }
    //[lx,rx)×[ly,ry)の長方形領域に含まれる値の和に答える
    VT query(const CT lx, const CT ly, const CT rx, const CT ry){
        const int lxid = lower_bound(all(xs), lx) - xs.begin();
        const int rxid = upper_bound(all(xs), rx-1) - xs.begin();
        if(lxid >= rxid) return 0;
        return _query(lxid, rxid, ly, ry);
    }
};
