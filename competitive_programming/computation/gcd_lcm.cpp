ll gcd(ll a,ll b)
{
    if(a % b == 0){
        return b;
    }else{
        return gcd(b,a%b);
    }
}

ll lcm(ll a,ll b)
{
    return  a / gcd(a,b) * b;
}
