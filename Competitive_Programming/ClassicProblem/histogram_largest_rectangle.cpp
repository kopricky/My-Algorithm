#include "../header.hpp"

int calc(vector<int>& t)
{
    int w = (int)t.size(), ans = 0;
    t.push_back(0);
    stack<pair<int, int> > st;
    for(int j = 0; j <= w; j++){
        if(st.empty()){
            st.push({j, t[j]});
            continue;
        }
        pair<int, int> p = st.top();
        if(p.second < t[j]){
            st.push({j, t[j]});
        }else if(p.second > t[j]){
            int pos;
            while(p.second >= t[j]){
                ans = max(ans, p.second*(j-(pos=p.first)));
                st.pop();
                if(!st.empty()) p = st.top();
                else break;
            }
            st.push({pos, t[j]});
        }
    }
	return ans;
}
