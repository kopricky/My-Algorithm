//領域木(フラクショナルカスケーディングは実装していません)
//2次元長方形領域の更新およびクエリに答えるデータ構造(例は長方形更新,長方形min)
//時間計算量:構築O(nlog(n)),クエリO(log^2(n))
//空間計算量:O(nlog(n))
#include "../header.hpp"
#define all(v) (v).begin(),(v).end()
template<typename V> class segtree {
private:
    int n,sz; vector<V> node, lazy; vector<bool> lazyFlag;
public:
    segtree(){}
    void init(vector<V>& v){
        sz = (int)v.size(); n = 1;
        while(n < sz) n *= 2;
        node.resize(2*n-1); lazy.resize(2*n-1, 0); lazyFlag.resize(2*n-1,false);
        rep(i,sz) node[i+n-1] = v[i];
        for(int i=n-2; i>=0; i--) node[i] = min(node[i*2+1],node[i*2+2]);
    }
    void eval(int k, int l, int r) {
        if(lazyFlag[k]){
            node[k] = lazy[k];
            if(r - l > 1) {
                lazy[k*2+1] = lazy[k*2+2] = lazy[k]; lazyFlag[k*2+1] = lazyFlag[k*2+2] = true;
            }
            lazyFlag[k] = false;
        }
    }
    void range(int a, int b, V x, int k=0, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a) return;
        if(a <= l && r <= b){
            lazy[k] = x; lazyFlag[k] = true; eval(k, l, r);
        }else{
            range(a, b, x, 2*k+1, l, (l+r)/2); range(a, b, x, 2*k+2, (l+r)/2, r);
            node[k] = min(node[2*k+1],node[2*k+2]);
        }
    }
    V query(int a, int b, int k=0, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a) return INT_MAX;
        if(a <= l && r <= b) return node[k];
        V vl = query(a, b, 2*k+1, l, (l+r)/2), vr = query(a, b, 2*k+2, (l+r)/2, r);
        return min(vl,vr);
    }
    void print(){rep(i,sz)cout<<query(i,i+1)<< " ";cout<<endl;}
};

template<typename CardType, typename ValueType> class RangeTree
{
    using CT = CardType;
    using VT = ValueType;
    using pcc = pair<CT, CT>;
    using pci = pair<CT, int>;
    //
    vector<segtree<VT> > seg;
    //座標, インデックス
    vector<pair<pcc, int> > sorted;
    //x座標
    vector<CT> xs;
    //y座標, インデックス
    vector<pair<CT, int> > ys;
    int n, sz;
    RangeTree(vector<pcc>& cand, vector<VT>& val){
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
        xs.resize(sz), ys.resize(n), seg.resize(n);
        rep(i,sz){
            xs[i] = (sorted[i].first).first;
            ys[i+sz-1] = make_pair((sorted[i].first).second, sorted[i].second);
            vector<VT> arg = {sorted[i].second};
            seg[i+sz-1].init(arg);
        }
        for(int i=n-2; i>=0; i--){
            ys[i].resize((int)ys[2*i+1].size() + (int)ys[2*i+2].size());
            merge(all(ys[2*i+1]),all(ys[2*i+2]),ys[i].begin(),[&](pci& a, pci& b){
                return a.first < b.first;
            });
            vector<VT> arg((int)ys[i].size());
            rep(j,(int)ys[i].size()){
                arg[j] = val[ys[i][j].second];
            }
            seg[i].init(arg);
        }
    }
    void range(int lxid, int rxid, CT ly, CT ry, const VT x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        if(r <= lxid || rxid <= l){
            return;
        }
        if(lxid <= l && r <= rxid){
            int lid = lower_bound(all(ys[k]),ly) - ys[k].begin();
            int rid = upper_bound(all(ys[k]),ry) - ys[k].begin();
            seg[k].range(lid,rid,x);
        }else{
            range(lxid,rxid,ly,ry,2*k+1,l,(l+r)/2), range(lxid,rxid,ly,ry,2*k+2,(l+r)/2,r);
        }
    }
    void range(pcc& l, pcc& r){
        int lxid = lower_bound(all(xs),l.first) - xs.begin();
        int rxid = upper_bound(all(xs),r.first) - xs.begin();
        return range(lxid,rxid,l.second,r.second);
    }
    VT query(int lxid, int rxid, CT ly, CT ry, const VT x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
        if(r <= lxid || rxid <= l){
            return numeric_limits<VT>::max();
        }
        if(lxid <= l && r <= rxid){
            int lid = lower_bound(all(ys[k]),ly) - ys[k].begin();
            int rid = upper_bound(all(ys[k]),ry) - ys[k].begin();
            return seg[k].query(lid,rid,x);
        }else{
            return min(query(lxid,rxid,ly,ry,2*k+1,l,(l+r)/2), query(lxid,rxid,ly,ry,2*k+2,(l+r)/2,r));
        }
    }
    VT query(pcc& l, pcc& r){
        int lxid = lower_bound(all(xs),l.first) - xs.begin();
        int rxid = upper_bound(all(xs),r.first) - xs.begin();
        return query(lxid,rxid,l.second,r.second);
    }
};
