#include "../header.hpp"

// きたまさ法を用いて k 項間線形漸化式の n 番目 a[n] を O(k^2logn) で求める
// 定数項のある場合にも対応している
// solve(c, x)
// 第1引数(係数) a[k] = c[0] * a[k-1] + c[1] * a[k-2] + ... + c[k-1] * a[0] + c[k]
// 第2引数(初期値) a[0] = x[0], a[1] = x[1], ... , a[k-1] = x[k-1]

template<typename T>
void rec(const vector<T>& c, vector<T>& res, const long long n, const int k)
{
    if(n < k){
        res[k-1-n] = 1;
        return;
    }
    rec(c, res, n/2, k);
    vector<T> num(k+1, 0), sm(k+1, 0);
    vector<T> tmp = res;
    int flag = n % 2;
    if(!flag) for(int i = 0; i <= k; i++) sm[i] = res[i] * tmp[k-1];
    for(int i = 0; i < k - !flag; i++){
        for(int j = 0; j <= k-1; j++){
            num[j] = res[0] * c[j] + res[j+1];
        }
        num[k-1] = res[0] * c[k-1], num[k] = res[0] * c[k] + res[k];
        for(int j = 0; j <= k; j++){
            sm[j] += num[j] * tmp[k-2-i+flag], res[j] = num[j];
        }
    }
    sm[k] += tmp[k];
    res = sm;
}

template<typename T>
T solve(vector<T>& c, vector<T>& x, const long long n)
{
    int k = (int)x.size();
    vector<T> res(k+1, 0);
    rec(c, res, n, k);
    T ans = 0;
    for(int i = 0; i < k; i++) ans += res[i] * x[k-1-i];
    return ans + res[k];
}
