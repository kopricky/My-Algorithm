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
    void query(const CT lx, const CT rx, const CT ly, const CT ry, vector<int>& report) {
        const int lxid = lower_bound(all(xs),lx) - xs.begin();
        const int rxid = upper_bound(all(xs),rx-1) - xs.begin();
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
    using pci = pair<CT, int>;
    int n, sz;
    // x座標
    vector<CT> xs;
    // y座標, インデックス
    vector<vector<pci> > ys;
    // 位置
    vector<vector<array<int, 2> > > pos;
    static bool less(const pci& a, const pci& b, const vector<pcc>& cand){
        return (a.first == b.first) ? (cand[a.second].first < cand[b.second].first) : (a.first < b.first);
    }
    void query(const int lxid, const int rxid, const int lyid, const int ryid,
                vector<int>& report, const int k, const int l, const int r){
        if(lyid >= ryid || r <= lxid || rxid <= l) return;
        if(lxid <= l && r <= rxid){
            for(int i = lyid; i < ryid; ++i){
                report.push_back(ys[k][i].second);
            }
        }else{
            query(lxid, rxid, pos[k][lyid][0], pos[k][ryid][0], report, 2*k+1, l, (l+r)/2);
            query(lxid, rxid, pos[k][lyid][1], pos[k][ryid][1], report, 2*k+2, (l+r)/2, r);
        }
    }
public:
    OrthogonalRangeReport(const vector<pcc>& cand) : n(1), sz((int)cand.size()), xs(sz){
        while(n < sz) n *= 2;
        vector<pci> sorted(sz);
        for(int i = 0; i < sz; ++i){
            sorted[i] = {cand[i].first, i};
        }
        sort(sorted.begin(), sorted.end());
        ys.resize(2*n-1), pos.resize(n-1);
        for(int i = 0; i < sz; ++i){
            xs[i] = sorted[i].first;
            ys[i+n-1] = {make_pair(cand[sorted[i].second].second, sorted[i].second)};
        }
        for(int i = n-2; i >= 0; --i){
            ys[i].resize((int)ys[2*i+1].size() + (int)ys[2*i+2].size());
            merge(all(ys[2*i+1]), all(ys[2*i+2]), ys[i].begin(), [&](const pci& a, const pci& b){
                return (a.first == b.first) ? (cand[a.second].first < cand[b.second].first) : (a.first < b.first);
            });
            pos[i].resize((int)ys[i].size() + 1);
            for(int k = 0; k < 2; ++k){
                int p = 0;
                for(int j = 0; j < (int)ys[i].size(); ++j){
                    while(p < (int)ys[2*i+k+1].size() && less(ys[2*i+k+1][p], ys[i][j], cand)) ++p;
                    pos[i][j][k] = p;
                }
                pos[i][(int)ys[i].size()][k] = (int)ys[2*i+k+1].size();
            }
        }
    }
    // [lx,rx)×[ly,ry)の長方形領域の範囲内の点のインデックスを報告する
    void query(const CT lx, const CT rx, const CT ly, const CT ry, vector<int>& report) {
        const int lxid = lower_bound(all(xs), lx) - xs.begin();
        const int rxid = lower_bound(all(xs), rx) - xs.begin();
        const int lyid = lower_bound(all(ys[0]), pci(ly, -1)) - ys[0].begin();
        const int ryid = lower_bound(all(ys[0]), pci(ry, -1)) - ys[0].begin();
        if(lxid >= rxid) return;
        query(lxid, rxid, lyid, ryid, report, 0, 0, n);
    }
};
