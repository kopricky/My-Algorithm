#include "../header.hpp"

template <uint mod>
class ModInt {
private:
    uint v;
    static uint norm(const uint& x){ return x < mod ? x : x - mod; }
	static ModInt make(const uint& x){ ModInt m; return m.v = x, m; }
    static ModInt inv(const ModInt& x){ return make(inverse(x.v, mod)); }
    static uint inverse(int a, int m){
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
    ModInt operator-() const { return v == 0 ? 0 : mod - v; }
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
    ModInt operator*(const long long val) const { return ModInt{(long long)v * (val % mod)}; }
    ModInt operator/(const long long val) const { return ModInt{(long long)v * inv(val)}; }
    ModInt& operator+=(const long long val){ return *this = *this + val; }
    ModInt& operator-=(const long long val){ return *this = *this - val; }
    ModInt& operator*=(const long long val){ return *this = *this * val; }
    ModInt& operator/=(const long long val){ return *this = *this / val; }
    bool operator==(const ModInt& val) const { return v == val.v; }
    bool operator!=(const ModInt& val) const { return !(*this == val); }
    bool operator==(const long long val) const { return v == norm(val % mod + mod); }
    bool operator!=(const long long val) const { return !(*this == val); }
    uint operator()() const { return v; }
    friend ModInt operator+(const long long val, const ModInt& n) { return n + val; }
    friend ModInt operator-(const long long val, const ModInt& n) { return ModInt{val - n()}; }
    friend ModInt operator*(const long long val, const ModInt& n) { return n * val; }
    friend ModInt operator/(const long long val, const ModInt& n) { return ModInt{val} / n; }
    friend bool operator==(const long long val, const ModInt& n) { return n == val; }
    friend bool operator!=(const long long val, const ModInt& n) { return !(val == n); }
    friend istream& operator>>(istream& is, ModInt& n){
        uint v;
        return is >> v, n = v, is;
    }
    friend ostream& operator<<(ostream& os, const ModInt& n){ return (os << n()); }
    friend ModInt power(ModInt x, long long n){
        ModInt ans = 1;
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
    inline int row() const { return r; }
    inline int column() const { return c; }
    mat(const int n) : mat(n, n, 0){}
    mat(const int n, const int m, T val = 0)
        : vector<vector<T> >(n, vector<T>(m, val)), r{n}, c{m}{}
    mat operator+(const mat& another) const {
        mat<T> X(r, c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = (*this)[i][j] + another[i][j];
            }
        }
        return X;
    }
    mat operator-(const mat& another) const {
        mat<T> X(r,c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = (*this)[i][j] - another[i][j];
            }
        }
        return X;
    }
    mat operator*(const mat& another) const {
        mat<T> X(r, another.c);
        for(int i = 0; i < r; i++){
            for(int k = 0; k < c; k++){
                if(!(*this)[i][k]) continue;
                for(int j = 0; j < (another.c); j++){
                    X[i][j] += (*this)[i][k] * another[k][j];
                }
            }
        }
        return X;
    }
    vector<int> rank(){
        mat<T> X(r, c);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = (*this)[i][j];
            }
        }
        vector<int> index;
        int res = 0;
        for(int i = 0; i < c; i++){
            if(res == r) break;
            if(!X[res][i]){
                int pivot = res + 1;
                for(; pivot < r; pivot++){
                    if(X[pivot][i]){
                        swap(X[res], X[pivot]);
                        break;
                    }
                }
                if(pivot == r) continue;
            }
            const T p = (T)1 / X[res][i];
            for(int j = i + 1; j < c; j++){ X[res][j] *= p; }
            for(int j = res + 1; j < r; j++){
                if(!X[j][i]) continue;
                for(int k = i + 1; k < c; k++){
                    X[j][k] -= X[res][k] * X[j][i];
                }
            }
            index.push_back(i), ++res;
        }
        return index;
    }
    bool det_zero() const {
        mat<T> X(r);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                X[i][j] = (*this)[i][j];
            }
        }
        for(int i = 0; i < c; i++){
            if(!X[i][i]){
                int pivot = i + 1;
                for(; pivot < r; pivot++){
                    if(X[pivot][i]){
                        swap(X[i], X[pivot]);
                        break;
                    }
                }
                if(pivot == r) return true;
            }
            const T p = (T)1 / X[i][i];
            for(int j = i + 1; j < c; j++){ X[i][j] *= p; }
            for(int j = i + 1; j < r; j++){
                if(!X[j][i]) continue;
                for(int k = i + 1; k < c; k++){
                    X[j][k] -= X[i][k] * X[j][i];
                }
            }
        }
        return false;
    }
    mat inverse() const {
        mat X(r, 2*r);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < r; j++){
                X[i][j] = (*this)[i][j];
            }
        }
        for(int i = 0; i < r; i++){
            X[i][r+i] = 1;
        }
        for(int i = 0; i < r; i++){
            if(!X[i][i]){
                int pivot = i + 1;
                for(; pivot < r; pivot++){
                    if(X[pivot][i]){
                        swap(X[i],X[pivot]);
                        break;
                    }
                }
                assert(pivot < r);
            }
            const T p = (T)1 / X[i][i];
            for(int j = i + 1; j < 2*r; j++){ X[i][j] *= p; }
            for(int j = 0; j < r; j++){
                if(i == j || !X[j][i]) continue;
                for(int k = i + 1; k < 2*r; k++){
                    X[j][k] -= X[i][k] * X[j][i];
                }
            }
        }
        mat res(r, r);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < r; j++){
                res[i][j] = X[i][r+j];
            }
        }
        return res;
    }
};

