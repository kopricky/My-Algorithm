#include "../header.hpp"

struct HLdecomposition{
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
            func(index[pathidA], index[pathidA] + pathdepthA + 1);
            if(centroids[pathidA].depth > centroids[pathidB].depth) {
                tie(pathidA, pathdepthA) = centroids[pathidA].Up();
            }else{
                func(index[pathidB], index[pathidB] + pathdepthB + 1);
                tie(pathidB, pathdepthB) = centroids[pathidB].Up();
            }
        }
        func(index[pathidA] + pathdepthA, index[pathidA] + pathdepthB + 1);
    }
    HLdecomposition(int V) : G(V), stsize(V, -1), nxpath(V), pathorder(V), pathid(V){}
};
