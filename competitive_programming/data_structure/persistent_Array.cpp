//永続配列
const int CH_SIZE = 8;
const int LOG_CH = 3;

template<typename T> class Array {
public:
	Array(){}
	Array(int n){
		h = 0;
		for (int i = 1; i < n; i *= CH_SIZE) h += LOG_CH;
	}
	T* mutable_get(int k){
		auto p = mutable_get(k, root, 0, h);
		root = p.first;
		return &p.second->value;
	}
	T operator[](int k){
		return immutable_get(k, root, 0, h)->value;
	}

private:
	struct node {
		node* ch[CH_SIZE] = {};
		T value;
		node() {}
		node(T value) : value(value) {}
	};
	node* immutable_get(int a, node* x, int l, int d){
		if (d == 0) return x;
		int id = (a - l) >> (d - LOG_CH);
		return immutable_get(a, x->ch[id], l + (id << (d - LOG_CH)), d - LOG_CH);
	}
	pair<node*, node*> mutable_get(int a, node* x, int l, int d){
		x = (x ? new node(*x) : new node());
		if (d == 0) return make_pair(x, x);
		int id = (a - l) >> (d - LOG_CH);
		auto p = mutable_get(a, x->ch[id], l + (id << (d - LOG_CH)), d - LOG_CH);
		x->ch[id] = p.first;
		return make_pair(x, p.second);
	}

    int h;
    node* root = nullptr;
};
