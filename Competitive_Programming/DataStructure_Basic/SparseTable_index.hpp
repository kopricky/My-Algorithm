#include "../header.hpp"

template<typename T> class SparseTable {
private:
    int sz;
    vector<int> logTable;
    vector<vector<pair<T, int> > > table; //最小値のインデックスを保持
public:
    SparseTable(vector<T>& v) : sz((int)v.size()), logTable(sz+1){
        for(int i = 2; i < sz + 1; i++){
            logTable[i] = logTable[i >> 1] + 1;
        }
        table.resize(sz, vector<pair<T, int> >(logTable[sz]+1));
        for(int i = 0; i < sz; i++){
            table[i][0] = make_pair(v[i], i);
        }
        for(int k = 1; (1 << k) <= sz; k++){
            for(int i = 0; i + (1 << k) <= sz; i++){
                table[i][k] = min(table[i][k-1], table[i + (1 << (k-1))][k-1]);
            }
        }
    }
    pair<T, int> query(int l,int r){
        int k = logTable[r-l];
        return min(table[l][k], table[r-(1<<k)][k]);
    }
};
