template<typename T> class knapsack{
public:
    int V,W;
    int id;
    vector<T> v,deqv;
    //dpテーブル使い回し
    vi dp,deq,w,m;
    knapsack(int node_size,int max_w){
        V = node_size, W = max_w, id = 0;
        dp.resize(W+1,0),deq.resize(W+1),deqv.resize(W+1);
        v.resize(V),w.resize(V),m.resize(V);
    }
    void add_goods(T value,int weight,int cnt){
        v[id] = value, w[id] = weight, m[id++] = cnt;
    }
    T solve(){
        rep(i,V){
    		rep(a,w[i]){
    			int s=0,t=0;
    			for(int j=0;j*w[i]+a<=W;j++){
    				ll val = dp[j*w[i]+a]-j*v[i];
    				while(s<t&&deqv[t-1]<=val){
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
        return dp[W];
    }
};
