#include "../header.hpp"

const double PI = 4.0*atan(1.0);

vector<complex<double> > fft(vector<complex<double> > a,bool rev = false)
{
    const double PI = std::acos(-1);
    int n = a.size(),h = 0;
    for(int i = 0; 1 << i < n; i++) h++;
    rep(i,n){
        int j = 0;
        rep(k,h) j |= (i >> k & 1) << (h-1-k);
        if (i < j) swap(a[i], a[j]);
    }
    for(int i = 1; i < n; i *= 2) {
        rep(j,i){
            complex<double> w = polar(1.0,2*PI/(i*2)*(rev?-1:1)*j);
            for(int k = 0; k < n; k += i * 2) {
                complex<double> s = a[j+k];
                complex<double> t = a[j+k+i]*w;
                a[j+k+0] = s+t;
                a[j+k+i] = s-t;
            }
        }
    }
    if (rev) rep(i,n) a[i] /= n;
    return a;
}

vector<int> mul(vector<int> a,vector<int> b)
{
    int s = (int)a.size() + (int)b.size() - 1,t = 1;
    while (t < s) t *= 2;
    vector<complex<double> > A(t), B(t);
    rep(i,a.size()) A[i].real(a[i]);
    rep(i,b.size()) B[i].real(b[i]);
    A = fft(A),B = fft(B);
    rep(i,t) A[i] *= B[i];
    A = fft(A, true);
    a.resize(s);
    //整数に直す
    rep(i,s) a[i] = round(A[i].real());
    return a;
}

int main()
{
    // cin.tie(0);
    // ios::sync_with_stdio(false);
    int n;
    scanf("%d",&n);
    vector<int> a(n+1,0),b(n+1,0);
    rep(i,n){
        scanf("%d%d",&a[i+1],&b[i+1]);
    }
    a = mul(a,b);
    rep(i,2*n){
        cout << a[i+1] << "\n";
    }
    return 0;
}
