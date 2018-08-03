#include "../header.hpp"

class AhoCorasick
{
private:
    using AC = AhoCorasick*;
    vector<pair<int, int> > ac;
    map<int, AC> next;
    AC t, fail, dnx;
    void add(const string& s, int pos, int id){
        if(pos == (int)s.size()){
            ac.emplace_back(id, pos);
            return;
        }
        if(!next[s[pos]]) next[s[pos]] = new AhoCorasick();
        next[s[pos]]->add(s,pos+1,id);
    }
    AC count(vector<vector<int> >& res, int pos){
        if(!fail) return this;
        for(auto val : ac) res[val.first].push_back(pos-val.second+1);
        if(dnx) dnx->count(res,pos);
        else dnx = fail->count(res,pos);
        return ac.size() ? this : dnx;
    }
    void match_(vector<vector<int> >& res, const string& s, int pos=0){
        if(pos == (int)s.size()) return;
        if(next[s[pos]]){
            next[s[pos]]->count(res, pos);
            next[s[pos]]->match_(res,s,pos+1);
        }else{
            if(!fail) match_(res,s,pos+1);
            else fail->match_(res,s,pos);
        }
    }
public:
    AhoCorasick() : fail(nullptr), dnx(nullptr){}
    //パターンを入力として木を構築
    AhoCorasick(vector<string>& pattern): AhoCorasick(){
        int sz = (int)pattern.size();
        t = new AhoCorasick();
        rep(i,sz) t->add(pattern[i],0,i);
        queue<AC> que;
        que.push(t);
        t->fail = nullptr;
        while(!que.empty()){
            AC nw = que.front();
            que.pop();
            for(auto p : nw->next){
                int i = p.first;
                AC fail = nw->fail;
                while(fail && !fail->next.count(i)){
                    fail = fail->fail;
                }
                nw->next[i]->fail = (!fail ? t : fail->next[i]);
                que.push(nw->next[i]);
            }
        }
    }
    //sのインデックスpos(デフォルト0)以降に含まれる各パターンの開始位置をresで返す
    void match(vector<vector<int> >& res, const string& s, int pos=0){
        t->match_(res,s,pos);
    }
};
