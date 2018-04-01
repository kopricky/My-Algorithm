//区間(pair<int,int>)の配列が与えられたときに最内の区間および最外の区間を取り除く(前処理)
//外側の区間を削除
typedef pair<int,int> P;
vector<P> EraseOuterInterval(vector<P> vec)
{
    vector<P> res;
    sort(all(vec),[](const P& a,const P& b){
        return (a.second == b.second)?(a.first > b.first):(a.second < b.second);
    });
    priority_queue<P> que;
    for(int i = (int)vec.size()-1; i >= 0; i--){
        while(!que.empty()){
            P p = que.top();
            if(-p.first > vec[i].first){
                break;
            }
            que.pop();
        }
        que.push(P(-vec[i].first,vec[i].second));
    }
    while(!que.empty()){
        int x,y;
        tie(x,y) = que.top();
        que.pop();
        res.push_back(P(-x,y));
    }
    return res;
}
//内側の区間を削除
vector<P> EraseInnerInterval(vector<P> vec)
{
    vector<P> res;
    sort(all(vec),[](const P& a,const P& b){
        return (a.second == b.second)?(a.first < b.first):(a.second > b.second);
    });
    priority_queue<P> que;
    for(int i = (int)vec.size()-1; i >= 0; i--){
        while(!que.empty()){
            P p = que.top();
            if(p.first < vec[i].first){
                break;
            }
            que.pop();
        }
        que.push(P(vec[i].first,vec[i].second));
    }
    vec.clear();
    while(!que.empty()){
        int x,y;
        tie(x,y) = que.top();
        que.pop();
        res.push_back(P(x,y));
    }
    return res;
}
