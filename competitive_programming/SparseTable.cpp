template<typename T> class SparseTable {
private:
    vector<int> LogTable;
    vector<vector<T> > Table; //最小値を保持
    int sz;
public:
    SparseTable(vector<T>& v){
        sz = (int)v.size();
        LogTable.resize(sz+1);
        for(int i = 2; i < sz + 1; i++){
            LogTable[i] = LogTable[i >> 1] + 1;
        }
        Table.resize(sz,vector<T>(LogTable[sz]+1));
        rep(i,sz){
            Table[i][0] = v[i];
        }
        for(int k = 1; (1 << k) <= sz; k++){
            for(int i = 0; i + (1 << k) <= sz; i++){
                Table[i][k] = min(Table[i][k-1],Table[i + (1 << (k-1))][k-1]);
            }
        }
    }
    T query(int l,int r){
        int k = LogTable[r-l];
        return min(Table[l][k],Table[r-(1<<k)][k]);
    }
};
