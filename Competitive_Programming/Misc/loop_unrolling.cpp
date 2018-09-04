// loop-unrolling のコードを生成するコード
// i,j,kループについて行うべき処理を一行実装しかつ各添字i,j,kの前に!をつける
// 入力は
// 処理 (一行)
// x y z (i,j,kのアンローリングの段数)
// の形

#include <bits/stdc++.h>
#define ll long long
#define rep(i,n) for(int i=0;i<(int)(n);++i)
#define len(v) (int)(v).size()
#define pb push_back

using namespace std;

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    string s;
    getline(cin,s);
    int x,y,z;
    cin >> x >> y >> z;
    vector<vector<vector<string> > > res(x,vector<vector<string> >(y,vector<string>(z)));
    rep(i,len(s)){
        if(s[i] == '!'){
            if(s[i+1] == 'i'){
                rep(k,y){ rep(l,z){ rep(j,x){
                    if(j) res[j][k][l].pb('i'), res[j][k][l].pb('+'), res[j][k][l] += to_string(j);
                    else res[j][k][l].pb('i');
                } } }
            }else if(s[i+1] == 'j'){
                rep(j,x){ rep(l,z){ rep(k,y){
                    if(k) res[j][k][l].pb('j'), res[j][k][l].pb('+'), res[j][k][l] += to_string(k);
                    else res[j][k][l].pb('j');
                } } }
            }else{
                rep(j,x){ rep(k,y){ rep(l,z){
                    if(l) res[j][k][l].pb('k'), res[j][k][l].pb('+'), res[j][k][l] += to_string(l);
                    else res[j][k][l].pb('k');
                } } }
            }
            i++;
        }else{
            rep(j,x){ rep(k,y){ rep(l,z){
                res[j][k][l].pb(s[i]);
            } } }
        }
    }
    rep(i,x){ rep(j,y){ rep(k,z){
        cout << res[i][j][k] << "\n";
    } } }
    return 0;
}
