#include "../header.hpp"

template<typename T>
vector<int> Hungarian(const vector<vector<T> > cost){
    int n = (int)cost.size(), m = (int)cost[0].size();
    vector<int> r(n, -1), l(m, -1);
    vector<T> lf(n, 0), rf(m, 0);
    auto residue = [&](const int i,const int j){
        return cost[i][j] + lf[i] + rf[j];
    };
    rep(i,n){
        vector<bool> left(n, false), right(m, false);
        vector<int> trace(m, -1), ptr(m, i);
        left[i] = true;
        while(true){
            T d = numeric_limits<T>::max();
            rep(j,m) if(!right[j]) d = min(d, residue(ptr[j], j));
            rep(j,n) if(left[j]) lf[j] -= d;
            rep(j,m) if(right[j]) rf[j] += d;
            int b = -1;
            rep(j,m) if(!right[j] && residue(ptr[j], j) == 0) b = j;
            trace[b] = ptr[b];
            int c = l[b];
            if(c < 0){
                while(b >= 0){
                    const int a = trace[b], z = r[a];
                    l[b] = a, r[a] = b, b = z;
                }
                break;
            }
            right[b] = left[c] = true;
            rep(j,m){
                if(residue(c, j) < residue(ptr[j], j)){
                    ptr[j] = c;
                }
            }
        }
    }
    return r;
}
