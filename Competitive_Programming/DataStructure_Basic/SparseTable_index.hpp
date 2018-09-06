#include "../header.hpp"

template<typename T> class SparseTable {
private:
    int sz;
    vector<T> arr;
    vector<int> logTable;
    vector<vector<int> > table; //最小値のインデックスを保持
    bool max_query;
    bool greater(T a, T b){ return (max_query) ? (a > b) : (a < b); }
public:
    SparseTable(vector<T>& v, bool max_query_=false) : sz((int)v.size()), arr(v),
        logTable(sz+1), max_query(max_query_){
        for(int i = 2; i < sz + 1; i++){
            logTable[i] = logTable[i >> 1] + 1;
        }
        table.resize(sz,vector<T>(logTable[sz]+1));
        for(int i = 0; i < sz; i++){
            table[i][0] = i;
        }
        for(int k = 1; (1 << k) <= sz; k++){
            for(int i = 0; i + (1 << k) <= sz; i++){
                int s = table[i][k-1];
                int t = table[i + (1 << (k-1))][k-1];
                if(greater(arr[s], arr[t])){
                    table[i][k] = s;
                }else{
                    table[i][k] = t;
                }
            }
        }
    }
    //
    pair<T,int> query(int l,int r){
        int k = logTable[r-l];
        if(greater(arr[table[l][k]], arr[table[r-(1<<k)][k]])){
            return make_pair(arr[table[l][k]],table[l][k]);
        }else{
            return make_pair(arr[table[r-(1<<k)][k]],table[r-(1<<k)][k]);
        }
    }
};
