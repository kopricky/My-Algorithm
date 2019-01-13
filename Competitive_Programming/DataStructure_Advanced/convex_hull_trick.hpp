#include "../header.hpp"

//f[j](x) = a[j]x + b[j]でaがjの増加に伴い単調減少する場合
//クエリのxが単調でない場合はコメントアウトしたにぶたんの方を用いる
template<typename T> class CHT
{
private:
    using ptt = pair<T, T>;
    bool check(ptt l1, ptt l2, ptt l3){
        return (l2.first-l1.first)*(l3.second-l2.second)>=(l2.second-l1.second)*(l3.first-l2.first);
    }
    T f(int i, T x){
        return lines[i].first * x + lines[i].second;
    }
public:
    vector<ptt> lines;
    int head;
    CHT(): head(0){};
    void add(T a, T b){
        ptt line(a, b);
        while((int)lines.size() - head >= 2 && check(*(lines.end()-2), lines.back(), line)){
            lines.pop_back();
        }
        lines.emplace_back(line);
    }
    T get(T x){
        while((int)lines.size() - head >= 2 && f(head, x) >= f(head + 1, x)){
            head++;
        }
        return f(head, x);
        // int low = -1, high = lines.size() - 1;
        // while (high - low > 1) {
        // 	int mid = (high + low) / 2;
        //     if(f(mid, x) >= f(mid+1, x)){
        //         low = mid;
        //     }else{
        //         high = mid;
        //     }
        // }
        // return f(high, x);
    }
};
