vector<ll> divisor(ll n)
{
    vector<ll> res;
    for(ll i=1;i*i<=n;i++){
        if(n%i==0){
            res.push_back(i);
            if(i != n/i){
                res.push_backs(n/i);
            }
        }
    }
    sort(all(res));
    return res;
}
