#include "../header.hpp"

template<typename T> class SparseTable {
private:
    int sz;
    vector<int> LogTable;
    vector<vector<pair<T, int> > > Table; //最小値を保持
public:
    SparseTable(const vector<T>& v) : sz((int)v.size()), LogTable(sz + 1){
        for(int i = 2; i < sz + 1; i++){
            LogTable[i] = LogTable[i >> 1] + 1;
        }
        Table.resize(LogTable[sz]+1, vector<pair<T, int> >(sz));
        rep(i,sz){
            Table[0][i] = make_pair(v[i], i);
        }
        for(int k = 1; (1 << k) <= sz; k++){
            for(int i = 0; i + (1 << k) <= sz; i++){
                Table[k][i] = min(Table[k-1][i], Table[k-1][i + (1 << (k-1))]);
            }
        }
    }
    pair<T, int> query(int l, int r){
        int k = LogTable[r-l];
        return min(Table[k][l], Table[k][r-(1<<k)]);
    }
};