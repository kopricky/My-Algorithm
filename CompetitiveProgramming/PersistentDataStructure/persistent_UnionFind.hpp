#include "../header.hpp"

const int CH_SIZE = 8;
const int LOG_CH = 3;

class Array {
public:
	Array() {}
	Array(int n) {
		h = 0;
		for (int i = 1; i < n; i *= CH_SIZE) h += LOG_CH;
	}
	int* mutable_get(int k,bool del){   //k以下の部分木のコピーを得る
		auto p = mutable_get(k, root, 0, h);
		if(del){
			delete root;
		}
		root = p.first;
		return &p.second->value;
	}
	int operator[](int k){
		node* res = immutable_get(k, root, 0, h);
		return res ? res->value : -1;
	}

private:
	struct node {
		node *ch[CH_SIZE] = {};
		int value = -1;
	};
	int h;
	node* root = nullptr;
	node* immutable_get(int a, node* x, int l, int d) {
		if(!x) return x;
		if(d == 0) return x;
		int id = (a - l) >> (d - LOG_CH);
		return immutable_get(a, x->ch[id], l + (id << (d - LOG_CH)), d - LOG_CH);
	}
	pair<node* ,node*> mutable_get(int a, node* x, int l, int d) {
		x = (x ? new node(*x) : new node());  //xをコピーもしくは新しいノードを確保
		if(d == 0) return make_pair(x, x);
		int id = (a - l) >> (d - LOG_CH);
		auto p = mutable_get(a, x->ch[id], l + (id << (d - LOG_CH)), d - LOG_CH);
		x->ch[id] = p.first;
		return make_pair(x, p.second);
	}
};

struct UnionFind {
	Array uf;
	UnionFind() : uf(0){}
	UnionFind(int n) : uf(n){}
	int root(int x){
		int nd = uf[x];
		if (nd < 0) return x;
		return root(nd);
	}
	bool same(int x,int y){
        return root(x) == root(y);
    }
	void merge(int x, int y){
		x = root(x);
		y = root(y);
		if(x == y) return;
		int* u = uf.mutable_get(x,false);
		int* v = uf.mutable_get(y,true);
		if(-*u < -*v) swap(u, v), swap(x, y);
		*u += *v;
		*v = x;
	}
	int size(int x){
		return -uf[root(x)];
	}
};
