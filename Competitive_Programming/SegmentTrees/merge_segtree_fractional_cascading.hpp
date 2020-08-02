#include "../header.hpp"

template<typename CandidateType, typename ValueType> class segtree
{
private:
    using CT = CandidateType;
    using VT = ValueType;
    struct info {
        int left, right;
        VT sum;
        info(const int _left, const int _right, const VT _sum)
            : left(_left), right(_right), sum(_sum){}
    };
    int m;
    vector<CT> xs, ys;
    vector<info> ptr;
    vector<int> st;
    int update(const int prv, const int xid, const VT val, const int l, const int r){
        const int cur = (int)ptr.size();
        ptr.emplace_back(0, 0, ptr[prv].sum + val);
        if(r - l == 1) return cur;
        const int mid = (l + r) / 2;
        if(xid < mid){
            const int res = update(ptr[prv].left, xid, val, l, mid);
            ptr[cur].left = res, ptr[cur].right = ptr[prv].right;
        }else{
            const int res = update(ptr[prv].right, xid, val, mid, r);
            ptr[cur].left = ptr[prv].left, ptr[cur].right = res;
        }
        ptr[cur].sum = ptr[ptr[cur].left].sum + ptr[ptr[cur].right].sum;
        return cur;
    }
    void preprocessing(const vector<pair<CT, CT> >& cand, const vector<VT>& val){
        const int n = (int)cand.size();
        vector<pair<CT, int> > sorted(n);
        for(int i = 0; i < n; ++i){
            xs[i] = cand[i].first, sorted[i] = {cand[i].second, i};
        }
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        sort(sorted.begin(), sorted.end());
        m = (int)xs.size();
        int prv = 0;
        st.push_back(prv), ptr.emplace_back(0, 0, 0);
        for(const auto& p : sorted){
            const int newx = (int)(lower_bound(xs.begin(), xs.end(), cand[p.second].first) - xs.begin());
            prv = update(prv, newx, val[p.second], 0, m);
            if(ys.empty() || ys.back() < p.first) st.push_back(prv), ys.push_back(p.first);
            else st.back() = prv;
        }
    }
    VT query(const int cur, const int a, const int b, const int l, const int r){
        if(cur == 0 || b <= l || r <= a) return 0;
        if(a <= l && r <= b) return ptr[cur].sum;
        const int mid = (l + r) / 2;
        return query(ptr[cur].left, a, min(b, mid), l, mid) + query(ptr[cur].right, max(a, mid), b, mid, r);
    }
public:
    segtree(const vector<pair<CT, CT> >& cand, const vector<VT>& val) : xs((int)cand.size()){
        preprocessing(cand, val);
    }
    //[lx,rx)×[ly,ry)の長方形領域に含まれる値の和に答える
    VT query(const CT lx, const CT ly, const CT rx, const CT ry){
        const int lxid = (int)(lower_bound(xs.begin(), xs.end(), lx) - xs.begin());
        const int rxid = (int)(lower_bound(xs.begin(), xs.end(), rx) - xs.begin());
        const int lyid = (int)(lower_bound(ys.begin(), ys.end(), ly) - ys.begin());
        const int ryid = (int)(lower_bound(ys.begin(), ys.end(), ry) - ys.begin());
        if(lxid == rxid || lyid == ryid) return 0;
        else return query(st[ryid], lxid, rxid, 0, m) - query(st[lyid], lxid, rxid, 0, m);
    }
};