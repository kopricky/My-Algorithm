#include "header.hpp"

class GaleSharpley
{
public:
    GaleSharpley(){}
    vector<int> solve(const vector<vector<int> >& mpref, const vector<vector<int> >& wpref)
    {
        int n = mpref.size();
        vector<vector<int> > worder(n, vector<int>(n));
        stack<int> single;
        for(int i = 0; i < n; ++i){
            single.push(i);
            for(int j = 0; j < n; ++j){
                worder[i][wpref[i][j]] = j;
            }
        }
        vector<int> id_pos(n, 0);
        vector<int> wtom(n, -1);
        while(!single.empty()){
            int man = single.top();
            single.pop();
            while(true){
                int woman = mpref[man][id_pos[man]++];
                if(wtom[woman] < 0){
                    wtom[woman] = man;
                    break;
                }else if(worder[woman][wtom[woman]] > worder[woman][man]){
                    single.push(wtom[woman]);
                    wtom[woman] = man;
                    break;
                }
            }
        }
        vector<int> mtow(n);
        for(int i = 0; i < n; ++i)
            mtow[wtom[i]] = i;
        return mtow;
    }
};
