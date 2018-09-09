#include "../header.hpp"

#define MAX_N 100000

//2〜MAX_Nのすべての数の素因数分解を計算(O(MAX_Nlog(MAX_N)))
vector<int> pr[MAX_N],id[MAX_N];
bool is_prime[MAX_N];

void all_fac(int n)
{
    fill(is_prime,is_prime+n+1,true);
    for(int i=2;i<=n;i++){
        if(is_prime[i]){
            pr[i].push_back(i);
            id[i].push_back(1);
            for(int j=2*i;j<=n;j+=i){
                is_prime[j] = false;
                int cnt = 0;
                int nw = j;
                while(nw % i == 0){
                    nw /= i;
                    cnt++;
                }
                pr[j].push_back(i);
                id[j].push_back(cnt);
            }
        }
    }
}
