#include "../header.hpp"

template<typename T>
T gcd(T a, T b)
{
    if(a == 0 || b == 0) return max(a, b);
    if(a % b == 0){
        return b;
    }else{
        return gcd(b,a%b);
    }
}

template<typename T>
T lcm(T a,T b)
{
    return  a / gcd(a,b) * b;
}
