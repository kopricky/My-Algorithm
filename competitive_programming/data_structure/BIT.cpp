template<typename V> class BIT {
private:
	int n; vector<V> bit;
public:
	void add(int i,V x){ i++; while(i < n) bit[i] += x, i += i & -i;}
	V sum(int i){ i++; V s = 0; while(i>0) s += bit[i], i -= i & -i; return s;}
	BIT(){} BIT(int sz){ n = sz + 1, bit.resize(n,0);} //初期値がすべて0の場合
	BIT(vector<V> v){ n = (int)v.size()+1; bit.resize(n); rep(i,n-1) add(i,v[i]);}
	void print(){ rep(i,n-1)cout<<sum(i)-sum(i-1)<< " ";cout<<endl;}
	void print_sum(){ rep(i,n)cout<<sum(i-1)<<" ";cout<<endl;}	//-1スタート
};
