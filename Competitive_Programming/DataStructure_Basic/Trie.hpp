#include "../header.hpp"

//トライ木

class Trie {
public:
	struct Node {
		Node *next[26];
		int sub;
		Node() : sub(0){
			for(int i = 0; i < 26; i++){
				next[i] = nullptr;
			}
		}
	};
	Node* root;
	Trie(){
		root = new Node();
	}
	//Trie木にxを加える
	void add(string& s) {
		Node *curr = root;
		for(int i = 0; i < (int)s.size(); i++){
			int y = s[i] - 'a';
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
		int res = 0;
		for(int i = 0; i < 26; i++){
			if(curr->next[i]){
				res += query(curr->next[i],d+1);
			}
		}
		return res;
	}
	int query() {
		return query(root, 0);
	}
};
