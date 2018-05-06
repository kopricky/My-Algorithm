#include "../header.hpp"

//長さnのArrayから連続k個の最小値の配列をresに格納
template <typename T> class SlideMin
{
public:
    vector<int> deq;
    vector<T> res;
    int n,k;
    SlideMin(vector<T> Array,int sz){
        n = (int)Array.size(), k = sz;
        deq.resize(n), res.resize(n-k+1);
        int s = 0, t = 0;
        rep(i,n){
            //iを追加した場合に添字deq[t-1]の値がa[i]以上のときdeqから削除されるまで採用されることはないので
            //最大値を取る場合は Array[deq[t-1]] <= Array[i] とする
            while(s < t && Array[deq[t-1]] >= Array[i]){
                t--;
            }
            deq[t++] = i;
            if(i-k+1 >= 0){
                res[i-k+1] = Array[deq[s]];
                if(deq[s] == i-k+1){
                    s++;
                }
            }
        }
    }
};
