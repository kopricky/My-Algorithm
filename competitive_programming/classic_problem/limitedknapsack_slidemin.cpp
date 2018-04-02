//個数制約付きナップザック(O(nW))
class knapsaq
{
public:
    int res;
    //価値v,重さw,個数m,重さの上限W
    knapsaq(vector<int> v,vector<int> w,vector<int> m,int W){
        int n = (int)v.size();
        vector<int> dp(W+1),deq(W+1),deqv(W+1);
        rep(i,n){
            rep(a,w[i]){
                int s = 0,t = 0;
                for(int j = 0; j*w[i]+a <= W; j++){
    				int val = dp[j*w[i]+a] - j*v[i];
    				while(s < t && deqv[t-1] <= val){
    					t--;
    				}
    				deq[t] = j;
    				deqv[t++] = val;
    				dp[j*w[i]+a] = deqv[s] + j*v[i];
    				if(deq[s] == j-m[i]){
    					s++;
    				}
    			}
            }
        }
        res = dp[W];
    }
};
