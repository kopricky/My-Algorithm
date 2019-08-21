#include "../header.hpp"

// 長方形領域に含まれる点(インデックス)の報告
// フラクショナルカスケーディング版はクエリが O(log(n)+k) であるものの構築にかかる計算量や空間計算量が定数倍重いため実測は速くない
// 時間計算量:構築O(nlog(n)),クエリO(log^2(n)+k)
// (フラクショナルカスケーディング版)時間計算量:構築O(nlog(n)),クエリO(log(n)+k)
// 空間計算量:O(nlog(n))

#define all(v) (v).begin(),(v).end()

template<typename CandidateType> class OrthogonalRangeReport
{
private:
    using CT = CandidateType;
    using pcc = pair<CT, CT>;
    using pci = pair<CT, int>;
    int n,sz;
    //座標, インデックス
    vector<pair<pcc, int> > sorted;
    //x座標
    vector<CT> xs;
    //y座標, インデックス
    vector<vector<pci> > ys;
    void query(int lxid, int rxid, CT ly, CT ry, vector<int>& report, int k, int l, int r){
        if(r <= lxid || rxid <= l) return;
        if(lxid <= l && r <= rxid){
            auto st = lower_bound(all(ys[k]),pci(ly,-1)), ed = upper_bound(all(ys[k]),pci(ry,-1));
            for(auto it = st; it != ed; ++it){
                report.push_back(it->second);
            }
        }else{
            query(lxid,rxid,ly,ry,report,2*k+1,l,(l+r)/2);
            query(lxid,rxid,ly,ry,report,2*k+2,(l+r)/2,r);
        }
    }
public:
    OrthogonalRangeReport(const vector<pcc>& cand) : n(1), sz((int)cand.size()), sorted(sz), xs(sz){
        while(n < sz) n *= 2;
        for(int i = 0; i < sz; i++){
            sorted[i] = make_pair(cand[i], i);
        }
        sort(sorted.begin(), sorted.end());
        ys.resize(2*n-1);
        for(int i = 0; i < sz; i++){
            xs[i] = (sorted[i].first).first;
            ys[i+n-1] = {pci((sorted[i].first).second, sorted[i].second)};
        }
        for(int i=n-2; i>=0; i--){
            ys[i].resize((int)ys[2*i+1].size() + (int)ys[2*i+2].size());
            merge(all(ys[2*i+1]),all(ys[2*i+2]),ys[i].begin(),[&](pci& a, pci& b){
                return a.first < b.first;
            });
        }
    }
    // [lx,rx)×[ly,ry)の長方形領域の範囲内の点のインデックスを報告する
    void query(CT lx, CT rx, CT ly, CT ry, vector<int>& report) {
        int lxid = lower_bound(all(xs),lx) - xs.begin();
        int rxid = upper_bound(all(xs),rx-1) - xs.begin();
        if(lxid >= rxid) return;
        query(lxid,rxid,ly,ry,report,0,0,n);
    }
};

// フラクショナルカスケーディング版
template<typename CandidateType> class OrthogonalRangeReport
{
private:
    using CT = CandidateType;
    using pcc = pair<CT, CT>;
    using pcci = pair<pcc, int>;
    int n,sz;
    //座標, インデックス
    vector<pcci> sorted;
    //x座標
    vector<CT> xs;
    // (y座標, x座標), インデックス
    vector<vector<pcci> > ys;
    vector<vector<int> > start[2];
    vector<vector<int> > end[2];
    static bool less(const pcci& a, const pcci& b){
        return ((a.first).first == (b.first).first) ? ((a.first).second < (b.first).second) : ((a.first).first < (b.first).first);
    }
    void query(int lxid, int rxid, int lyid, int ryid, vector<int>& report, int k, int l, int r){
        if(lyid >= ryid) return;
        if(r <= lxid || rxid <= l) return;
        if(lxid <= l && r <= rxid){
            for(int i = lyid; i < ryid; ++i){
                report.push_back(ys[k][i].second);
            }
        }else{
            query(lxid, rxid, start[0][k][lyid], end[0][k][ryid], report, 2*k+1, l, (l+r)/2);
            query(lxid, rxid, start[1][k][lyid], end[1][k][ryid], report, 2*k+2, (l+r)/2, r);
        }
    }
public:
    OrthogonalRangeReport(const vector<pcc>& cand) : n(1), sz((int)cand.size()), sorted(sz), xs(sz){
        while(n < sz) n *= 2;
        for(int i = 0; i < sz; i++){
            sorted[i] = pcci(cand[i], i);
        }
        sort(sorted.begin(), sorted.end());
        ys.resize(2*n-1);
        for(int i = 0; i < 2; ++i){
            start[i].resize(n-1), end[i].resize(n-1);
        }
        for(int i = 0; i < sz; ++i){
            xs[i] = (sorted[i].first).first;
            ys[i+n-1] = {pcci(pcc((sorted[i].first).second, (sorted[i].first).first), sorted[i].second)};
        }
        for(int i = n - 2; i >= 0; --i){
            ys[i].resize((int)ys[2*i+1].size() + (int)ys[2*i+2].size());
            merge(all(ys[2*i+1]), all(ys[2*i+2]), ys[i].begin(), less);
            for(int j = 0; j < 2; ++j){
                start[j][i].resize((int)ys[i].size() + 1);
                end[j][i].resize((int)ys[i].size() + 1);
                int st = 0, ed = 0;
                for(int k = 0; k < (int)ys[i].size(); ++k){
                    while(st < (int)ys[2*i+j+1].size() && less(ys[2*i+j+1][st], ys[i][k])) ++st;
                    start[j][i][k] = st;
                    while(ed < (int)ys[2*i+j+1].size() && less(ys[2*i+j+1][ed], ys[i][k])) ++ed;
                    end[j][i][k] = ed;
                }
                start[j][i].back() = end[j][i].back() = (int)ys[2*i+j+1].size();
            }
        }
    }
    // [lx,rx)×[ly,ry)の長方形領域の範囲内の点のインデックスを報告する
    void query(CT lx, CT rx, CT ly, CT ry, vector<int>& report) {
        const int lxid = lower_bound(all(xs),lx) - xs.begin();
        const int rxid = upper_bound(all(xs),rx-1) - xs.begin();
        const int lyid = lower_bound(all(ys[0]), pcci(pcc(ly, numeric_limits<CT>::min()), 0)) - ys[0].begin();
        const int ryid = upper_bound(all(ys[0]), pcci(pcc(ry, numeric_limits<CT>::min()), 0)) - ys[0].begin();
        if(lxid >= rxid) return;
        query(lxid, rxid, lyid, ryid, report, 0, 0, n);
    }
};
