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
    vector<vector<VT> > sum;
    //座標, インデックス
    vector<pair<pcc, int> > sorted;
    //x座標
    vector<CT> xs;
    //y座標, インデックス
    vector<vector<pci> > ys;
    VT query(int lxid, int rxid, CT ly, CT ry, int k, int l, int r){
        if(r <= lxid || rxid <= l) return 0;
        if(lxid <= l && r <= rxid){
            int lyid = lower_bound(all(ys[k]),pci(ly,-1)) - ys[k].begin();
            int ryid = upper_bound(all(ys[k]),pci(ry,-1)) - ys[k].begin();
            if(lyid >= ryid) return 0;
            return sum[k][ryid] - sum[k][lyid];
        }else{
            return query(lxid,rxid,ly,ry,2*k+1,l,(l+r)/2) + query(lxid,rxid,ly,ry,2*k+2,(l+r)/2,r);
        }
    }
public:
    segtree(vector<pcc>& cand, vector<VT>& val) : n(1), sz((int)cand.size()), sorted(sz), xs(sz){
        while(n < sz){
            n *= 2;
        }
        for(int i = 0; i < sz; i++){
            sorted[i] = make_pair(cand[i], i);
        }
        sort(sorted.begin(), sorted.end());
        ys.resize(2*n-1), sum.resize(2*n-1);
        for(int i = 0; i < sz; i++){
            xs[i] = (sorted[i].first).first;
            ys[i+n-1] = {pci((sorted[i].first).second, sorted[i].second)};
            sum[i+n-1] = {0, val[sorted[i].second]};
        }
        for(int i=n-2; i>=0; i--){
            ys[i].resize((int)ys[2*i+1].size() + (int)ys[2*i+2].size());
            merge(all(ys[2*i+1]),all(ys[2*i+2]),ys[i].begin(),[&](pci& a, pci& b){
                return a.first < b.first;
            });
            sum[i].resize((int)ys[i].size()+1,0);
            for(int j = 0; j < (int)ys[i].size(); j++){
                sum[i][j+1] = sum[i][j] + val[ys[i][j].second];
            }
        }
    }
    //[lx,rx)×[ly,ry)の長方形領域に含まれる値の和に答える
    VT query(CT lx, CT rx, CT ly, CT ry){
        int lxid = lower_bound(all(xs),lx) - xs.begin();
        int rxid = upper_bound(all(xs),rx-1) - xs.begin();
        if(lxid >= rxid) return 0;
        return query(lxid,rxid,ly,ry,0,0,n);
    }
};
