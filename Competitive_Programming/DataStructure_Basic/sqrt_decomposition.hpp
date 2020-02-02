#include "../header.hpp"

//平方分割

template<typename T> class SqrtDecomposition{
public:
    vector<T> data;
    int sz,backet_size,backet_cnt;
    vector<T> backet;
    SqrtDecomposition(vector<T>& v) : data(v), sz((int)data.size()),
        backet_size((int)sqrt(sz)), backet_cnt(sz / backet_size + 1), backet(backet_cnt, 0){
        for(int i = 0; i < sz; i++){
            backet[i / backet_size] += data[i];  //例えば和を持たせる処理
        }
    }
    // [l.r)
    T query(int l, int r){
        T res = 0;
        int backet_index, data_index;
        // 同じバケット内にある
        if(l / backet_size == (r-1) / backet_size){
            for(data_index = l; data_index < r; data_index++){
                res += data[data_index];
            }
        }else{
            // 完全に被っているバケット
            for(backet_index = (l + backet_size - 1) / backet_size; backet_index < r / backet_size; backet_index++){
                res += backet[backet_index];
            }
            // 中途半端な左側のバケット
            if(l % backet_size){
                for(data_index = l; data_index < backet_size*(l / backet_size + 1); data_index++){
                    res += data[data_index];
                }
            }
            // 中途半端な右側のバケット
            if(r % backet_size){
                for(data_index = r - r % backet_size; data_index < r; data_index++){
                    res += data[data_index];
                }
            }
        }
        return res;
    }
};
