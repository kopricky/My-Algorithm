#include "../header.hpp"

//区間(pair<int,int>)の配列が与えられたときに最内の区間および最外の区間を取り除く(前処理で用いる)
//外側の区間を削除
template<typename T>
vector<pair<T, T> > EraseOuterInterval(vector<pair<T, T> > vec)
{
    using ptt = pair<T, T>;
    vector<ptt> res;
    sort(all(vec),[](const ptt& a,const ptt& b){
        return (a.second == b.second)?(a.first > b.first):(a.second < b.second);
    });
    priority_queue<ptt> que;
    for(int i = (int)vec.size()-1; i >= 0; i--){
        while(!que.empty()){
            ptt p = que.top();
            if(-p.first > vec[i].first){
                break;
            }
            que.pop();
        }
        que.push(ptt(-vec[i].first,vec[i].second));
    }
    while(!que.empty()){
        int x,y;
        tie(x,y) = que.top();
        que.pop();
        res.push_back(ptt(-x,y));
    }
    return res;
}
//内側の区間を削除
template<typename T>
vector<pair<T, T> > EraseInnerInterval(vector<pair<T, T> > vec)
{
    using ptt = pair<T, T>;
    vector<ptt> res;
    sort(all(vec),[](const ptt& a,const ptt& b){
        return (a.second == b.second)?(a.first < b.first):(a.second > b.second);
    });
    priority_queue<ptt> que;
    for(int i = (int)vec.size()-1; i >= 0; i--){
        while(!que.empty()){
            ptt p = que.top();
            if(p.first < vec[i].first){
                break;
            }
            que.pop();
        }
        que.push(ptt(vec[i].first,vec[i].second));
    }
    vec.clear();
    while(!que.empty()){
        int x,y;
        tie(x,y) = que.top();
        que.pop();
        res.push_back(ptt(x,y));
    }
    return res;
}
