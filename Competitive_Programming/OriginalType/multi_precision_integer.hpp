#include "../header.hpp"

// シフト演算子を使ったり, ビット演算をしたりしていなければ int や long long と同じように動くはず
// 主要な演算は verify した

// 宣言は MPI(数字 or string) もしくは MPI hoge = 数字; またデフォルトは0初期化されます
// 引数の string は 0 を除いて leading-zero がないことを仮定しています
// シフト演算子, および ビット演算(&,|,^) はフォローしていません
// 数字(intやlong long) との演算や比較は暗黙的に MPI にキャストされます
// 多倍長整数の足し算, 引き算, 掛け算, 割り算(商と余り), 2で割る(div2), sqrt, log10, pow, 絶対値, 大小判定, 入出力, 任意の桁へのランダムアクセス などを行えます
// 2で割る, sqrt, log10 は lower_bound を返します(MPI型)
// n 桁(10進数)の数に対して 足し算, 引き算(O(n)), 掛け算(O(nlogn)), 割り算(O(n^2)), 2で割る(O(n)), sqrt((10~20)*O(n^2)), log10(O(1)), pow(nlognlogm) の計算時間がかかります
// 掛け算の演算を効率的に行うために本実装では base を 10 にとっています(足し算, 引き算, 割り算を定数倍高速化したいなら base を 10^9 とか 10^18 とかでとったほうがよい)
// 掛け算は NTT を用いています
// 負の除算もフォローしています(定義はC++の除算と同じです)
// 2で割るのは多倍長整数での2分探索やsqrt計算に使う目的で実装しました
// to_ll, to_string で long long, string に変換可能
// 10倍や10で割るなどもO(1)で行うようにできる

class MPI : public vector<int> {
private:

    static constexpr int root = 5;
    static constexpr int MOD_ = 924844033;

    bool sign;

    static void trim_sign(MPI& num){
        if(num.isZero()) num.sign = false;
    }
    static void trim_digit(MPI& num){
        while(num.back() == 0 && (int)num.size() >= 2) num.pop_back();
    }
    static bool abs_less(const MPI& a, const MPI& b){
        if(a.size() != b.size()) return a.size() < b.size();
        for(int index = (int)a.size() - 1; index >= 0; index--){
            if(a[index] != b[index]) return a[index] < b[index];
        }
        return false;
    }
    static void num_sbst(MPI& a, const MPI& b){
        int n = (int)b.size();
        a.resize(n);
        for(int i = 0; i < n; i++) a[i] = b[i];
    }
    static void add(const MPI& a, const MPI& b, MPI& res){
        num_sbst(res, a);
        int mx = (int)max(a.size(), b.size());
        res.resize(mx, 0);
        int carry = 0;
        for(int i = 0; i < mx; i++){
            int val = res[i] + ((i < (int)b.size()) ? b[i] : 0) + carry;
            carry = val/10;
            res[i] = val%10;
        }
        if(carry) res.push_back(1);
    }
    static void sub(const MPI& a, const MPI& b, MPI& res){
        num_sbst(res, a);
        int carry = 0;
        for(int i = 0; i < (int)res.size(); i++){
            int val = res[i] - carry - ((i < (int)b.size()) ? b[i] : 0);
            if(val < 0){
                carry = 1, val += 10;
            }else{
                carry = 0;
            }
            res[i] = val;
        }
        trim_digit(res), trim_sign(res);
    }
    static int add_(const int x, const int y) { return (x + y < MOD_) ? x + y : x + y - MOD_; }
    static int mul_(const int x, const int y) { return (long long)x * y % MOD_; }
    static int power(int x, int n){
        int res = 1;
        while(n > 0){
            if(n & 1) res = mul_(res, x);
            x = mul_(x, x);
            n >>= 1;
        }
        return res;
    }
    static int inverse(const int x) { return power(x, MOD_ - 2); }
    static void ntt(vector<int>& a, const bool rev = false){
        int i,j,k,s,t,v,w,wn;
        const int size = (int)a.size();
        const int height = (int)log2(2 * size - 1);
        for(i = 0; i < size; i++){
            j = 0;
            for(k = 0; k < height; k++) j |= (i >> k & 1) << (height - 1 - k);
            if(i < j) std::swap(a[i], a[j]);
        }
        for(i = 1; i < size; i *= 2) {
            w = power(root, (MOD_ - 1) / (i * 2));
            if(rev) w = inverse(w);
            for(j = 0; j < size; j += i * 2){
                wn = 1;
                for(k = 0; k < i; k++){
                    s = a[j + k], t = mul_(a[j + k + i], wn);
                    a[j + k] = add_(s, t);
                    a[j + k + i] = add_(s, MOD_ - t);
                    wn = mul_(wn, w);
                }
            }
        }
        if(rev){
            v = inverse(size);
            for (i = 0; i < size; i++) a[i] = mul_(a[i], v);
        }
    }
    static void mul(const MPI& a, const MPI& b, MPI& res){
        const int size = (int)a.size() + (int)b.size() - 1;
        int t = 1;
        while (t < size) t <<= 1;
        vector<int> A(t, 0), B(t, 0);
        for(int i = 0; i < (int)a.size(); i++) A[i] = a[i];
        for(int i = 0; i < (int)b.size(); i++) B[i] = b[i];
        ntt(A), ntt(B);
        for(int i = 0; i < t; i++) A[i] = mul_(A[i], B[i]);
        ntt(A, true);
        res.resize(size);
        int carry = 0;
        for(int i = 0; i < size; i++){
            int val = A[i] + carry;
            carry = val / 10;
            res[i] = val % 10;
        }
        if(carry) res.push_back(carry);
        trim_digit(res), trim_sign(res);
    }
    bool isZero() const {
        return (int)size() == 1 && (*this)[0] == 0;
    }
    static bool div_geq(const MPI& mod, const MPI& num){
        if((int)mod.size() != (int)num.size()) return (int)mod.size() > (int)num.size();
        int n = (int)mod.size();
        for(int i = 0; i < n; i++){
            if(mod[i] != num[n-1-i]){
                return mod[i] > num[n-1-i];
            }
        }
        return true;
    }
    static void div_(const MPI& a, const MPI& b, MPI& quo, MPI& mod){
        vector<MPI> mult(9);
        mult[0] = b;
        for(int i = 0; i < 8; i++) mult[i+1] = mult[i] + b;
        for(int i = (int)a.size() - 1; i >= 0; i--){
            if(mod.isZero()){
                mod.back() = a[i];
            }else{
                mod.push_back(a[i]);
            }
            if(div_geq(mod, b)){
                int l = 0, r = 9;
                reverse(mod.begin(), mod.end());
                while(r-l>1){
                    int mid = (l+r)/2;
                    if(mult[mid] > mod){
                        r = mid;
                    }else{
                        l = mid;
                    }
                }
                mod -= mult[l];
                reverse(mod.begin(), mod.end());
                quo.push_back(l+1);
            }else{
                quo.push_back(0);
            }
        }
        reverse(quo.begin(), quo.end());
        trim_digit(quo);
        reverse(mod.begin(), mod.end());
    }

public:
    MPI() : sign(false){ this->push_back(0); }

