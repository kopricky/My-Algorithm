#include "../header.hpp"

//乗算がMODの場合はコメントアウトの方を使う

template<typename T> class mat : public vector<vector<T> > {
public:
    int r, c;    //行,列
    mat(int n, int m) : r(n), c(m){
        for(int i = 0; i < r; i++){
            this->push_back(vector<T>(c,0));
        }
    }
    mat operator*(const mat& that){
        mat<T> X(this->r,that.c);
        for(int i = 0; i < this->r; i++){
            for(int k = 0; k < this->c; k++){
                for(int j = 0; j < that.c; j++){
                    X[i][j] += (*this)[i][k]*that[k][j];
                }
            }
        }
        return X;
    }
    // mat operator*(const mat& that){
    //     mat<T> X(this->r, that.c);
    //     for(int i = 0; i < this->r; i++){
    //         for(int k = 0; k < this->c; k++){
    //             for(int j = 0; j < that.c; j++){
    //                 X[i][j] = (X[i][j] + (*this)[i][k]*that[k][j]) % MOD;
    //             }
    //         }
    //     }
    //     return X;
    // }
    void print(){
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c-1; j++){
                cout << (*this)[i][j] << ",";
            }
            cout << (*this)[i][c-1] << endl;
        }
    }
};

template<typename T> mat<T> pow(mat<T> A,long long cnt)
{
    int n = A.r;
	mat<T> B(n,n);
	for(int i = 0; i < n; i++){
		B[i][i] = 1;
	}
	while(cnt>0){
		if(cnt & 1){
			B = B*A;
		}
		A = A*A;
		cnt >>= 1;
	}
	return B;
}

template<typename T> mat<T> mod_pow(mat<T> A,long long cnt)
{
    int n = A.r;
	mat<T> B(n,n);
	for(int i = 0; i < n; i++){
		B[i][i] = 1;
	}
	while(cnt>0){
		if(cnt & 1){
			B = B*A;
		}
		A = A*A;
		cnt >>= 1;
	}
	return B;
}
