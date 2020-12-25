#include "../header.hpp"

#define MOD 1000000007

template <unsigned int mod>
class ModInt {
private:
    unsigned int v;
    static unsigned int norm(const unsigned int& x){ return x < mod ? x : x - mod; }
    static ModInt make(const unsigned int& x){ ModInt m; return m.v = x, m; }
    static ModInt inv(const ModInt& x){ return make(inverse(x.v, mod)); }
    static unsigned int inverse(int a, int m){
        int u[] = {a, 1, 0}, v[] = {m, 0, 1}, t;
        while(*v){
            t = *u / *v;
            swap(u[0] -= t * v[0], v[0]), swap(u[1] -= t * v[1], v[1]), swap(u[2] -= t * v[2], v[2]);
        }
        return (u[1] % m + m) % m;
    }

public:
    ModInt() : v{0}{}
    ModInt(const long long val) : v{norm(val % mod + mod)} {}
    ModInt(const ModInt<mod>& n) : v{n()} {}
    explicit operator bool() const noexcept { return v != 0; }
    bool operator!() const noexcept { return !static_cast<bool>(*this); }
    ModInt& operator=(const ModInt& n){ return v = n(), (*this); }
    ModInt& operator=(const long long val){ return v = norm(val % mod + mod), (*this); }
    ModInt operator+() const { return *this; }
    ModInt operator-() const { return v == 0 ? make(0) : make(mod - v); }
    ModInt operator+(const ModInt& val) const { return make(norm(v + val())); }
    ModInt operator-(const ModInt& val) const { return make(norm(v + mod - val())); }
    ModInt operator*(const ModInt& val) const { return make((long long)v * val() % mod); }
    ModInt operator/(const ModInt& val) const { return *this * inv(val); }
    ModInt& operator+=(const ModInt& val){ return *this = *this + val; }
    ModInt& operator-=(const ModInt& val){ return *this = *this - val; }
    ModInt& operator*=(const ModInt& val){ return *this = *this * val; }
    ModInt& operator/=(const ModInt& val){ return *this = *this / val; }
    ModInt operator+(const long long val) const { return ModInt{v + val}; }
    ModInt operator-(const long long val) const { return ModInt{v - val}; }
    ModInt operator*(const long long val) const { return ModInt{(long long)(v * (val % mod))}; }
    ModInt operator/(const long long val) const { return ModInt{(long long)v * inv(val)}; }
    ModInt& operator+=(const long long val){ return *this = *this + val; }
    ModInt& operator-=(const long long val){ return *this = *this - val; }
    ModInt& operator*=(const long long val){ return *this = *this * val; }
    ModInt& operator/=(const long long val){ return *this = *this / val; }
    bool operator==(const ModInt& val) const { return v == val.v; }
    bool operator!=(const ModInt& val) const { return !(*this == val); }
    bool operator==(const long long val) const { return v == norm(val % mod + mod); }
    bool operator!=(const long long val) const { return !(*this == val); }
    unsigned int operator()() const { return v; }
    friend ModInt operator+(const long long val, const ModInt& n) { return n + val; }
    friend ModInt operator-(const long long val, const ModInt& n) { return ModInt{val - n()}; }
    friend ModInt operator*(const long long val, const ModInt& n) { return n * val; }
    friend ModInt operator/(const long long val, const ModInt& n) { return ModInt{val} / n; }
    friend bool operator==(const long long val, const ModInt& n) { return n == val; }
    friend bool operator!=(const long long val, const ModInt& n) { return !(val == n); }
    friend istream& operator>>(istream& is, ModInt& n){
        unsigned int v;
        return is >> v, n = v, is;
    }
    friend ostream& operator<<(ostream& os, const ModInt& n){ return (os << n()); }
    friend ModInt mod_pow(ModInt x, long long n){
        ModInt ans = ((mod == 1) ? 0 : 1);
        while(n){
            if(n & 1) ans *= x;
            x *= x, n >>= 1;
        }
        return ans;
    }
};

