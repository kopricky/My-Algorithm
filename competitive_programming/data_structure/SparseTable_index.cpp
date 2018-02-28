template<typename T> class SparseTable {
private:
    vector<T> Arr;
    vector<int> LogTable;
    vector<vector<int> > Table; //最小値のインデックスを保持
    int sz;
public:
    SparseTable(vector<T>& v){
        sz = (int)v.size();
        Arr = v;
        LogTable.resize(sz+1);
        for(int i = 2; i < sz + 1; i++){
            LogTable[i] = LogTable[i >> 1] + 1;
        }
        Table.resize(sz,vector<T>(LogTable[sz]+1));
        rep(i,sz){
            Table[i][0] = i;
        }
        for(int k = 1; (1 << k) <= sz; k++){
            for(int i = 0; i + (1 << k) <= sz; i++){
                int s = Table[i][k-1];
                int t = Table[i + (1 << (k-1))][k-1];
                if(Arr[s] < Arr[t]){
                    Table[i][k] = s;
                }else{
                    Table[i][k] = t;
                }
            }
        }
    }
    pair<T,int> query(int l,int r){
        int k = LogTable[r-l];
        if(Arr[Table[l][k]] < Arr[Table[r-(1<<k)][k]]){
            return make_pair(Arr[Table[l][k]],Table[l][k]);
        }else{
            return make_pair(Arr[Table[r-(1<<k)][k]],Table[r-(1<<k)][k]);
        }
    }
};
