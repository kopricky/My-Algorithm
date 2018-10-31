#include "../header.hpp"

#define EPS 1e-10

// max c * x s.t. A*x <= b, x >= 0
class Simplex {
private:
    using Arr = vector<double>;
    using Mat = vector<vector<double> >;
    int* index;
    double** a;
    int row, column, L;

    void Set(const Mat& A, const Arr& b, const Arr& c){
        infinity = none = false;
        row = A.size(),column = A[0].size() + 1;
        index = new int[row + column];
        int i, j;
        for(i = 0; i < row + column; i++) index[i] = i;
        L = row;
        a = new double*[row + 2];
        for(i = 0; i < row + 2; i++) a[i] = new double[column + 4];
        for(i = 0; i < row; i++){
            for(j = 0; j < column - 1; j++) a[i][j] = -A[i][j];
            a[i][column-1] = 1;
            a[i][column] = b[i];
            if(a[L][column] > a[i][column]) L = i;
        }
        for(j = 0; j < column - 1; j++) a[row][j] = c[j];
        a[row+1][column-1] = -1;
    }

    void solve(){
        int E, i, j;
        for(E = column - 1;;){
    	    if(L < row){
                swap(index[E], index[L + column]);
                a[L][E] = 1 / a[L][E];
                for(j = 0; j < column + 1; j++) if(j != E) a[L][j] *= -a[L][E];
                for(i = 0; i < row + 2; i++){
                    if(abs(a[i][E]) < EPS || i == L) continue;
                    for(j = 0; j < column + 1; j += 4){
                        if(j != E) a[i][j] += a[i][E] * a[L][j];
                        if(j+1 != E) a[i][j+1] += a[i][E] * a[L][j+1];
                        if(j+2 != E) a[i][j+2] += a[i][E] * a[L][j+2];
                        if(j+3 != E) a[i][j+3] += a[i][E] * a[L][j+3];
                    }
                    a[i][E] *= a[L][E];
                }
    	    }
    	    E = -1;
            // double pre = EPS;
    	    for(j = 0; j < column; j++){
                if(E < 0 || index[E] > index[j]){
                    if(a[row + 1][j] > EPS || (abs(a[row + 1][j]) < EPS && a[row][j] > EPS)) E = j;
                    // if(a[row + 1][j] > pre) E = j, pre = a[row + 1][j];
                    // else if(abs(a[row + 1][j]) < EPS && a[row][j] > pre) E = j, pre = a[row][j];
                }
            }
    	    if(E < 0) break;
            L = -1;
            for(i = 0; i < row; i++){
                if(a[i][E] < -EPS){
                    if(L < 0) L = i;
                    else if(a[L][column] / a[L][E] - a[i][column] / a[i][E] < -EPS) L = i;
                    else if(a[L][column] / a[L][E] - a[i][column] / a[i][E] < EPS && index[L] > index[i]) L = i;
                    // if(L < 0 || a[L][column] / a[L][E] - a[i][column] / a[i][E] < EPS) L = i;
                }
            }
            if(L < 0){
                infinity = true;
                return;
            }
    	}
    	if(a[row + 1][column] < -EPS){
    	    none = true;
    	    return;
    	}
    	x.assign(column - 1, 0);
        for(i = 0; i < row; i++){
            if(index[column + i] < column - 1) x[index[column + i]] = a[i][column];
        }
    	ans = a[row][column];
    }
public:
    bool infinity, none;
    double ans;
    Arr x;
    Simplex(const Mat& A, const Arr& b, const Arr& c){
        Set(A,b,c);
        solve();
    }
};
