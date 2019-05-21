#include "../header.hpp"

template<typename T> class Rational {
private:
    static Rational make(const T& x, const T& y){
        Rational m; return m.num = x, m.den = y, m;
    }
public:
    friend ostream& operator<<(ostream& os, const Rational& rn) {
        return (os << rn.num << " / " << rn.den);
    }
    Rational& operator=(T val){ return *this = Rational(val); }
    bool operator<(const Rational& val) const { return num*val.den < den*val.num; }
    bool operator<(const T val) const { return *this < Rational(val); }
    friend bool operator<(const T val1, const Rational& val2){ return Rational(val1) < val2; }
    bool operator>(const Rational& val) const { return val < *this; }
    bool operator>(const T val) const { return *this > Rational(val); }
    friend bool operator>(const T val1, const Rational& val2){ return Rational(val1) > val2; }
    bool operator<=(const Rational& val) const { return !(*this > val); }
    bool operator<=(const T val) const { return *this <= Rational(val); }
    friend bool operator<=(const T val1, const Rational& val2){ return Rational(val1) <= val2; }
    bool operator>=(const Rational& val) const { return !(*this < val); }
    bool operator>=(const T val) const { return *this >= Rational(val); }
    friend bool operator>=(const T val1, const Rational& val2){ return Rational(val1) >= val2; }
    bool operator==(const Rational& val) const { return num*val.den == den*val.num; }
    bool operator==(const T val) const { return *this == Rational(val); }
    friend bool operator==(const T val1, const Rational& val2){ return Rational(val1) == val2; }
    bool operator!=(const Rational& val) const { return !(*this == val); }
    bool operator!=(const T val) const { return *this != Rational(val); }
    friend bool operator!=(const T val1, const Rational& val2){ return Rational(val1) != val2; }
    explicit operator bool() const noexcept { return num; }
    bool operator!() const noexcept { return !static_cast<bool>(*this); }
    Rational operator+() const { return *this; }
    Rational operator-() const { return make(-num, den); }
    friend Rational abs(const Rational& val){ return make(abs(val.num), val.den); }
    Rational operator+(const Rational& val) const { return make(num*val.den+val.num*den, den*val.den); }
    Rational operator+(T val) const { return *this + Rational(val); }
    friend Rational operator+(T a, const Rational& b){ return b + a; }
    Rational& operator+=(const Rational& val){ return *this = *this + val; }
    Rational& operator+=(const T& val){ return *this = *this + val; }
    Rational& operator++(){ return *this += 1; }
    Rational operator++(int){ return make(num + den, den); }
    Rational operator-(const Rational& val) const { return make(num*val.den-val.num*den, den*val.den); }
    Rational operator-(T val) const { return *this - Rational(val); }
    friend Rational operator-(T a, const Rational& b){ return Rational(a) - b; }
    Rational& operator-=(const Rational& val){ return *this = *this - val; }
    Rational& operator-=(const T& val){ return *this = *this - val; }
    Rational& operator--(){ return *this -= 1; }
    Rational operator--(int){ return make(num - den, den); }
    Rational operator*(const Rational& val) const { return make(num*val.num, den*val.den); }
    Rational operator*(T val) const { return *this * Rational(val); }
    friend Rational operator*(T a, const Rational& b){ return b * a; }
    Rational& operator*=(const Rational& val){ return *this = *this * val;}
    Rational& operator*=(const T& val){ return *this = *this * val; }
    Rational operator/(const Rational& val) const { return make(num*val.den, den*val.num); }
    Rational operator/(T val) const { return *this / Rational(val); }
    friend Rational operator/(T a, const Rational& b){ return Rational(a) / b; }
    Rational& operator/=(const Rational& val){ return *this / val; }
    Rational& operator/=(const T& val){ return *this = *this / val; }

    T num, den;

    Rational(){}
    Rational(T num_) : num(num_), den(1){}
    Rational(T num_, T den_) : num(num_), den(den_){ if(den < 0) num = -num, den = -den; }
};

template<typename T>
bool is_valid(const Rational<T>& a, const Rational<T>& b, const Rational<T>& c)
{
    return (a < b && b < c);
}

// 有理数 → 連分数展開(2 種類)
template<typename T>
void rtocf(const Rational<T>& val, vector<vector<T> >& res)
{
    if(res.empty()) res.push_back(vector<T>());
    res[0].push_back(val.num / val.den);
    long long mod = val.num % val.den;
    if(mod == 0){
        res.push_back(res[0]);
        if(res[0].back() == 1 && (int)res[0].size() > 1){
            res[1].pop_back(), --res[1].back();
        }else{
            --res[0].back(), res[0].push_back(1);
        }
        return;
    }else{
        rtocf(Rational<T>(val.den, val.num-res[0].back()*val.den) , res);
    }
}

// 連分数展開 → 有理数
template<typename T>
Rational<T> cftor(vector<T> val)
{
    T a = 1, b = 0;
    while(!val.empty()){
        swap(a, b);
        a += val.back()*b;
        val.pop_back();
    }
    return Rational<T>(a, b);
}

// val1, val2 の間にある (分子)+(分母) が最小の有理数を返す.
template<typename T>
Rational<T> find_rational(const Rational<T> val1, const Rational<T> val2)
{
    vector<vector<T> > l, r;
    rtocf(val1, l), rtocf(val2, r);
    vector<T> res;
    for(vector<T>& lvec : l){
        for(vector<T>& rvec : r){
            vector<T> nw;
            int i = 0, mn_size = min((int)lvec.size(), (int)rvec.size());
            for(i = 0; i < mn_size; ++i){
                if(lvec[i] != rvec[i]){
                    nw.push_back(min(lvec[i], rvec[i]) + 1);
                    break;
                }else{
                    nw.push_back(lvec[i]);
                }
            }
            if(i == mn_size){
                if((int)lvec.size() > mn_size){
                    nw.push_back(lvec[mn_size]+1);
                }else{
                    nw.push_back(rvec[mn_size]+1);
                }
            }
            Rational<T> cand = cftor(nw);
            if(is_valid(val1, cand, val2)) return cand;
        }
    }
    return Rational<T>();
}
