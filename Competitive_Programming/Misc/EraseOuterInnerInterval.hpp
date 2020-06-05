#include "../header.hpp"

//区間(pair<int,int>)の配列が与えられたときに最内の区間および最外の区間を取り除く(前処理で用いる)
//外側の区間を削除
template<typename T>
vector<pair<T, T> > EraseOuterInterval(vector<pair<T, T> > vec)
{
    sort(vec.begin(), vec.end(), [](const pair<T, T>& a, const pair<T, T>& b){
        return (a.second == b.second) ? (a.first < b.first) : (a.second > b.second);
    });
    vector<pair<T, T> > res;
    for(const pair<T, T>& p : vec){
        while(!res.empty()){
            const pair<T, T>& q = res.back();
            if(p.first < q.first) break;
            res.pop_back();
        }
        res.push_back(p);
    }
    reverse(res.begin(), res.end());
    return res;
}

//内側の区間を削除
template<typename T>
vector<pair<T, T> > EraseInnerInterval(vector<pair<T, T> > vec)
{
    sort(vec.begin(), vec.end(), [](const pair<T, T>& a, const pair<T, T>& b){
        return (a.second == b.second) ? (a.first > b.first) : (a.second < b.second);
    });
    vector<pair<T, T> > res;
    for(const pair<T, T>& p : vec){
        while(!res.empty()){
            const pair<T, T>& q = res.back();
            if(q.first < p.first) break;
            res.pop_back();
        }
        res.push_back(p);
    }
    return res;
}
