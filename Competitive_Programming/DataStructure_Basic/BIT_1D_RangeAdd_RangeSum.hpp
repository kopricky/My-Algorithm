#include "../header.hpp"

//区間加算,区間総和を2個のBITで表現する
template<typename T> class BIT {
private:
	int n;
	vector<T> bit;
public:
	// 0_indexed で i 番目の要素に x を加える
	void add(int i, T x){
		i++;
		while(i < n){
			bit[i] += x, i += i & -i;
		}
	}
	// 0_indexed で [0,i] の要素の和(両閉区間！！)
	T sum(int i){
		i++;
		T s = 0;
		while(i > 0){
			s += bit[i], i -= i & -i;
		}
		return s;
	}
	BIT(){}
	//初期値がすべて0の場合
	BIT(int sz) : n(sz+1), bit(n, 0){}
	BIT(vector<T>& v) : n((int)v.size()+1), bit(n, 0){
		for(int i = 0; i < n-1; i++){
			add(i,v[i]);
		}
	}
	void print(){
		for(int i = 0; i < n-1; i++){
			cout<<sum(i)-sum(i-1)<< " ";
		}
		cout<<endl;
	}
	//-1スタート
	void print_sum(){
		for(int i = 0; i < n; i++){
			cout<<sum(i-1)<<" ";
		}
		cout<<endl;
	}
};

template<typename T> class BIT_1D_RangeAdd_RangeSum {
private:
	int n;
	BIT<T> bitc, biti;
	void add(BIT<T>& bit, int l, int r, T val){
		bit.add(l, val), bit.add(r, -val);
	}
public:
	// 初期値がすべて0の場合
	BIT_1D_RangeAdd_RangeSum(int sz) : n(sz), bitc(sz), biti(sz){}
	void add(int l, int r, T val){
		add(bitc, l, r, val), add(biti, l, r, -val*(l-1)), add(biti, r, n, val*(r-l));
	}
	// [0, x] の和
	T sum(int x){
		return bitc.sum(x) * x + biti.sum(x);
	}
	// [l, r) の和
	T sum(int l, int r){
		return sum(r-1) - sum(l-1);
	}
	void print(){
		for(int i = 0; i < n; i++){
			cout << sum(i, i+1) << " ";
		}
		cout << "\n";
	}
};
