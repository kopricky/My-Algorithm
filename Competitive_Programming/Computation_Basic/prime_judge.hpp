#include "../header.hpp"

//素数かどうかを判定する
bool is_prime(long long n)
{
    for(int i=2;(long long)i*i<=n;i++){
        if(n % i == 0){
            return false;
        }
    }
    return n != 1;
}
