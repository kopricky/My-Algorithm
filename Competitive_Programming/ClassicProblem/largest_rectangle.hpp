#include "../header.hpp"

// 0, 1 の field(0 の場所に長方形作れる)
int calc(vector<vector<int> >& field)
{
	int h = (int)field.size(), w = (int)field[0].size();
	vector<vector<int> > t(h+1, vector<int>(w+1));
	int ans = 0;
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
			if(field[i][j]) t[i+1][j] = 0;
			else t[i+1][j] = t[i][j] + 1;
		}
		t[i+1][w] = 0;
		stack<pair<int, int> > st;
		for(int j = 0; j <= w; j++){
			if(st.empty()){
				if(t[i+1][j]) st.push({j, t[i+1][j]});
				continue;
			}
			pair<int, int> p = st.top();
			if(p.second < t[i+1][j]){
				st.push({j, t[i+1][j]});
			}else if(p.second > t[i+1][j]){
				int pos;
				while(p.second >= t[i+1][j]){
					ans = max(ans, p.second*(j-(pos=p.first)));
					st.pop();
					if(!st.empty()) p = st.top();
					else break;
				}
				if(t[i+1][j]) st.push({pos, t[i+1][j]});
			}
		}
	}
	return ans;
}
