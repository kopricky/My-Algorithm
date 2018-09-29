#include "../header.hpp"

#define MAX_N 100000

//2〜MAX_N-1のすべての数の素因数分解を計算(O(MAX_Nlog(MAX_N)))
vector<int> pr[MAX_N],id[MAX_N];
bool is_prime[MAX_N];

void all_fac()
{
    fill(is_prime,is_prime+MAX_N,true);
    for(int i=2;i<MAX_N;i++){
        if(is_prime[i]){
            pr[i].push_back(i);
            id[i].push_back(1);
            for(int j=2*i;j<MAX_N;j+=i){
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
