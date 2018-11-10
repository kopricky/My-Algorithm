#include "../header.hpp"
#define EPS 1e-10
#define MOD 1000000007

template<typename T> class mat : public vector<vector<T> > {
private:
    int r,c;    //行,列
public:
    int row() const {
        return r;
    }
    int column() const {
        return c;
    }
    mat(int n,int m,T val = 0){
        r = n, c = m;
        for(int i = 0; i < n; i++){
            this->push_back(vector<T>(m,val));
        }
    }
    mat operator+(const mat& another){
        if(r != another.r && c != another.c){
            cout << "足し算失敗(サイズ不一致)" << endl;
            exit(1);
        }
        mat<T> X(r, c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = (*this)[i][j] + another[i][j];
            }
        }
        return X;
    }
    mat operator+(const T val){
        mat<T> X(r,c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = (*this)[i][j] + val;
            }
        }
        return X;
    }
    mat operator-(const mat& another){
        if(r != another.r && c != another.c){
            cout << "引き算失敗(サイズ不一致)" << endl;
            exit(1);
        }
        mat<T> X(r,c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = (*this)[i][j] - another[i][j];
            }
        }
        return X;
    }
    mat operator-(const T val){
        mat<T> X(r,c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = (*this)[i][j] - val;
            }
        }
        return X;
    }
    vector<T> operator*(const vector<T>& another){
        if(c != (int)another.size()){
            cout << "掛け算失敗(サイズ不一致)" << endl;
            exit(1);
        }
        vector<T> vec(r,0);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                vec[i] += (*this)[i][j] * another[j];
            }
        }
        return vec;
    }
    mat operator*(const mat& another){
        if(c != another.r){
            cout << "掛け算失敗(サイズ不一致)" << endl;
            exit(1);
        }
        mat<T> X(r,another.c);
        for(int i = 0; i < r; i++){
            for(int k = 0; k < c; k++){
                for(int j = 0; j < (another.c); j++){
                    X[i][j] += (*this)[i][k]*another[k][j];
                }
            }
        }
        return X;
    }
    mat operator-(){
        mat<T> X(r,c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = -(*this)[i][j];
            }
        }
        return X;
    }
    int rank(){
        int res = 0;
        mat B(r, c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                B[i][j] = (*this)[i][j];
            }
        }
        for(int i = 0; i < c; i++){
            if(res == r) return res;
            int pivot = res;
            for(int j = res + 1; j < r; j++){
                if(abs(B[j][i]) > abs(B[pivot][i])){
                    pivot = j;
                }
            }
            if(abs(B[pivot][i]) < EPS) continue;
            swap(B[pivot],B[res]);
            for(int j = i + 1; j < c; j++){
                B[res][j] /= B[res][i];
            }
            for(int j = res + 1; j < r; j++){
                for(int k = i + 1; k < c; k++){
                    B[j][k] -= B[res][k]*B[j][i];
                }
            }
            res++;
        }
        return res;
    }
    T det(){
        if(r != c){
            cout << "正方行列でない(行列式定義不可)" << endl;
            exit(1);
        }
        T ans = 1;
        mat B(r, r);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < r; j++){
                B[i][j] = (*this)[i][j];
            }
        }
        for(int i = 0; i < r; i++){
            for(int j = i + 1; j < r; j++){
                for (; B[j][i] != 0; ans = -ans) {
                    T r = B[i][i] / B[j][i];
                    for(int k = i; k < r; k++) {
                        T t = B[i][k] - r * B[j][k];
                        B[i][k] = B[j][k];
                        B[j][k] = t;
                    }
                }
           }
           ans *= B[i][i];
       }
       return ans;
    }
    mat inverse(){
        if(r != c){
            cout << "正方行列でない(逆行列定義不可)" << endl;
            exit(1);
        }
        mat B(r, 2*r);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < r; j++){
                B[i][j] = (*this)[i][j];
            }
        }
        for(int i = 0; i < r; i++){
            B[i][r+i] = 1;
        }
        for(int i = 0; i < r; i++){
            int pivot = i;
            for(int j = i; j < r; j++){
                if(abs(B[j][i]) > abs(B[pivot][i])){
                    pivot = j;
                }
            }
            if(abs(B[pivot][i]) < EPS){
                cout << "解なしor不定" << endl;
                exit(1);
            }
            swap(B[i],B[pivot]);
            for(int j = i + 1; j < 2*r; j++){
                B[i][j] /= B[i][i];
            }
            for(int j = 0; j < r; j++){
                if(i != j){
                    for(int k = i + 1; k < 2*r; k++){
                        B[j][k] -= B[j][i] * B[i][k];
                    }
                }
            }
        }
        mat res(r, r);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < r; j++){
                res[i][j] = B[i][r+j];
            }
        }
        return res;
    }
    void print(){
        for(int i = 0; i < r; i++){
            for(int j = 0; j < (c-1); j++){
                cout << (*this)[i][j] << ",";
            }
            cout << (*this)[i][c-1] << endl;
        }
    }
};

template<typename T> vector<T> eq_solve(const mat<T>& A,const vector<T>& b){
    if(A.row() != A.column()){
        cout << "正方行列でない(解なしor不定)" << endl;
        exit(1);
    }
    int n = A.row();
    mat<T> B(n,n+1);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            B[i][j] = A[i][j];
        }
    }
    for(int i = 0; i < n; i++){
        B[i][n] = b[i];
    }
    for(int i = 0; i < n; i++){
        int pivot = i;
        for(int j = i; j < n;j++){
            if(abs(B[j][i]) > abs(B[pivot][i])){
                pivot = j;
            }
        }
        if(abs(B[pivot][i]) < EPS){
            cout << "解なしor不定" << endl;
            exit(1);
        }
        swap(B[i], B[pivot]);
        for(int j = i + 1; j <= n; j++){
            B[i][j] /= B[i][i];
        }
        for(int j = 0; j < n; j++){
            if(i != j){
                for(int k = i + 1; k <= n; k++){
                    B[j][k] -= B[j][i] * B[i][k];
                }
            }
        }
    }
    vector<T> res(n);
    for(int i = 0; i < n; i++){
        res[i] = B[i][n];
    }
    return res;
}

template<typename T> mat<T> pow(mat<T> A,long long cnt)
{
    if(A.row() != A.column()){
        cout << "累乗不可" << endl;
    }
    int n = A.row();
    mat<T> B(n,n);
    for(int i = 0; i < n; i++){
        B[i][i] = 1;
    }
    while(cnt>0){
        if(cnt & 1) B = B*A;
        A = A*A;
        cnt >>= 1;
    }
    return B;
}

template<typename T> mat<T> mod_mul(mat<T>& A,mat<T>& B)
{
    if(A.column() != B.row()){
        cout << "掛け算失敗(サイズ不一致)" << endl;
        exit(1);
    }
    mat<T> X(A.row(),B.column());
    for(int i = 0; i < A.row(); i++){
        for(int k = 0; k < A.column(); k++){
            for(int j = 0; j < B.column(); j++){
                X[i][j] = (X[i][j] + A[i][k]*B[k][j]) % MOD;
            }
        }
    }
    return X;
}

template<typename T> mat<T> mod_pow(mat<T> A,long long cnt)
{
    if(A.row() != A.column()){
        cout << "累乗不可" << endl;
    }
    int n = A.row();
    mat<T> B(n,n);
    for(int i = 0; i < n; i++){
        B[i][i] = 1;
    }
    while(cnt>0){
		if(cnt & 1) B = mod_mul(B,A);
        A = mod_mul(A,A);
        cnt >>= 1;
    }
    return B;
}