template<typename T> class mat : public vector<vector<T> > {
private:
    int r, c;    //行,列
public:
    inline int row() const {
        return r;
    }
    inline int column() const {
        return c;
    }
    mat(int n, int m, T val = 0){
        r = n, c = m;
        for(int i = 0; i < n; i++){
            this->push_back(vector<T>(m, val));
        }
    }
    mat operator+(const mat& another) const {
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
    mat operator+(const T val) const {
        mat<T> X(r, c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = (*this)[i][j] + val;
            }
        }
        return X;
    }
    mat operator-(const mat& another) const {
        if(r != another.r && c != another.c){
            cout << "引き算失敗(サイズ不一致)" << endl;
            exit(1);
        }
        mat<T> X(r, c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = (*this)[i][j] - another[i][j];
            }
        }
        return X;
    }
    mat operator-(const T val) const {
        mat<T> X(r, c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = (*this)[i][j] - val;
            }
        }
        return X;
    }
    vector<T> operator*(const vector<T>& another) const {
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
    mat operator*(const mat& another) const {
        if(c != another.r){
            cout << "掛け算失敗(サイズ不一致)" << endl;
            exit(1);
        }
        mat<T> X(r, another.c);
        for(int i = 0; i < r; i++){
            for(int k = 0; k < c; k++){
                for(int j = 0; j < (another.c); j++){
                    X[i][j] += (*this)[i][k] * another[k][j];
                }
            }
        }
        return X;
    }
    mat operator-() const {
        mat<T> X(r, c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = -(*this)[i][j];
            }
        }
        return X;
    }
    int rank() const {
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
            for(int j = res; j < r; j++){
                if(B[j][i]){
                    pivot = j;
                    break;
                }
            }
            if(!B[pivot][i]) continue;
            swap(B[pivot], B[res]);
            const T d = (T)1 / B[res][i];
            for(int j = i + 1; j < c; j++){
                B[res][j] *= d;
            }
            for(int j = res + 1; j < r; j++){
                if(!B[j][i]) continue;
                for(int k = i + 1; k < c; k++){
                    B[j][k] -= B[res][k] * B[j][i];
                }
            }
            ++res;
        }
        return res;
    }
    T det() const {
        if(r != c){
            cout << "正方行列でない(行列式定義不可)" << endl;
            exit(1);
        }
        T ans = 1;
        mat B(r, r);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                B[i][j] = (*this)[i][j];
            }
        }
        for(int i = 0; i < c; i++){
            int pivot = i;
            for(int j = i; j < r; j++){
                if(B[j][i]){
                    pivot = j;
                    break;
                }
            }
            if(!B[pivot][i]) return (T)0;
            if(pivot != i) swap(B[i], B[pivot]), ans = -ans;
            ans *= B[i][i];
            const T d = (T)1 / B[i][i];
            for(int j = i + 1; j < c; j++){
                B[i][j] *= d;
            }
            for(int j = i + 1; j < r; j++){
                if(!B[j][i]) continue;
                for(int k = i + 1; k < c; k++){
                    B[j][k] -= B[i][k] * B[j][i];
                }
            }
        }
        return ans;
    }
    mat inverse() const {
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
                if(B[j][i]){
                    pivot = j;
                    break;
                }
            }
            if(!B[pivot][i]){
                cout << "正則でない" << endl;
                exit(1);
            }
            swap(B[i], B[pivot]);
            const T d = (T)1 / B[i][i];
            for(int j = i + 1; j < 2*r; j++){
                B[i][j] *= d;
            }
            for(int j = 0; j < r; j++){
                if(j == i || !B[j][i]) continue;
                for(int k = i + 1; k < 2*r; k++){
                    B[j][k] -= B[i][k] * B[j][i];
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
    inline void print() const {
        for(int i = 0; i < r; i++){
            for(int j = 0; j < (c-1); j++){
                cout << (*this)[i][j] << ",";
            }
            cout << (*this)[i][c-1] << endl;
        }
    }
};

template<typename T> vector<T> eq_solve(const mat<T>& A, const vector<T>& b){
    if(A.row() != A.column()){
        cout << "正方行列でない(解なしor不定)" << endl;
        exit(1);
    }
    int n = A.row();
    mat<T> B(n, n+1);
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
        for(int j = i; j < n; j++){
            if(B[j][i]){
                pivot = j;
                break;
            }
        }
        if(!B[pivot][i]){
            cout << "解なしor不定" << endl;
            exit(1);
        }
        swap(B[i], B[pivot]);
        const T d = (T)1 / B[i][i];
        for(int j = i + 1; j <= n; j++){
            B[i][j] *= d;
        }
        for(int j = 0; j < n; j++){
            if(i != j){
                for(int k = i + 1; k <= n; k++){
                    B[j][k] -= B[i][k] * B[j][i];
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

template<typename T> mat<T> pow(mat<T> A, long long cnt)
{
    if(A.row() != A.column()){
        cout << "累乗不可" << endl;
    }
    int n = A.row();
    mat<T> B(n, n);
    for(int i = 0; i < n; i++){
        B[i][i] = 1;
    }
    while(cnt > 0){
		if(cnt & 1) B = B * A;
        A = A * A;
        cnt >>= 1;
    }
    return B;
}

using mod = ModInt<MOD>;

// 宣言 mat<mod> A(n, m); 