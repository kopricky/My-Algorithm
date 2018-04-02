// typedef pair<int,int> P;
// #define all(v) (v).begin(),(v).end()
//長方形領域に含まれる点の報告(更新なし)
template<typename T> class segtree
{
public:
    using ptt = pair<T,T>;
    //originalとsort済みのoriginal
    vector<ptt> org,sorg;
    //昇順に並べたy座標の列に対応するインデックス
    vector<vector<int> > id;
    //syは各セグメントにおけるsortされたy座標の列
    vector<vector<T> > sy;
    int n,sz;
    segtree(vector<ptt>& vec){
        sz = (int)vec.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        org = vec;
        sorg.resize(sz);
        id.resize(2*n-1), sy.resize(2*n-1);
        vector<int> index(sz);
        iota(all(index),0);
        sort(all(index),[&](int a, int b){
            return org[a] < org[b];
        });
        rep(i,sz){
            sorg[i] = org[index[i]];
            id[i+n-1].push_back(index[i]);
            sy[i+n-1].push_back(org[index[i]].second);
        }
        for(int i = n-2; i >= 0; i--){
            id[i].resize((int)id[2*i+1].size()+(int)id[2*i+2].size());
            merge(all(id[2*i+1]),all(id[2*i+2]),id[i].begin(),[&](int a, int b){
                return org[a].second < org[b].second;
            });
            sy[i].resize((int)id[i].size());
            rep(j,(int)id[i].size()){
                sy[i][j] = org[id[i][j]].second;
            }
        }
    }
    vector<int> query(int a,int b,const T infy,const T supy,int k=0,int l=0,int r=-1)
    {
        if(r < 0) r = n;
        if(r <= a || b <= l){
            return vector<int>();
        }
        if(a <= l && r <= b){
            int lid = lower_bound(all(sy[k]),infy) - sy[k].begin();
            int rid = upper_bound(all(sy[k]),supy) - sy[k].begin();
            vector<int> ans(rid-lid);
            for(int i = lid; i < rid; i++){
                ans[i-lid] = id[k][i];
            }
            return ans;
        }else{
            vector<int> vl = query(a,b,infy,supy,2*k+1,l,(l+r)/2);
            vector<int> vr = query(a,b,infy,supy,2*k+2,(l+r)/2,r);
            vl.insert(vl.end(),all(vr));
            return vl;
        }
    }
    //左下がl,右上がrの長方形領域に含まれる点のIDを報告
    void query(const ptt l, const ptt r)
    {
        int lid = lower_bound(all(sorg),l) - sorg.begin();
        int rid = upper_bound(all(sorg),r) - sorg.begin();
        vector<int> ans = query(lid,rid,l.second,r.second);
        rep(i,len(ans)){
            cout << ans[i] << "\n";
        }
    }
};
