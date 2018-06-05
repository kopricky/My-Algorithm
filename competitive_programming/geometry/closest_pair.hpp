bool compyx(C c1,C c2)
{
    return c1.imag() != c2.imag() ? c1.imag() < c2.imag() : c1.real() < c2.real();
}

double closest_pair(C* a, int n)
{
    if(n<=1) return 1e100;
    int m=n/2;
    double x=a[m].real();
    double d=min(closest_pair(a,m),closest_pair(a+m,n-m));
    inplace_merge(a,a+m,a+n,compyx);
    vector<C> b;
    rep(i,n){
        if(abs(x-a[i].real())>=d) continue;
        rep(j,b.size()){
            C dp=a[i]-b[b.size()-1-j];
            if(dp.imag()>=d) break;
            d=min(d,abs(dp));
        }
        b.push_back(a[i]);
    }
    return d;
}
//最近点対を求める
double compute_shortest(C* a,int n)
{
    sort(a,a+n);
    return closest_pair(a,n);
}
