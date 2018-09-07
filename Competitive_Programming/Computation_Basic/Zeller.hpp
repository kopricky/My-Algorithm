#include "../header.hpp"

// グレゴリオ暦y年m月d日の曜日(mod7)を返す(Zellerの公式)
// 日曜日始まりで日曜日が0
int zeller(int y, int m, int d){
    if(m <= 2){
        y--;
        m += 12;
    }
    return (y + y/4 - y/100 + y/400 + (13*m+8)/5 + d) % 7;
}
