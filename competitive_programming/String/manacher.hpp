#include "../header.hpp"

#define MAX_N 100005

//iを中心とする最長の回文の半径をR[i]に格納(O(n))
// abaab を $a$b$a$a$b$ みたいにすると偶数長のもの求めることが可能
int R[MAX_N];

void manacher(string& S)
{
    int np=0,nr=0;
    while (np < (int)S.size()) {
        while(np-nr >= 0 && np+nr < (int)S.size() && S[np-nr] == S[np+nr]){
            nr++;
        }
        R[np] = nr;
        int k = 1;
        while(np-k >= 0 && np+k < (int)S.size() && k+R[np-k] < nr){
            R[np+k] = R[np-k];
            k++;
        }
        np += k; nr -= k;
    }
}
