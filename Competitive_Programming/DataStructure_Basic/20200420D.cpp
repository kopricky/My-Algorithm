#include <bits/stdc++.h>
#define rep(i,n) for(int i = 0; i < (int)(n); ++i)
#define ll long long
#define show(x) cout<<#x<<" = "<<(x)<<endl

using namespace std;

const int MAX_N = 400005;

class BIT {
private:
    int n;
    vector<int> bit;
public:
    void add(int i, int x){
        ++i;
        while(i < n) bit[i] += x, i += i & -i;
    }
    int sum(int i){
        ++i;
        int s = 0;
        while(i > 0) s += bit[i], i -= i & -i;
        return s;
    }
    int upper_bound(int x){
        int b = 1;
        while(b < n) b *= 2;
        int a = 0;
        while(b > 0){
            if(a + b < n - 1 && x >= bit[a + b]){
                x -= bit[a + b], a += b;
            }
            b >>= 1;
        }
        return (a < b) ? a : -1;
    }
    BIT(const int sz) : n(sz + 1), bit(n, 0){}
};

vector<pair<int, int> > info[MAX_N];

int main(){
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n;
    rep(i,n){
        cin >> u >> v;
    }
}