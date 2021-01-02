#include "../header.hpp"

template<typename T> class SparseTable {
private:
    int sz;
    vector<int> LogTable;
    vector<vector<T> > Table; //最小値を保持
public:
    SparseTable(const vector<T>& v) : sz((int)v.size()), LogTable(sz + 1){
        for(int i = 2; i < sz + 1; i++){
            LogTable[i] = LogTable[i >> 1] + 1;
        }
        Table.resize(LogTable[sz]+1, vector<T>(sz));
        rep(i,sz){
            Table[0][i] = v[i];
        }
        for(int k = 1; (1 << k) <= sz; k++){
            for(int i = 0; i + (1 << k) <= sz; i++){
                Table[k][i] = min(Table[k-1][i], Table[k-1][i + (1 << (k-1))]);
            }
        }
    }
    T query(const int l, const int r){
        const int k = LogTable[r-l];
        return min(Table[k][l], Table[k][r-(1<<k)]);
    }
};
