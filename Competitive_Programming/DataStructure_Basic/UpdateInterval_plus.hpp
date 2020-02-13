#include "../header.hpp"

// セグ木で持てないくらい大きい範囲の区間更新のクエリに答える
// mpは(key=区間の左端:val=値) でコンストラクタで(-inf, 番兵)を詰めている(適宜変更する)
// Strict を true にすると隣り合う区間の値は常に異なるようになる(等しい値の区間はちゃんと merge される)
template<typename PositionType, class DataType, bool Strict=false> class UpdateInterval {
public:
	map<PositionType, DataType> interval;
	UpdateInterval(const DataType& sentinel)
	 	: interval({{numeric_limits<PositionType>::min(), sentinel}}){}
	void update(const PositionType& left, const PositionType& right, const DataType& _data){
		auto st = interval.lower_bound(left);
		auto ed = interval.lower_bound(right);
		bool add = (ed == interval.end() || ed->first != right);
        bool _update = ((--st)->second != _data);
		const DataType kp = (--ed)->second;
		interval.erase(++st, ++ed);
		if(!Strict || _update) interval.emplace(left, _data);
		if(add){
			if(!Strict || (kp != _data)) interval.emplace(right, kp);
		}else if(Strict){
			ed = interval.lower_bound(right);
			if(ed->second == _data) interval.erase(ed);
		}
	}
	DataType& query(const PositionType& pos){
		auto it = interval.upper_bound(pos);
		return (--it)->second;
	}
	void print() const {
		for(const auto& e : interval){
			cout << "{" << e.first << "," << e.second << "}" << " ";
		}
        cout << "\n";
	}
};
