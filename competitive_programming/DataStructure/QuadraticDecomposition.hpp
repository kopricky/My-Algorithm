#include "../header.hpp"

template<typename T> class QuadraticDecomposition{
public:
    vector<T> data, backet;
    int sz,backet_size,backet_cnt;
    QuadraticDecomposition(vector<T>& v) : data(v){
        sz = (int)data.size();
        backet_size = (int)sqrt(sz);
        backet_cnt = (sz + backet_size - 1) / backet_size;
        backet.resize(backet_cnt);
        rep(i,sz){
            backet[i / backet_size] += data[i];   //例えば和を持たせる処理
        }
    }
    T query(int l, int r){    //[l.r)
        T res = 0;
        // 同じバケット内にある
        if(l / backet_size == (r-1) / backet_size){
            for(int i = l; i < r; i++){
                res += data[i];
            }
        }else{
            // 完全に被っているバケット
            for(int i = (l + backet_size - 1) / backet_size; i < r / backet_size; i++){
                res += backet[i];
            }
            // 中途半端な左側のバケット
            if(l % backet_size){
                for(int i = l; i < backet_size*(l / backet_size + 1); i++){
                    res += data[i];
                }
            }
            // 中途半端な右側のバケット
            if(r % backet_size){
                for(int i = r - r % backet_size; i < r; i++){
                    res += data[i];
                }
            }
        }
        return res;
    }
};
