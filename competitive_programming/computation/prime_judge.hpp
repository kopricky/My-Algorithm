#include "../header.hpp"

//素数かどうかを判定する
bool is_prime(ll n)
{
    for(ll i=2;i*i<=n;i++){
        if(n % i == 0){
            return false;
        }
    }
    return n != 1;
}
