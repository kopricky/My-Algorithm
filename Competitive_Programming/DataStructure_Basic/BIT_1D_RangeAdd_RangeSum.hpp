#include "../header.hpp"

//区間加算, 区間総和を 2 個のBITで表現する
template<typename T> class BIT {
private:
	int n;
	vector<T> bit;
public:
	void add(int i, const T x){
		i++;
		while(i < n){
			bit[i] += x, i += i & -i;
		}
	}
	T sum(int i) const {
		i++;
		T s = 0;
		while(i > 0){
			s += bit[i], i -= i & -i;
		}
		return s;
	}
	BIT(const int sz) : n(sz + 1), bit(n, 0){}
};

template<typename T> class BIT_1D_RangeAdd_RangeSum {
private:
	int n;
	BIT<T> bitc, biti;
public:
	// 初期値はすべて 0
	BIT_1D_RangeAdd_RangeSum(const int sz) : n(sz), bitc(sz), biti(sz){}
	// [l, r) に val を加算
	void add(const int l, const int r, const T val){
		bitc.add(l, val), bitc.add(r, -val);
		biti.add(l, -val * (l - 1)), biti.add(r, val * (r - 1));
	}
	// [0, x] の和
	T sum(const int x) const {
		return bitc.sum(x) * x + biti.sum(x);
	}
	// [l, r) の和
	T sum(const int l, const int r) const {
		return sum(r-1) - sum(l-1);
	}
	void print(){
		for(int i = 0; i < n; i++){
			cout << sum(i, i+1) << " ";
		}
		cout << "\n";
	}
};
