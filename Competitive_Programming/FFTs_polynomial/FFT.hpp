#include "../header.hpp"

complex<double> operator*(const complex<double> a, const complex<double> b)
{
    return complex<double>(a.real()*b.real()-a.imag()*b.imag(),a.real()*b.imag()+a.imag()*b.real());
}

void fft(vector<complex<double> >& a,bool rev = false)
{
    constexpr double PI = std::acos(-1);
    unsigned int i, j, k, l;
    double r;
    complex<double> p, q, s;
    const unsigned int size = a.size();
    if(size == 1) return;
    vector<complex<double> > b(size);
    r = rev ? (- 2 * PI / size) : 2 * PI / size;
    s = polar(1.0, r);
    vector<complex<double> > kp(size / 2 + 1, 1);
    for(i = 0; i < size / 2; ++i) kp[i + 1] = kp[i] * s;
    for(i = 1, l = size / 2; i < size; i <<= 1, l >>= 1){
        for(j = 0, r = 0; j < l; ++j, r += i){
            for(k = 0, s = kp[i * j]; k < i; ++k){
                p = a[k + r], q = a[k + r + size / 2];
                b[k + 2 * r] = p + q;
                b[k + 2 * r + i] = (p - q) * s;
            }
        }
        swap(a, b);
    }
    if(rev){
        for(i = 0; i < size; i++){ a[i] /= size; }
    }
}
 
vector<int> mul(const vector<int>& a, const vector<int>& b)
{
    int s = (int)a.size() + (int)b.size() - 1, t = 1;
    while(t < s) t *= 2;
    vector<complex<double> > A(t), B(t);
    for(int i = 0; i < (int)a.size(); i++) A[i].real(a[i]);
    for(int i = 0; i < (int)b.size(); i++) B[i].real(b[i]);
    fft(A), fft(B);
    for(int i = 0; i < t; i++) A[i] *= B[i];
    fft(A, true);
    vector<int> res(s);
    for(int i = 0; i < s; i++) res[i] = round(A[i].real());
    return res;
}