    MPI(long long val) : sign(false){
        if(val == 0){
            this->push_back(0);
        }else{
            if(val < 0) sign = true, val = -val;
            while(val){
                this->push_back(val%10);
                val /= 10;
            }
        }
    }

    MPI(const string& s) : sign(false){
        if(s.empty()){
            this->push_back(0);
            return;
        }
        if(s[0] == '-') sign = true;
        for(int i = (int)s.size() - 1; i >= sign; i--) this->push_back(s[i]-'0');
    }

    long long to_ll() const {
        long long res = 0, dig = 1;
        for(int i = 0; i < (int)size(); i++){
            res += dig * (*this)[i], dig *= 10;
        }
        if(sign) res = -res;
        return res;
    }

    string to_string() const {
        int n = (int)size() + sign;
        string s(n, ' ');
        if(sign) s[0] = '-';
        for(int i = sign; i < n; i++) s[i] = (char)('0'+(*this)[n-1-i]);
        return s;
    }

    friend istream& operator>>(istream& is, MPI& num) {
        string s;
        is >> s;
        num = MPI(s);
        return is;
    }

    friend ostream& operator<<(ostream& os, const MPI& num) {
        if(num.sign) os << '-';
        for(int i = (int)num.size()-1; i >= 0; i--) os << (char)('0'+num[i]);
        return os;
    }

    MPI& operator=(long long val) {
        *this = MPI(val);
        return *this;
    }

    bool operator<(const MPI& another) const {
        if(sign ^ another.sign) return sign;
        if(size() != another.size()) return (size() < another.size()) ^ sign;
        for(int index = (int)size() - 1; index >= 0; index--){
            if((*this)[index] != another[index]) return ((*this)[index] < another[index]) ^ sign;
        }
        return false;
    }

    bool operator<(const long long num) const {
        return *this < MPI(num);
    }

    friend bool operator<(const long long num, const MPI& another){
        return MPI(num) < another;
    }

    bool operator>(const MPI& another) const {
        return another < *this;
    }

    bool operator>(const long long num) const {
        return *this > MPI(num);
    }

    friend bool operator>(const long long num, const MPI& another){
        return MPI(num) > another;
    }

    bool operator<=(const MPI& another) const {
        return !(*this > another);
    }

    bool operator<=(const long long num) const {
        return *this <= MPI(num);
    }

    friend bool operator<=(const long long num, const MPI& another){
        return MPI(num) <= another;
    }

    bool operator>=(const MPI& another) const {
        return !(*this < another);
    }

    bool operator>=(const long long num) const {
        return *this >= MPI(num);
    }

    friend bool operator>=(const long long num, const MPI& another){
        return MPI(num) >= another;
    }

    bool operator==(const MPI& another) const {
        if(sign ^ another.sign) return false;
        if(size() != another.size()) return false;
        for(int index = (int)size() - 1; index >= 0; index--){
            if((*this)[index] != another[index]) return false;
        }
        return true;
    }

    bool operator==(const long long num) const {
        return *this == MPI(num);
    }

