#include "../header.hpp"

// セグ木で持てないくらい大きい範囲の区間更新のクエリに答える
// mpは(key=区間の左端:val=値)で初めに(-inf,-1)を詰めておく
// UpdateInterval.hppと違い[1,2),[2,3)みたいな区間が[1,3)にマージされているとは限らない
// マージされるように変更することは容易
template<typename CandType, typename ValType> class UpdateInterval
{
private:
    map<CandType, ValType> mp;
    CandType inf;
public:
    UpdateInterval() : inf(numeric_limits<CandType>::max()){
        mp[-inf] = -1;
    }
    // [l,r)の区間を値valに更新する
    void update(int l, int r, int val) {
        auto st = mp.lower_bound(l);
        auto ed = mp.upper_bound(r);
        CandType p = (--ed)->second;
        ed++;
        mp.erase(st, ed);
        mp[l] = val, mp[r] = p;
    }
};
