//iを中心とする最長の回文の半径をR[i]に格納
int rad[MAX_N];

void manacher(string& S)
{
    int np=0,nr=0;
    while (np < S.size()) {
        while(np-nr >= 0 && np+nr < S.size() && S[np-nr] == S[np+nr]){
            nr++;
        }
        R[np] = nr;
        int k = 1;
        while(np-k >= 0 && np+k < S.size() && k+R[np-k] < nr){
            R[np+k] = R[np-k];
            k++;
        }
        np += k; nr -= k;
    }
}
