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
        this->r = n,this->c = m;
        rep(i,n){
            this->push_back(vector<T>(m,val));
        }
    }
    mat operator+(const mat& another){
        if(this->r != another.r && this->c != another.c){
            cout << "足し算失敗(サイズ不一致)" << endl;
            exit(1);
        }
        mat<T> X(this->r,this->c);
        rep(i,this->r){
            rep(j,this->c){
                X[i][j] = (*this)[i][j] + another[i][j];
            }
        }
        return X;
    }
    mat operator+(const T val){
        mat<T> X(this->r,this->c);
        rep(i,this->r){
            rep(j,this->c){
                X[i][j] = (*this)[i][j] + val;
            }
        }
        return X;
    }
    mat operator-(const mat& another){
        if(this->r != another.r && this->c != another.c){
            cout << "引き算失敗(サイズ不一致)" << endl;
            exit(1);
        }
        mat<T> X(this->r,this->c);
        rep(i,this->r){
            rep(j,this->c){
                X[i][j] = (*this)[i][j] - another[i][j];
            }
        }
        return X;
    }
    mat operator-(const T val){
        mat<T> X(this->r,this->c);
        rep(i,this->r){
            rep(j,this->c){
                X[i][j] = (*this)[i][j] - val;
            }
        }
        return X;
    }
    vector<T> operator*(const vector<T>& another){
        if(this->c != another.size()){
            cout << "掛け算失敗(サイズ不一致)" << endl;
            exit(1);
        }
        vector<T> vec(this->r,0);
        rep(i,this->r){
            rep(j,this->c){
                vec[i] += (*this)[i][j] * another[j];
            }
        }
        return vec;
    }
    mat operator*(const mat& another){
        if(this->c != another.r){
            cout << "掛け算失敗(サイズ不一致)" << endl;
            exit(1);
        }
        mat<T> X(this->r,another.c);
        rep(i,this->r){
            rep(k,this->c){
                rep(j,another.c){
                    X[i][j] += (*this)[i][k]*another[k][j];
                }
            }
        }
        return X;
    }
    mat operator-(){
        mat<T> X(this->r,this->c);
        rep(i,this->r){
            rep(j,this->c){
                X[i][j] = -(*this)[i][j];
            }
        }
        return X;
    }
    int rank(){
        int n = this->r;
        int res = 0;
        mat B(n,n);
        rep(i,n){
            rep(j,n){
                B[i][j] = (*this)[i][j];
            }
        }
        rep(i,n){
            int pivot = res;
            for(int j=i;j<n;j++){
                if(abs(B[j][i]) > abs(B[pivot][i])){
                    pivot = j;
                }
            }
            if(abs(B[pivot][i]) < EPS){
                continue;
            }
            swap(B[res],B[pivot]);
            for(int j=i+1;j<n;j++){
                B[i][j] /= B[i][i];
            }
            for(int j=i+1;j<n;j++){
                for(int k=i+1;k<n;k++){
                    B[j][k] -= B[j][i] * B[i][k];
                }
            }
            res++;
        }
        return res;
    }
    T det(){
        if(this->r != this->c){
            cout << "正方行列でない(行列式定義不可)" << endl;
            exit(1);
        }
        T ans = 1;
        int n = this->r;
        mat B(n,n);
        rep(i,n){
            rep(j,n){
                B[i][j] = (*this)[i][j];
            }
        }
        rep(i,n) {
            for(int j=i+1;j<n;j++){
                for (; B[j][i] != 0; ans = -ans) {
                    T r = B[i][i] / B[j][i];
                    for(int k=i;k<n;k++) {
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
        if(this->r != this->c){
            cout << "正方行列でない(逆行列定義不可)" << endl;
            exit(1);
        }
        int n = this->r;
        mat B(n,2*n);
        rep(i,n){
            rep(j,n){
                B[i][j] = (*this)[i][j];
            }
        }
        rep(i,n){
            B[i][n+i] = 1;
        }
        rep(i,n){
            int pivot = i;
            for(int j=i;j<n;j++){
                if(abs(B[j][i]) > abs(B[pivot][i])){
                    pivot = j;
                }
            }
            if(abs(B[pivot][i]) < EPS){
                cout << "解なしor不定" << endl;
                exit(1);
            }
            swap(B[i],B[pivot]);
            for(int j=i+1;j<=2*n;j++){
                B[i][j] /= B[i][i];
            }
            rep(j,n){
                if(i != j){
                    for(int k=i+1;k<=2*n;k++){
                        B[j][k] -= B[j][i] * B[i][k];
                    }
                }
            }
        }
        mat res(n,n);
        rep(i,n){
            rep(j,n){
                res[i][j] = B[i][n+j];
            }
        }
        return res;
    }
    void print(){
        rep(i,this->r){
            rep(j,(this->c)-1){
                cout << (*this)[i][j] << ",";
            }
            cout << (*this)[i][(this->c)-1] << endl;
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
    rep(i,n){
        rep(j,n){
            B[i][j] = A[i][j];
        }
    }
    rep(i,n){
        B[i][n] = b[i];
    }
    rep(i,n){
        int pivot = i;
        for(int j=i;j<n;j++){
            if(abs(B[j][i]) > abs(B[pivot][i])){
                pivot = j;
            }
        }
        if(abs(B[pivot][i]) < EPS){
            cout << "解なしor不定" << endl;
            exit(1);
        }
        swap(B[i],B[pivot]);
        for(int j=i+1;j<=n;j++){
            B[i][j] /= B[i][i];
        }
        rep(j,n){
            if(i != j){
                for(int k=i+1;k<=n;k++){
                    B[j][k] -= B[j][i] * B[i][k];
                }
            }
        }
    }
    vector<T> res(n);
    rep(i,n){
        res[i] = B[i][n];
    }
    return res;
}

template<typename T> mat<T> pow(mat<T> A,ll cnt)
{
    if(A.row() != A.column()){
        cout << "累乗不可" << endl;
    }
    int n = A.row();
	mat<T> B(n,n);
	rep(i,n){
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

template<typename T> mat<T> mod_mul(mat<T>& A,mat<T>& B)
{
    if(A.column() != B.row()){
        cout << "掛け算失敗(サイズ不一致)" << endl;
        exit(1);
    }
    mat<T> X(A.row(),B.column());
    rep(i,A.row()){
        rep(k,A.column()){
            rep(j,B.column()){
                X[i][j] = (X[i][j] + A[i][k]*B[k][j]) % MOD;
            }
        }
    }
    return X;
}

template<typename T> mat<T> mod_pow(mat<T> A,ll cnt)
{
    if(A.row() != A.column()){
        cout << "累乗不可" << endl;
    }
    int n = A.row();
	mat<T> B(n,n);
	rep(i,n){
		B[i][i] = 1;
	}
	while(cnt>0){
		if(cnt & 1){
			B = mod_mul(B,A);
		}
		A = mod_mul(A,A);
		cnt >>= 1;
	}
	return B;
}
