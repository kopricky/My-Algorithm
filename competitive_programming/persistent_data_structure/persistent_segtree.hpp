#include "../header.hpp"

// segtreeの完全永続化(更新される部分に新しいノード(O(log(n))個)を構築)

template<typename T> class segtree {
public:
	segtree(int n, T e) : E(e), N(1 << (int)log2(n * 2 - 1)) {}

	void update(int k, T val) {
		root = update(k, val, root, 0, N);
	}

	T query(int a, int b) {
		return query(a, b, root, 0, N);
	}

private:
	struct node {
		node *left = nullptr, *right = nullptr;
		T val;
	};
	const T E;
	const int N;

	node* root = nullptr;

	T get(node* x) {
		return x ? x->val : E;
	}
	T func(T arg1, T arg2)
	{
		return min(arg1,arg2);
	}
	node* update(int k, T val, node* x, int l, int r) {
		x = x ? new node(*x) : new node();
		if (r - l == 1) {
			x->val = val;
			return x;
		}
		int m = (l + r) / 2;
		if(k < m) {
			x->left = update(k, val, x->left, l, m);
		}else{
			x->right = update(k, val, x->right, m, r);
		}
		x->val = func(get(x->left), get(x->right));
		return x;
	}

	T query(int a, int b, node* x, int l, int r) {
		if (!x) return E;
		if (r <= a || b <= l) return E;
		if (a <= l && r <= b) return x->val;
		int m = (l + r) / 2;
		T vl = query(a, b, x->left, l, m);
		T vr = query(a, b, x->right, m, r);
		return func(vl, vr);
	}
};
