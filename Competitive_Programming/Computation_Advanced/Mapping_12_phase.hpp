#include "../header.hpp"
//写像12相のうち難しめのものたちのアルゴリズム
//一部のみverify済み

#define MOD 1000000007

const int MAX_N = 1005;

long long inv[MAX_N],fac[MAX_N],finv[MAX_N];

void make()
{
	fac[0] = fac[1] = 1;
	finv[0] = finv[1] = 1;
	inv[1] = 1;
	for(int i=2;i<MAX_N;i++){
		inv[i] = MOD - inv[MOD%i] * (MOD/i) % MOD;
		fac[i] = fac[i-1] * (long long) i % MOD;
		finv[i] = finv[i-1] * inv[i] % MOD;
	}
}

long long comb(int a,int b)
{
	if(a<b){
		return 0;
	}
	return fac[a] * (finv[b] * finv[a-b] % MOD) % MOD;
}

long long mod_pow(long long a,long long b)
{
    a %= MOD;
    long long res = 1;
    while(b){
        if(b & 1){
            res = res * a % MOD;
        }
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

long long add(long long x,long long y)
{
    return (x + y)%MOD;
}

long long sub(long long x,long long y)
{
    return (x+MOD-y)%MOD;
}

long long mul(long long x,long long y)
{
    return x*y%MOD;
}

//(１)ボール,箱を区別し、各箱に最低でも１つ以上入れる場合
//包除原理を用いて解ける(O(mlog(n)))

int count1(int n, int m)
{
    if(n < m) return 0;
    make();
    long long res = 0;
    for(int i = 1; i <= m; i++){
        // i個以下の箱にn個のボールを入れる場合の数
        int tmp = mul(comb(m,i), mod_pow(i,n));
        if((m-i)%2) res = sub(res, tmp);
        else res = add(res, tmp);
    }
    return res;
}
//(２)ボールは区別し、箱を区別せず、入れ方に制限がない場合
//n=mのときベル数と呼ばれる(O(n*m))

int S[MAX_N][MAX_N];

void Stirling(int n, int m)
{
    S[0][0] = 1;
    for(int i = 0; i < n; i++){
		for(int j = 0; j < min(m,i+1); j++){
            S[i+1][j+1] = add(S[i][j],mul(j+1,S[i][j+1]));
        }
    }
}

int count2(int n, int m)
{
    Stirling(n, m);
    int res = 0;
    for(int i = 1; i <= m; i++) res = add(res, S[n][i]);
    return res;
}

//(3)ボールは区別し、箱を区別せず、各箱に最低でも１つ以上入れる場合
//第二種スターリング数と呼ばれ,(1)の値をm!で割った値に等しい(O(mlog(n)))

int count3(int n, int m)
{
    int res = count1(n, m);
    return mul(res, finv[m]);
}

//以下はO(n*m)のDP解法((2)の前半部分と同じ)
// int S[MAX_N][MAX_N];
// void Stirling(int n, int m)
// {
//     S[0][0] = 1;
//     rep(i,n){
//         rep(j,min(m,i+1)){
//             S[i+1][j+1] = add(S[i][j],mul(j+1,S[i][j+1]));
//         }
//     }
// }

//(4)ボール,箱を区別せず,入れ方に制限がない場合
//nをm個の非負整数の和に分割する場合の数(O(n*m))
//n=mのとき分割数と呼ばれる

int dp[MAX_N][MAX_N];

int count4(int n, int m)
{
   dp[0][0] = 1;
   for(int i = 0; i <= n; i++){
       for(int j = 0; j < m; j++){
           dp[i][j+1] = add(dp[i][j+1], dp[i][j]);
           if(i > j) dp[i][j+1] = add(dp[i][j+1], dp[i-j-1][j+1]);
       }
   }
   return dp[n][m];
}

//(5)ボール,箱を区別せず,各箱に最低でも１つ以上入れる場合
//nをm個の和の自然数に分割する場合の数(O(n*m))

int count5(int n, int m)
{
    if(n < m) return 0;
    return count4(n-m, m);
}
