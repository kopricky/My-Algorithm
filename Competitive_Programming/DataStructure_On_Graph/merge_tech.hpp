#include "../header.hpp"

// "データ構造をマージする一般的なテク"と呼ばれるもの
// 大きい方に小さい方を merge すれば全体で O(n log n) という話

class MergeTech{
private:
    int sz;
public:
    vector<int> par;              //所属しているグループ番号(par[x] は union find の find(x) に対応)
    vector<vector<int> > group;   //group[x]: グループ x に含まれる数
    MergeTech(){}
    MergeTech(int node_size) : sz(node_size), par(sz), group(sz){
        for(int i = 0; i < sz; i++){
            par[i] = i, group[i].push_back(i);
        }
    }
    bool same(int a,int b){ return par[a] == par[b]; }
    void unite(int a,int b){
        if(same(a,b)) return;
        if(group[par[a]].size() < group[par[b]].size()) swap(a,b);
        int ga = par[a], gb = par[b];
        for(int u : group[gb]){
            par[u] = ga;
        }
        group[ga].insert(group[ga].end(),group[gb].begin(),group[gb].end());
        group[gb].clear();
    }
};
