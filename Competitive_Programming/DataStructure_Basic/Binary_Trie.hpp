#include "../header.hpp"

//2分トライ木

class BinaryTrie {
public:
	struct Node {
		Node *next[2];
		int sub;
		Node() : sub(0) { next[0] = next[1] = nullptr; }
	};
	Node* root;
	//2^30より小さい場合
    const int MAX_BIT = 30;
	BinaryTrie(){
		root = new Node();
	}
	//Trie木にxを加える
	void add(int x) {
		Node *curr = root;
        for(int i = MAX_BIT-1; i >= 0; i--){
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
	int query(Node *curr,const int a,int d){
		if(!curr) return 0;
		if((a >> d) & 1){
            return query(curr->next[1],a,d-1);
		}else{
            return query(curr->next[0],a,d-1);
        }
	}
    int query(const int a){
        return query(root,a,MAX_BIT-1);
    }
};
