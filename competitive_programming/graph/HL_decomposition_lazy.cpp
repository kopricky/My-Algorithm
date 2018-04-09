struct HLdecomposition{
    using P = pair<int,int>;
    struct Centroid{
        int parid, pardepth, depth, sz;
        Centroid(int idx, int dep, int deep, int size) : parid(idx), pardepth(dep), depth(deep), sz(size){}
        P Up(){
            return P(parid, pardepth);
        }
    };
    vector<vector<int> > G;
    vector<int> stsize, nxpath;
    vector<int> pathorder, pathid;
    vector<Centroid> centroids;
    vector<int> index;
    void Buildstsize(){
        stack<P> s;
        s.push(P(0, -1));
        while(!s.empty()) {
            P p = s.top();
            s.pop();
            if(stsize[p.first] != -1){
                nxpath[p.first] = -1;
                for(int to : G[p.first]){
                    if(p.second == to){
                        continue;
                    }
                    stsize[p.first] += stsize[to];
                    if(nxpath[p.first] == -1 || stsize[nxpath[p.first]] < stsize[to]) {
                        nxpath[p.first] = to;
                    }
                }
            }else{
                s.push(p);
                stsize[p.first] = 1;
                for(int to : G[p.first]){
                    if(p.second != to){
                        s.push(P(to, p.first));
                    }
                }
            }
        }
    }
    void BuildPath()
    {
        stack<P> s;
        centroids.emplace_back(-1, -1, 0, 0);
        s.push(P(0, -1));
        pathorder[0] = 0;
        while(!s.empty()) {
            P p = s.top();
            s.pop();
            pathid[p.first] = centroids[pathorder[p.first]].sz;
            for(int to : G[p.first]){
                if(p.second == to) continue;
                if(to == nxpath[p.first]){
                    pathorder[to] = pathorder[p.first];
                }else{
                    pathorder[to] = (int)centroids.size();
                    centroids.emplace_back(pathorder[p.first], pathid[p.first], centroids[pathorder[p.first]].depth + 1,0);
                }
                s.emplace(to, p.first);
            }
            centroids[pathorder[p.first]].sz++;
        }
    }
    void Build_index(){
        int ptr = 0;
        for(auto& centroid : centroids){
            index.push_back(ptr);
            ptr += centroid.sz;
        }
    }
    void add_edge(int x, int y)
    {
        G[x].push_back(y), G[y].push_back(x);
    }
    void Build()
    {
        Buildstsize(); BuildPath(), Build_index();
    }
    P info(int idx)
    {
        return P(pathorder[idx], pathid[idx]);
    }
    int get(int a)
    {
        P p = info(a);
        return (index[p.first] + p.second);
    }
    Centroid &operator[](int k)
    {
        return (centroids[k]);
    }
    void query(int a, int b, const function< void(int, int) > &func)
    {
        int pathidA, pathdepthA, pathidB, pathdepthB;
        tie(pathidA, pathdepthA) = info(a);
        tie(pathidB, pathdepthB) = info(b);
        while(pathidA != pathidB) {
            if(centroids[pathidA].depth > centroids[pathidB].depth) {
                func(index[pathidA], index[pathidA] + pathdepthA + 1);
                tie(pathidA, pathdepthA) = centroids[pathidA].Up();
            }else{
                func(index[pathidB], index[pathidB] + pathdepthB + 1);
                tie(pathidB, pathdepthB) = centroids[pathidB].Up();
            }
        }
        if(pathdepthA > pathdepthB) swap(pathdepthA, pathdepthB);
        func(index[pathidA] + pathdepthA, index[pathidA] + pathdepthB + 1);
    }
    HLdecomposition(int SZ)
    {
        G.resize(SZ);
        stsize.assign(SZ, -1);
        nxpath.resize(SZ);
        pathorder.resize(SZ);
        pathid.resize(SZ);
    }
};


template<typename V> class segtree {
private:
    int n,sz;
    vector<V> node, lazy;

public:
    segtree(vector<V>& u,vector<V>& v) {
        sz = (int)v.size();
        n = 1;
        while(n < sz){
            n *= 2;
        }
        node.resize(2*n-1);
        lazy.resize(2*n-1, id2);
        rep(i,sz){
            node[i+n-1] = v[i];
        }
        for(int i=n-2; i>=0; i--){
            node[i] = opr1(node[i*2+1],node[i*2+2]);
        }
    }
    V id1,id2;
    V opr1(V x,V y){}
    V opr2(V x,V y){}
    void eval(int k, int l, int r) {
        if(lazy[k] != id2) {
            node[k] = opr2(node[k],lazy[k]);
            if(r - l > 1) {
                lazy[2*k+1] = opr1(lazy[2*k+1],lazy[k]);
                lazy[2*k+2] = opr1(lazy[2*k+2],lazy[k]);
            }
            lazy[k] = id2;
        }
    }
    void range(int a, int b, V x, int k=0, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a){
            return;
        }
        if(a <= l && r <= b) {
            lazy[k] = opr1(lazy[k],x);
            eval(k, l, r);
        }else{
            range(a, b, x, 2*k+1, l, (l+r)/2);
            range(a, b, x, 2*k+2, (l+r)/2, r);
            node[k] = opr1(node[2*k+1],node[2*k+2]);
        }
    }
    V query(int a, int b, int k=0, int l=0, int r=-1) {
        if(r < 0) r = n;
        eval(k, l, r);
        if(b <= l || r <= a){
            return id1;
        }
        if(a <= l && r <= b){
            return node[k];
        }
        V vl = query(a, b, 2*k+1, l, (l+r)/2);
        V vr = query(a, b, 2*k+2, (l+r)/2, r);
        return opr1(vl,vr);
    }
    void print()
    {
        rep(i,sz){
            cout << query(i,i+1) << " ";
        }
        cout << endl;
    }
};
