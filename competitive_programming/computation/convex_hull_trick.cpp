//f[j](x)=a[j]x+b[j]でaがjの増加に伴い単調減少する場合
//0<=j<=i-kを満たす直線f[j](x)についてminをとる
//maxをとる場合はcheckの不等号とsolve内のsの移動の部分の不等号を逆にする
template<typename T> class CHT
{
public:
    //deqには直線のインデックスを格納
    //dpの初期値はINFづめしている
    vector<int> deq;
    vector<T> a,b;
    vector<T> S,dp;
    int n,k;
    CHT(vector<T> A,vector<T> B,int sz){
        a = A,b = B;
        n = (int)a.size(), k = sz;
        deq.resize(n),S.resize(n+1,0),dp.resize(n+1,INF);
        rep(i,n){
            S[i+1] = S[i] + a[i];
        }
    }
    T func(int j,int x){
        return a[j]*x+b[j];
    }
    bool check(int j1, int j2, int j3){
        return (a[j2]-a[j1])*(b[j3]-b[j2]) >= (b[j2]-b[j1])*(a[j3]-a[j2]);
    }
    //今回の場合は0<=j<=i-kを満たす直線f[j](x)についてminをとっている
    T solve(){
        dp[0] = 0, deq[0] = 0;
        int s = 0, t = 1;
        //今回の場合はb[i]の値がdp[i]の値に依存するのでb[i]を更新する
        rep(i,k){
            b[i] += dp[i];
        }
        //今回の問題の場合i=k~2k-1まではj=0の場合が最適であることが明らかなので
        //i-k<kの間はdeqに新たに直線を追加することはしない
        for(int i=k;i<=n;i++){
            if(i-k >= k){
                //末端から最小値を取る可能性がなくなったものを取り除く
                while(s+1 < t && check(deq[t-2],deq[t-1],i-k)){
                    t--;
                }
                deq[t++] = i-k;
            }
            //先頭が最小値でなければ取り除く
            while(s+1 < t && func(deq[s],i) >= func(deq[s+1],i)){
                s++;
            }
            dp[i] = -S[i] + func(deq[s],i);
            //今回の場合はb[i]の値がdp[i]の値に依存するのでb[i]を更新する
            if(i < n){
                b[i] += dp[i];
            }
        }
        return dp[n];
    }
};
