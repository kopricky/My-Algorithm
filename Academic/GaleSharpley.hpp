#include "header.hpp"

class GaleSharpley
{
public:
    GaleSharpley(){}
    vector<int> solve(const vector<vector<int> >& mpre, const vector<vector<int> >& wpre)
    {
        int n = mpre.size();
        vector<vector<int> > worder(n, vector<int>(n));
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
                worder[i][wpre[i][j]] = j;
        vector<int> id_pos(n, 0);
        queue<int> single;
        for(int i = 0; i < n; ++i)
            single.push(i);
        vector<int> wtom(n);
        while(!single.empty()){
            int man = single.front();
            single.pop();
            for(;;++id_pos[man]){
                int woman = id_pos[man];
                if(wpre[woman][wtom[woman]] > wpre[woman][man]){
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
