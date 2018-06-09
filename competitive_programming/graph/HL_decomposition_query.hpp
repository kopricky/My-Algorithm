#include "../header.hpp"

class HLdecomposition{
private:
    struct Centroid{
        int parid, pardepth, depth, sz;
        Centroid(int idx, int dep, int deep, int size) : parid(idx), pardepth(dep), depth(deep), sz(size){}
    };
    int V;
    vector<vector<int> > G;
    vector<int> stsize, nxpath, pathorder, pathid, index;
    vector<Centroid> centroids;
    void BuildStsize(int u, int p){
        stsize[u] = 1;
        for(int v : G[u]){
            if(v == p) continue;
            BuildStsize(v, u);
            stsize[u] += stsize[v];
            if(nxpath[u] == -1 || stsize[v] > stsize[nxpath[u]]){
                nxpath[u] = v;
            }
        }
    }
    void BuildPath(int u, int p){
        pathid[u] = centroids[pathorder[u]].sz - 1;
        for(int v : G[u]){
            if(v == p) continue;
            if(v == nxpath[u]){
                pathorder[v] = pathorder[u];
                centroids[pathorder[u]].sz++;
            }else{
                pathorder[v] = (int)centroids.size();
                centroids.emplace_back(pathorder[u], pathid[u], centroids[pathorder[u]].depth + 1, 1);
            }
            BuildPath(v, u);
        }
    }
    void BuildIndex(){
        int ptr = 0;
        for(auto& centroid : centroids){
            index.push_back(ptr);
            ptr += centroid.sz;
        }
    }

public:
    void add_edge(int u, int v){
        G[u].push_back(v), G[v].push_back(u);
    }
    void build(){
        BuildStsize(0, -1), BuildPath(0, -1), BuildIndex();
    }
    int get(int a){
        return index[pathorder[a]] + pathid[a];
    }
    void query(int a, int b, const function< void(int, int) > &func)
    {
        int pathidA = pathorder[a], pathdepthA = pathid[a];
        int pathidB = pathorder[b], pathdepthB = pathid[b];
        while(pathidA != pathidB) {
            if(centroids[pathidA].depth > centroids[pathidB].depth) {
                func(index[pathidA], index[pathidA] + pathdepthA + 1);
                pathdepthA = centroids[pathidA].pardepth, pathidA = centroids[pathidA].parid;
            }else{
                func(index[pathidB], index[pathidB] + pathdepthB + 1);
                pathdepthB = centroids[pathidB].pardepth, pathidB = centroids[pathidB].parid;
            }
        }
        if(pathdepthA > pathdepthB) swap(pathdepthA, pathdepthB);
        func(index[pathidA] + pathdepthA, index[pathidA] + pathdepthB + 1);
    }
    HLdecomposition(int node_size) : V(node_size), G(V), stsize(V, -1), nxpath(V, -1),
        pathorder(V, -1), pathid(V, -1){
        pathorder[0] = 0, centroids.emplace_back(-1, -1, 0, 1);
    }
};
