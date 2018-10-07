#include "../header.hpp"

// 無向グラフ G に長さ4の単純閉路が含まれるかどうかを返す(O(n^2))
bool C4_Find(vector<vector<int> >& G) {
    int n = (int)G.size();
    vector<vector<bool> > check(n, vector<bool>(n, false));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < (int)G[i].size()-1; j++){
            for(int k = j+1; k < (int)G[i].size(); k++){
                if(check[G[i][j]][G[i][k]]) return true;
                check[G[i][j]][G[i][k]] = check[G[i][k]][G[i][j]] = true;
            }
        }
    }
    return false;
}
