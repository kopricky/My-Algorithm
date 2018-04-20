#include "../header.hpp"

class Trie {
public:
	struct Node {
		Node *next[26];
		int sub;
		Node() : sub(0){
            rep(i,26){
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
        rep(i,(int)s.size()){
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
		if (!curr) return 0;
		return query(curr->next[0],d-1) + query(curr->next[1],d-1);
	}
};
