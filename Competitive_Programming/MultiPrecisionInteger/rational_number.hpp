#include "../header.hpp"

template<typename T> class Rational {
private:
    static T gcd(const T a, const T b){ return (b != 0) ? gcd(b, a % b) : a; }
    static T lcm(const T a, const T b){ return a / gcd(a, b) * b; }

public:
    friend ostream& operator<<(ostream& os, const Rational& rn) {
        os << rn.val << " / " << rn.den;
        return os;
    }

    Rational& operator=(T val) {
        *this = Rational(val);
        return *this;
    }

    bool operator<(const Rational& val) const {
        return num * val.den < den * val.num;
    }

    bool operator<(const T val) const {
        return *this < Rational(val);
    }

    friend bool operator<(const T val, const Rational& another){
        return Rational(val) < another;
    }

    bool operator>(const Rational& val) const {
        return val < num;
    }

    bool operator>(const T val) const {
        return *this > Rational(val);
    }

    friend bool operator>(const T val, const Rational& another){
        return Rational(val) > another;
    }

    bool operator<=(const Rational& val) const {
        return !(*this > val);
    }

    bool operator<=(const T val) const {
        return *this <= Rational(val);
    }

    friend bool operator<=(const T val, const Rational& another){
        return Rational(val) <= another;
    }

    bool operator>=(const Rational& val) const {
        return !(*this < val);
    }

    bool operator>=(const T val) const {
        return *this >= Rational(val);
    }

    friend bool operator>=(const T val, const Rational& another){
        return Rational(val) >= another;
    }

    bool operator==(const Rational& val) const {
        return num * val.den == den * val.num;
    }

    bool operator==(const T val) const {
        return *this == Rational(val);
    }

    friend bool operator==(const T val, const Rational& another){
        return Rational(val) == another;
    }

    bool operator!=(const Rational& val) const {
        return !(*this == val);
    }

    bool operator!=(const T val) const {
        return *this != Rational(val);
    }

    friend bool operator!=(const T val, const Rational& another){
        return Rational(val) != another;
    }

    explicit operator bool() const noexcept { return num; }
    bool operator!() const noexcept { return !static_cast<bool>(*this); }

    Rational operator+() const {
        return *this;
    }

    Rational operator-() const {
        return Rational(-num, den);
    }

    friend Rational abs(const Rational& val){
        return Rational(abs(val.num), val.den);
    }

    Rational operator+(const Rational& val) const {
        const T nden = lcm(den, val.den);
        const T nnum = nden / den * num + nden / val.den * val.num;
        return Rational(nnum, nden);
    }

    Rational operator+(T val) const {
        return *this + Rational(val);
    }

    friend Rational operator+(T a, const Rational& b){
        return b + a;
    }

    Rational& operator+=(const Rational& val){
        *this = *this + val;
        return *this;
    }

    Rational& operator+=(const T& val){
        *this = *this + val;
        return *this;
    }

    Rational& operator++(){
        return *this += 1;
    }

    Rational operator++(int){
        return Rational(num + den, den);
    }

    Rational operator-(const Rational& val) const {
        const T nden = lcm(den, val.den);
        const T nnum = nden / den * num - nden / val.den * val.num;
        return Rational(nnum, nden);
    }

    Rational operator-(T val) const {
        return *this - Rational(val);
    }

    friend Rational operator-(T a, const Rational& b){
        return Rational(a) - b;
    }

    Rational& operator-=(const Rational& val){
        *this = *this - val;
        return *this;
    }

    Rational& operator-=(const T& val){
        *this = *this - val;
        return *this;
    }

    Rational& operator--(){
        return *this -= 1;
    }

    Rational operator--(int){
        return Rational(num - den, den);
    }

    Rational operator*(const Rational& val) const {
        const T g1 = gcd(num, val.den), g2 = gcd(den, val.num);
        return Rational((num / g1) * (val.num / g2), (den / g2) * (val.den / g1));
    }

    Rational operator*(T val) const {
        return *this * Rational(val);
    }

    friend Rational operator*(T a, const Rational& b){
        return b * a;
    }

    Rational& operator*=(const Rational& val){
        *this = *this * val;
        return *this;
    }

    Rational& operator*=(const T& val){
        *this = *this * val;
        return *this;
    }

    Rational operator/(const Rational& val) const {
        const T g1 = gcd(num, val.num), g2 = gcd(den, val.den);
        return Rational((num / g1) * (val.den / g2), (den / g2) * (val.num / g1));
    }

    Rational operator/(T val) const {
        return *this / Rational(val);
    }

    friend Rational operator/(T a, const Rational& b){
        return Rational(a) / b;
    }

    Rational& operator/=(const Rational& val){
        *this = *this / val;
        return *this;
    }

    Rational& operator/=(const T& val){
        *this = *this / val;
        return *this;
    }

    T num, den;

    Rational(){}
    Rational(T num_) : num(num_), den(1){}
    Rational(T num_, T den_) : num(num_), den(den_){}
};
