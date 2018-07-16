#include "../header.hpp"

//長方形領域に含まれる点(インデックス)の報告
//WaveletMatrixよりlog分遅い(selectがO(1)なら)
//時間計算量:構築O(nlog(n)),クエリO(log^2(n)+k)
//空間計算量:O(nlog(n))

#define all(v) (v).begin(),(v).end()

template<typename CandidateType> class OrthogonalRangeReport
{
private:
    using CT = CandidateType;
    using pcc = pair<CT, CT>;
    using pci = pair<CT, int>;
    //座標, インデックス
    vector<pair<pcc, int> > sorted;
    //x座標
    vector<CT> xs;
    //y座標, インデックス
    vector<vector<pci> > ys;
    int n,sz;
    void query(int lxid, int rxid, CT ly, CT ry, list<int>& report, int k, int l, int r){
        if(r <= lxid || rxid <= l) return;
        if(lxid <= l && r <= rxid){
            auto st = lower_bound(all(ys[k]),pci(ly,-1)), ed = upper_bound(all(ys[k]),pci(ry,-1));
            for(auto it = st; it != ed; ++it){
                report.push_back(it->second);
            }
            return;
        }else{
            list<int> left, right;
            query(lxid,rxid,ly,ry,left,2*k+1,l,(l+r)/2);
            query(lxid,rxid,ly,ry,right,2*k+2,(l+r)/2,r);
            report.splice(report.begin(), move(left)), report.splice(report.begin(), move(right));
            return;
        }
    }
public:
    OrthogonalRangeReport(vector<pcc>& cand){
        sz = (int)cand.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        sorted.resize(sz);
        rep(i,sz){
            sorted[i] = make_pair(cand[i], i);
        }
        sort(sorted.begin(), sorted.end());
        xs.resize(sz), ys.resize(2*n-1);
        rep(i,sz){
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
    void query(CT lx, CT ly, CT rx, CT ry, list<int>& report) {
        int lxid = lower_bound(all(xs),lx) - xs.begin();
        int rxid = upper_bound(all(xs),rx-1) - xs.begin();
        if(lxid >= rxid) return;
        return query(lxid,rxid,ly,ry,report,0,0,n);
    }
};
