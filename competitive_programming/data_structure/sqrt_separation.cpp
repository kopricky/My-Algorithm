template<typename T> class sqrt_tree{
public:
    vector<T> data, backet;
    int sq;
    sqrt_tree(vector<T> const& v) : data(v){
        sq = (int)sqrt((int)data.size());
        backet.resize(((int)data.size() + sq - 1) / sq);
        rep(i,data.size()){
            backet[i / sq] += data[i];   //例えば和を持たせる処理
        }
    }
    T sum(int l, int r){    //[l.r)
        T res = 0;
        // 同じバケット内にある
        if(l / sq == (r-1) / sq){
            for(int i = l; i < r; i++){
                res += data[i];
            }
        }else{
            // 完全に被っているバケット
            for(int i = (l + sq - 1) / sq; i < r / sq; i++){
                res += backet[i];
            }
            // 中途半端な左側のバケット
            if(l%sq){
                for(int i = l; i < sq*(l / sq + 1); i++){
                    res += data[i];
                }
            }
            // 中途半端な右側のバケット
            if(r%sq){
                for(int i = r - r % sq; i < r; i++){
                    res += data[i];
                }
            }
        }
        return res;
    }
};
