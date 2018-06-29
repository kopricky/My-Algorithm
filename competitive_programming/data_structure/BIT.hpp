#include "../header.hpp"

template<typename T> class BIT {
private:
	int n; vector<T> bit;
public:
	void add(int i,T x){ i++; while(i < n) bit[i] += x, i += i & -i;}
	T sum(int i){ i++; T s = 0; while(i>0) s += bit[i], i -= i & -i; return s;} //0_indexedで[0,i]の要素の和
	BIT(){} BIT(int sz){ n = sz + 1, bit.resize(n,0);} //初期値がすべて0の場合
	BIT(vector<T>& v){ n = (int)v.size()+1; bit.resize(n, 0); rep(i,n-1) add(i,v[i]);}
	void print(){ rep(i,n-1)cout<<sum(i)-sum(i-1)<< " ";cout<<endl;}
	void print_sum(){ rep(i,n)cout<<sum(i-1)<<" ";cout<<endl;}	//-1スタート
};
