class BinaryTrie {
public:
	struct Node {
		Node *next[2];
		int sub;
		Node() : sub(0) { next[0] = next[1] = nullptr; }
	};
	Node* root;
	BinaryTrie(){
		root = new Node();
	}
	//Trie木にxを加える
	void add(int x) {
		Node *curr = root;
		rrep(i,30){
			int y = x >> i & 1;
			if (!curr->next[y]) {
				curr->next[y] = new Node();
			}
			curr->sub++;
			curr = curr->next[y];
		}
		curr->sub++;
	}
    //何らかのクエリ
	int query(Node *curr,int d) {
		if (!curr) return 0;
		return query(curr->next[0],d-1) + query(curr->next[1],d-1);
	}
};
