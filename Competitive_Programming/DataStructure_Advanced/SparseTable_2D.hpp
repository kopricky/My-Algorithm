#include "../header.hpp"

template<typename T> class SparseTable_2D {
private:
    const int R, C;
    vector<int> LogTable;
    T**** Table;
public:
    SparseTable_2D(const vector<vector<T> >& v) : R((int)v.size()), C((int)v[0].size()), LogTable(max(R, C) + 1){
        for(int i = 2; i <= max(R, C); ++i){
            LogTable[i] = LogTable[i >> 1] + 1;
        }
        Table = new T***[LogTable[R] + 1];
        for(int k = 0; k <= LogTable[R] ; ++k){
            Table[k] = new T**[LogTable[C] + 1];
            for(int l = 0; l <= LogTable[C]; ++l){
                Table[k][l] = new T*[R];
                for(int i = 0; i < R; ++i){
                    Table[k][l][i] = new T[C];
                }
            }
        }
        for(int i = 0; i < R; ++i){
            for(int j = 0; j < C; ++j){
                Table[0][0][i][j] = v[i][j];
            }
        }
        for(int k = 1; (1 << k) <= R; ++k){
            for(int i = 0; i + (1 << k) <= R; ++i){
                for(int j = 0; j < C; ++j){
                    Table[k][0][i][j] = min(Table[k - 1][0][i][j], Table[k - 1][0][i + (1 << (k - 1))][j]);
                }
            }
        }
        for(int k = 0; (1 << k) <= R; ++k){
            for(int l = 1; (1 << l) <= C; ++l){
                for(int i = 0; i + (1 << k) <= R; ++i){
                    for(int j = 0; j + (1 << l) <= C; ++j){
                        Table[k][l][i][j] = min(Table[k][l - 1][i][j], Table[k][l - 1][i][j + (1 << (l - 1))]);
                    }
                }
            }
        }
    }
    ~SparseTable_2D(){
        for(int i = 0; i <= LogTable[R]; ++i){
            for(int j = 0; j <= LogTable[C]; ++j){
                for(int k = 0; k < R; ++k){
                    delete[] Table[i][j][k];
                }
                delete[] Table[i][j];
            }
            delete[] Table[i];
        }
        delete[] Table;
    }
    // [lx,rx) × [ly,ry) の長方形領域の min に答える
    T query(const int lx, const int ly, const int rx, const int ry){
        const int kx = LogTable[rx - lx];
        const int ky = LogTable[ry - ly];
        return min({Table[kx][ky][lx][ly], Table[kx][ky][rx - (1 << kx)][ly], Table[kx][ky][lx][ry - (1 << ky)], Table[kx][ky][rx - (1 << kx)][ry - (1 << ky)]});
    }
};
