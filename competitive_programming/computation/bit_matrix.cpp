// bit行列の掛け算(推移行列を用いた到達可能性判定などに使える)
struct mat
{
    bitset<MAX_N> a[MAX_N];
};

mat operator*(const mat& m1,const mat& m2){
    mat ans;
    rep(i,MAX_N){
        rep(j,MAX_N){
            if(m1.a[i][j] == 0) continue;
            ans.a[i] |= m2.a[j];
        }
    }
    return ans;
}
