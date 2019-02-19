#include "../header.hpp"

// セグ木で持てないくらい大きい範囲の区間更新のクエリに答える
// mpは(key=区間の左端:val=値)でコンストラクタで(-inf,0)を詰めている(適宜変更する)
template<typename CandType, typename ValType> class UpdateInterval
{
public:
    map<CandType, ValType> interval;
    UpdateInterval(){
        interval[numeric_limits<CandType>::min()] = 0;
    }
    // [l,r) の区間を値 val に更新する
    void update(CandType l, CandType r, ValType val) {
        auto st = interval.lower_bound(l);
        auto ed = interval.upper_bound(r);
        CandType p = (--ed)->second;
        ed++;
        CandType nl = l;
        while(st != interval.begin()){
            --st;
            if(st->second != val){
                st++;
                break;
            }
            nl = st->first;
        }
        interval.erase(st, ed);
        interval[nl] = val, interval[r] = p;
    }
};
