#include "../header.hpp"

// 直線の管理, 座標の値は int 型を仮定

class line
{
public:
    int a, b;
    long long c;
    line(){}
    line(int a_, int b_, ll c_) : a(a_), b(b_), c(c_){}
    // (p,q), (r,s) を結ぶ直線
    line(int p, int q, int r, int s){
		if(p == r){
			a = 1, b = 0, c = -p;
		}else if(q == s){
			a = 0, b = 1, c = -q;
		}else{
            a = s - q, b = -(r - p), c = (long long)q * r - (long long)p * s;
		}
    }
    line(P x, P y) : line(x.first, x.second, y.first, y.second){}
    bool operator< (const line& l) const {
        return (a == l.a)?((b == l.b)?(c < l.c):(b < l.b)):(a < l.a);
    }
    bool operator> (const line& l) const {
        return (a == l.a)?((b == l.b)?(c > l.c):(b > l.b)):(a > l.a);
    }
    bool operator== (const line& l) const {
        return (a == l.a)?((b == l.b)?(c == l.c):false):false;
    }
    bool operator!= (const line& l) const {
        return !(*this == l);
    }
    // 点(p, q) が直線上にあるか
    inline bool onLine(int p, int q){
        return ((long long)a * p + (long long)b * q + c == 0);
    }
    inline bool onLine(P x){ return onLine(x.first, x.second); }
};
