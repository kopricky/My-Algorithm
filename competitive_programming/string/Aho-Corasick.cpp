//kmjpさんの実装を参照
//文字列がすべて小文字の場合に対応
const int MAX_N = 100005;
const int NUMC = 26;

class Trie {
public:
	vector<vector<int> > V;
	int find(string s){
		int cur=0;
		rep(i,s.size()){
            if((cur=V[cur][s[i]+1])==0) return -1;
        }
		return cur;
	}
	void create(vector<string> S){ // 0 is rep backtrack
		V.clear();
		V.push_back(vector<int>(NUMC+1));
		sort(S.begin(),S.end());
		rep(i,S.size()){
			int cur=0;
			rep(j,S[i].size()){
				if(V[cur][S[i][j]+1]==0) V.push_back(vector<int>(NUMC+1)),V[cur][S[i][j]+1]=V.size()-1;
				cur=V[cur][S[i][j]+1];
			}
		}
	}
};

class ACmatch_num {
public:
	Trie t;
	vector<int> acc;
	int ma;
	void create(vector<string> S){
        rep(i,len(S)){
            rep(j,len(S[i])){
                S[i][j] -= 'a';
            }
        }
		ma=S.size();
		t.create(S);
		acc.clear();
		acc.resize(t.V.size());
		rep(i,S.size()) acc[t.find(S[i])]++;
		queue<int> Q;
		rep(i,NUMC) if(t.V[0][i+1]) t.V[t.V[0][i+1]][0]=0, Q.push(t.V[0][i+1]);
		while(!Q.empty()) {
			int k=Q.front(); Q.pop();
			rep(i,NUMC) if(t.V[k][i+1]) {
				Q.push(t.V[k][i+1]);
				int pre=t.V[k][0];
				while(pre && t.V[pre][i+1]==0) pre=t.V[pre][0];
				t.V[t.V[k][i+1]][0]=t.V[pre][i+1];
				acc[t.V[k][i+1]] += acc[t.V[pre][i+1]];
			}
		}
	}
	int match(string S){
        rep(i,len(S)){
            S[i] -= 'a';
        }
		int R=0;
		int cur=0;
		rep(i,S.size()){
			while(cur && t.V[cur][S[i]+1]==0) cur=t.V[cur][0];
			cur=t.V[cur][S[i]+1];
			R += acc[cur];
		}
		return R;
	}
};
