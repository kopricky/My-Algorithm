#include "../header.hpp"

template<typename T>
T gcd(T a,T b)
{
    if(a % b == 0){
        return b;
    }else{
        return gcd(b,a%b);
    }
}
 
template <typename T>
void extgcd(T a, T b, T& x, T& y)
{
    if(b != 0){
        extgcd(b,a%b,y,x);
        y -= (a/b)*x;
    }else{
        x = 1;
        y = 0;
    }
}
 
//(value, mod)
template <typename T>
pair<T, T> CRT(const pair<T, T>& a1, const pair<T, T>& a2)
{
    const T v1 = a1.first, m1 = a1.second;
    const T v2 = a2.first, m2 = a2.second;
    if(v1 == v2) return make_pair(v1,m1*m2);
    T x, y;
    extgcd(m1, m2, x, y);
    x *= (v2 - v1), y *= (v2 - v1);
    const T m = m1 * m2;
    const T res = ((m1 * (x % m2)) + v1) % m;
    return make_pair((res >= 0 ? res : res + m), m);
}
 
//modどうしが互いに素でないとき
//(value, mod)
// template <typename T>
// pair<T, T> CRT(const pair<T, T>& a1, const pair<T, T>& a2)
// {
//     const T v1 = a1.first, m1 = a1.second;
//     const T v2 = a2.first, m2 = a2.second;
//     T g = gcd(m1, m2);
//     T dev = (v2 - v1) / g;
//     T mod = abs(v2 - v1) % g;
//     if(mod) return make_pair(-1,-1);
//     T x, y;
//     extgcd(m1 / g, m2 / g, x, y);
//     x *= dev, y *= dev;
//     const T m = m1 / g * m2;
//     const T res = ((m1 * (x % (m2 / g))) + v1) % m;
//     return make_pair((res >= 0 ? res : res + m), m);
// }