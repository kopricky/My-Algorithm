#include "header.hpp"

class Irving
{
private:
    int n;
    vector<list<int> > pref;
    vector<vector<list<int>::iterator> > pref_iter;
    vector<vector<int> > order;
    list<int> pref_index;
    vector<list<int>::iterator> pref_index_iter;
    bool _erase(int num1, int num2)
    {
        pref[num1].erase(pref_iter[num1][order[num1][num2]]);
        pref[num2].erase(pref_iter[num2][order[num2][num1]]);
        return pref[num1].empty() || pref[num2].empty();
    }
    bool __erase(int num1, int num2)
    {
        pref[num1].erase(pref_iter[num1][order[num1][num2]]);
        pref[num2].erase(pref_iter[num2][order[num2][num1]]);
        if(pref[num1].size() == 1) pref_index.erase(pref_index_iter[num1]);
        if(pref[num2].size() == 1) pref_index.erase(pref_index_iter[num2]);
        return pref[num1].empty() || pref[num2].empty();
    }
    bool reduction(const vector<int>& cand)
    {
        pref_index_iter.resize(n);
        for(int i = 0; i < n; ++i){
            if(cand[i] < 0) continue;
            for(auto it = --pref[i].end(); *it != cand[i]; it = --pref[i].end()){
                if(_erase(i, *it)) return true;
            }
            if(pref[i].size() > 1){
                pref_index.push_back(i);
                pref_index_iter[i] = --pref_index.end();
            }
        }
        return false;
    }
    bool phase1()
    {
        stack<int> single;
        for(int i = 0; i < n; ++i){
            single.push(i);
        }
        vector<int> cand(n, -1);
        while(!single.empty()){
            int u = single.top();
            single.pop();
            while(true){
                int v = *pref[u].begin();
                if(cand[v] < 0){
                    cand[v] = u;
                    break;
                }else if(order[v][cand[v]] > order[v][u]){
                    single.push(cand[v]);
                    if(_erase(v, cand[v])) return true;
                    cand[v] = u;
                    break;
                }else{
                    if(_erase(v, u)) return true;
                }
            }
        }
        return reduction(cand);
    }
    int eliminate(unique_ptr<bool[]>&& visit, int cur)
    {
        list<int> pseq;
        while(!visit[cur]){
            pseq.push_back(cur);
            visit[cur] = true;
            cur = *(--pref[*(++pref[cur].begin())].end());
        }
        list<int>::iterator it = pseq.begin();
        for(;;++it){
            visit[*it] = false;
            if(*it == cur) break;
        }
        list<pair<int, int> > elim;
        for(;it != pseq.end(); ++it){
            visit[*it] = false;
            elim.emplace_back(*it, *(++pref[*it].begin()));
        }
        for(pair<int, int>& value : elim){
            int p = value.first, q = value.second;
            if(*pref[p].begin() != q){
                if(__erase(p, *pref[p].begin())) return -1;
            }
            for(auto it = --pref[q].end(); *it != p; it = --pref[q].end()){
                if(__erase(q, *it)) return -1;
            }
        }
        return cur;
    }
    bool phase2()
    {
        if(pref_index.empty()) return false;
        unique_ptr<bool[]> visit(new bool[n]());
        int start = *pref_index.begin();
        do {
            if(pref[start].size() == 1) start = *pref_index.begin();
            if((start = eliminate(move(visit), start)) < 0) return true;
        }while(!pref_index.empty());
        return false;
    }
public:
    Irving(){}
    vector<int> solve(const vector<vector<int> >& _pref)
    {
        n = (int)_pref.size();
        assert(n % 2 == 0);
        pref.resize(n);
        pref_iter.resize(n, vector<list<int>::iterator>(n-1));
        order.resize(n, vector<int>(n));
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < n-1; ++j){
                pref[i].push_back(_pref[i][j]);
                pref_iter[i][j] = --pref[i].end();
                order[i][_pref[i][j]] = j;
            }
        }
        if(phase1()) return vector<int>();
        if(phase2()) return vector<int>();
        vector<int> ans(n);
        for(int i = 0; i < n; ++i){
            ans[i] = *pref[i].begin();
        }
        return ans;
    }
};