    friend bool operator==(const long long num, const MPI& another){
        return MPI(num) == another;
    }

    bool operator!=(const MPI& another) const {
        return !(*this == another);
    }

    bool operator!=(const long long num) const {
        return *this != MPI(num);
    }

    friend bool operator!=(const long long num, const MPI& another){
        return MPI(num) != another;
    }

    explicit operator bool() const noexcept { return !isZero(); }
    bool operator!() const noexcept { return !static_cast<bool>(*this); }

    explicit operator int() const noexcept { return (int)this->to_ll(); }
    explicit operator long long() const noexcept { return this->to_ll(); }

    MPI operator+() const {
        return *this;
    }

    MPI operator-() const {
        MPI res = *this;
        res.sign = !sign;
        return res;
    }

    friend MPI abs(const MPI& num){
        MPI res = num;
        res.sign = false;
        return res;
    }

    MPI operator+(const MPI& num) const {
        MPI res; res.sign = sign;
        if(sign != num.sign){
            if(abs_less(*this, num)){
                res.sign = num.sign;
                sub(num, *this, res);
                return res;
            }else{
                sub(*this, num, res);
                return res;
            }
        }
        add(*this, num, res);
        return res;
    }

    MPI operator+(long long num) const {
        return *this + MPI(num);
    }

    friend MPI operator+(long long a, const MPI& b){
        return b + a;
    }

    MPI& operator+=(const MPI& num){
        *this = *this + num;
        return *this;
    }

    MPI& operator+=(long long num){
        *this = *this + num;
        return *this;
    }

    MPI& operator++(){
        return *this += 1;
    }

    MPI operator++(int){
        MPI res = *this;
        *this += 1;
        return res;
    }

    MPI operator-(const MPI& num) const {
        if(sign != num.sign){
            MPI res; res.sign = sign;
            add(*this, num, res);
            return res;
        }
        MPI res; res.sign = (abs_less(*this, num) ^ sign);
        if(res.sign){
            sub(num, *this, res);
        }else{
            sub(*this, num, res);
        }
        return res;
    }

    MPI operator-(long long num) const {
        return *this - MPI(num);
    }

    friend MPI operator-(long long a, const MPI& b){
        return b - a;
    }

    MPI& operator-=(const MPI& num){
        *this = *this - num;
        return *this;
    }

    MPI& operator-=(long long num){
        *this = *this - num;
        return *this;
    }

    MPI& operator--(){
        return *this -= 1;
    }

    MPI operator--(int){
        MPI res = *this;
        *this -= 1;
        return res;
    }

    MPI operator*(const MPI& num) const {
        MPI res; res.sign = (sign^num.sign);
        mul(*this, num, res);
        return res;
    }

    MPI operator*(long long num) const {
        return *this * MPI(num);
    }

    friend MPI operator*(long long a, const MPI& b){
        return b * a;
    }

    MPI& operator*=(const MPI& num){
        *this = *this * num;
        return *this;
    }

    MPI& operator*=(long long num){
        *this = *this * num;
        return *this;
    }

    MPI operator/(const MPI& num) const {
        MPI num_ = abs(num);
        MPI a, b;
        div_(*this, num_, a, b);
        a.sign = (sign^num.sign);
        trim_sign(a);
        return a;
    }

    MPI operator/(long long num) const {
        return *this / MPI(num);
    }

    friend MPI operator/(long long a, const MPI& b){
        return b / a;
    }

    MPI& operator/=(const MPI& num){
        *this = *this / num;
        return *this;
    }

    MPI& operator/=(long long num){
        *this = *this / num;
        return *this;
    }

    MPI operator%(const MPI& num) const {
        MPI num_ = abs(num);
        MPI a, b;
        div_(*this, num_, a, b);
        b.sign = sign;
        trim_sign(b);
        return b;
    }

    MPI operator%(long long num) const {
        return *this % MPI(num);
    }

    friend MPI operator%(long long a, const MPI& b){
        return b % a;
    }

    MPI& operator%=(const MPI& num){
        *this = *this % num;
        return *this;
    }

    MPI& operator%=(long long num){
        *this = *this % num;
        return *this;
    }

    MPI div2() const {
        MPI res; res.sign = sign;
        int n = (int)this->size(), carry = 0;
        for(int i = n-1; i >= 0; i--){
            int val = (*this)[i]+carry*10;
            carry = val%2;
            if(i != n-1 || val >= 2) res.push_back(val/2);
        }
        reverse(res.begin(), res.end());
        trim_digit(res);
        return res;
    }

    friend MPI sqrt(const MPI& x){
        if(x <= MPI(0)) return MPI(0);
        MPI s = 1, t = x;
        while(s < t){
            s = s + s, t = t.div2();
        }
        do{ t = s, s = (x / s + s).div2();
        }while(s < t);
        return t;
    }

    friend MPI log10(const MPI& x){
        assert(x > MPI(0));
        return MPI((int)x.size());
    }

    friend MPI pow(MPI a, MPI b){
        assert(b >= 0);
        MPI res(1);
        while(b){
            if(b[0] % 2){
                res *= a;
            }
            a *= a;
            b = b.div2();
        }
        return res;
    }
};
