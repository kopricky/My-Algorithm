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
			cout << sum(i) - sum(i-1) << " ";
		}
		cout << "\n";
	}
	//-1スタート
	void print_sum(){
		for(int i = 0; i < n; i++){
			cout << sum(i-1) << " ";
		}
		cout << "\n";
	}
};

//u を昇順にソートするのに必要な交換回数(転倒数) (u は (0~n-1の並び替え))
long long inv_count(vector<int>& u)
{
	int n = (int)(u.size());
	BIT<int> bt(n);
	long long ans = 0;
	for(int i = 0; i < n; i++){
		ans += i-bt.sum(u[i]);
		bt.add(u[i],1);
	}
	return ans;
}

//u を v に変換するのに必要な交換回数 (u, v は (0~n-1の並び替え))
long long inv_count(vector<int>& u,vector<int>& v)
{
	int n = (int)(u.size());
	vector<int> p(n);
	BIT<int> bt(n);
	long long ans = 0;
	for(int i = 0; i < n; i++){
		p[v[i]] = i;
	}
	for(int i = 0; i < n; i++){
		ans += i-bt.sum(p[u[i]]);
		bt.add(p[u[i]],1);
	}
	return ans;
}
