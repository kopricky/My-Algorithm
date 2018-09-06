#include "../header.hpp"

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
