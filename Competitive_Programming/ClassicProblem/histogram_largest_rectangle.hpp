#include "../header.hpp"

long long calc(vector<int>& t)
{
    int w = (int)t.size();
    long long ans = 0;
    t.push_back(0);
    stack<pair<int, int> > st;
    for(int j = 0; j <= w; j++){
        int pos = j;
        while(!st.empty()){
            pair<int, int> p = st.top();
            if(p.second < t[j]) break;
            st.pop();
            ans = max(ans, (long long)p.second * (j - (pos = p.first)));
        }
        st.push({pos, t[j]});
    }
	return ans;
}