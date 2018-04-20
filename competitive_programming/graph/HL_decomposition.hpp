#include "../header.hpp"

struct HLdecomposition
{
    using P = pair<int,int>;
    struct Centroid
    {
        int parid, pardepth, depth;
        vector<int> node;
        Centroid(int idx, int dep, int deep) : parid(idx), pardepth(dep), depth(deep) {}
        int size()
        {
            return (int)node.size();
        }
        int &operator[](int k)
        {
            return node[k];
        }
        P Up()
        {
            return P(parid, pardepth);
        }
    };
    vector<vector<int> > G;
    vector<int> stsize, nxpath;
    vector<int> pathorder, pathid;
    vector<Centroid> centroids;
    void Buildstsize()
    {
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
                for(auto &to : G[p.first]) {
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
        centroids.emplace_back(-1, -1, 0);
        s.push(P(0, -1));
        pathorder[0] = 0;
        while(!s.empty()) {
            P p = s.top();
            s.pop();
            pathid[p.first] = (int)centroids[pathorder[p.first]].size();
            for(int to : G[p.first]){
                if(p.second == to) continue;
                if(to == nxpath[p.first]){
                    pathorder[to] = pathorder[p.first];
                }else{
                    pathorder[to] = (int)centroids.size();
                    centroids.emplace_back(pathorder[p.first], pathid[p.first], centroids[pathorder[p.first]].depth + 1);
                }
                s.emplace(to, p.first);
            }
            centroids[pathorder[p.first]].node.emplace_back(p.first);
        }
    }
    void add_edge(int x, int y)
    {
        G[x].push_back(y), G[y].push_back(x);
    }
    void Build()
    {
        Buildstsize(); BuildPath();
    }
    int size()
    {
        return (int)centroids.size();
    }
    P info(int idx)
    {
        return P(pathorder[idx], pathid[idx]);
    }
    Centroid &operator[](int k)
    {
        return (centroids[k]);
    }
    int LCA(int a, int b)
    {
        int pathordA, pathidA, pathordB, pathidB;
        tie(pathordA, pathidA) = info(a);
        tie(pathordB, pathidB) = info(b);
        while(pathordA != pathordB) {
            if(centroids[pathordA].depth > centroids[pathordB].depth) {
                tie(pathordA, pathidA) = centroids[pathordA].Up();
            } else {
                tie(pathordB, pathidB) = centroids[pathordB].Up();
            }
        }
        if(pathidA > pathidB) swap(pathidA, pathidB);
        return centroids[pathordA][pathidA];
    }
    void query(int a, int b, const function< void(int, int, int) > &func)
    {
        int pathordA, pathidA, pathordB, pathidB;
        tie(pathordA, pathidA) = info(a);
        tie(pathordB, pathidB) = info(b);
        while(pathordA != pathordB) {
            if(centroids[pathordA].depth > centroids[pathordB].depth) {
                func(pathordA, 0, pathidA + 1);
                tie(pathordA, pathidA) = centroids[pathordA].Up();
            }else{
                func(pathordB, 0, pathidB + 1);
                tie(pathordB, pathidB) = centroids[pathordB].Up();
            }
        }
        if(pathidA > pathidB) swap(pathidA, pathidB);
        func(pathordA, pathidA, pathidB + 1);
    }
    HLdecomposition(int V) : G(V), stsize(V, -1), nxpath(V), pathorder(V), pathid(V){}
};