class Matching {
private:
    const int V;
    vector<pair<int, int> > es;
    vector<int> index, cnt;
    random_device rnd;
    mt19937 mt;
    uniform_int_distribution<> randval;
    static constexpr int LOOP = 1;
    static constexpr uint mod = 1000000007;
    mat<ModInt<mod> > inverse_22(const int id, const mat<ModInt<mod> >& invT){
        ModInt<mod> invdev = (ModInt<mod>)1 / (invT[0][0]*invT[id][id] - invT[0][id]*invT[id][0]);
        mat<ModInt<mod> > invN_3n_3n(2, 2);
        invN_3n_3n[0][0] = invT[id][id] * invdev, invN_3n_3n[0][1] = -invT[0][id] * invdev;
        invN_3n_3n[1][0] = -invT[id][0] * invdev, invN_3n_3n[1][1] = invT[0][0] * invdev;
        return invN_3n_3n;
    }
    void schur_complement(const int id, mat<ModInt<mod> >& invT){
        int sz = invT.row(), x = 0;
        mat<ModInt<mod> >N_12_12(sz-2), N_12_3n(sz-2, 2), N_3n_12(2, sz-2);
        for(int i = 1; i < sz; i++){
            if(i == id) continue;
            int y = 0;
            for(int j = 1; j < sz; j++){
                if(j == id) continue;
                N_12_12[x][y] = invT[i][j];
                N_3n_12[0][y] = invT[0][j], N_3n_12[1][y++] = invT[id][j];
            }
            N_12_3n[x][0] = invT[i][0], N_12_3n[x++][1] = invT[i][id];
        }
        invT = N_12_12 - N_12_3n * inverse_22(id, invT) * N_3n_12;
    }
    void find_matching(const mat<ModInt<mod> >& T){
        int sz = T.row();
        mat<ModInt<mod> > invT = T.inverse();
        vector<int> vset(sz); iota(vset.begin(), vset.end(), 0);
        for(int i = sz; i > 0; i -= 2){
            for(int j = 1; j < i; j++){
                if(invT[0][j] && T[vset[0]][vset[j]]){
                    ans.emplace_back(index[vset[0]], index[vset[j]]);
                    vset.erase(vset.begin()), vset.erase(vset.begin() + j - 1);
                    schur_complement(j, invT);
                    break;
                }
            }
        }
    }

public:
    vector<pair<int, int> > ans;

    Matching(const int node_size)
        : V(node_size), cnt(V + 1, 0), mt(rnd()), randval(1, mod - 1){}
    void add_edge(const int u, const int v){ es.emplace_back(u, v); }
    bool perfect_matchching(){
        for(int i = 0; i < LOOP; i++){
            mat<ModInt<mod> > A(V);
            for(auto e : es){
                int r = randval(mt);
                A[e.first][e.second] = r, A[e.second][e.first] = -r;
            }
            if(!A.det_zero()) return true;
        }
        return false;
    }
    int maximum_matchching(){
        for(int i = 0; i < LOOP; i++){
            mat<ModInt<mod> > A(V);
            for(auto e : es){
                int r = randval(mt);
                A[e.first][e.second] = r, A[e.second][e.first] = -r;
            }
            vector<int> res = A.rank();
            ++cnt[res.size()];
            if((size_t)(max_element(cnt.begin(), cnt.end()) - cnt.begin()) == res.size()){
                index = move(res);
            }
        }
        return (int)(max_element(cnt.begin(), cnt.end()) - cnt.begin()) / 2;
    }
    int find_maximum_matching(){
        const int sz = maximum_matchching();
        vector<pair<int, int> > ans_tmp;
        vector<int> unzip(V, -1);
        for(int i = 0; i < 2 * sz; ++i) unzip[index[i]] = i;
        for(int i = 0; i < LOOP; i++){
            mat<ModInt<mod> > A(2 * sz);
            for(auto e : es){
                const int u = unzip[e.first], v = unzip[e.second];
                if(u < 0 || v < 0) continue;
                int r = randval(mt);
                A[u][v] = r, A[v][u] = -r;
            }
            if(!A.det_zero()){
                find_matching(A);
                break;
            }
        }
        return sz;
    }
};
