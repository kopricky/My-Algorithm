//2〜MAX_Nのすべての数の素因数分解を計算(O(MAX_Nlog(MAX_N)))
vi pr[MAX_N],id[MAX_N];
bool is_prime[MAX_N];

void allfac(int n)
{
    fill(is_prime,is_prime+n+1,true);
    for(int i=2;i<=n;i++){
        if(is_prime[i]){
            pr[i].pb(i);
            id[i].pb(1);
            for(int j=2*i;j<=n;j+=i){
                is_prime[j] = false;
                int cnt = 0;
                int nw = j;
                while(nw % i == 0){
                    nw /= i;
                    cnt++;
                }
                pr[j].pb(i);
                id[j].pb(cnt);
            }
        }
    }
}
