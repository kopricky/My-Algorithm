#include "../header.hpp"

class AhoCorasick
{
private:
    using AC = AhoCorasick*;
    vector<int> ac;
    map<int, AC> next;
    AC fail, dnx;
    void add(const string& s, int pos, int id){
        if(pos == (int)s.size()){
            ac.push_back(id);
            return;
        }
        if(!next[s[pos]]) next[s[pos]] = new AhoCorasick();
        next[s[pos]]->add(s,pos+1,id);
    }
    AC count(vector<int>& res){
        if(!fail) return this;
        for(int id : ac) res[id]++;
        if(dnx) dnx->count(res);
        else dnx = fail->count(res);
        return ac.size() ? this : dnx;
    }
public:
    AhoCorasick() : fail(nullptr), dnx(nullptr){}
    static AC build(vector<string>& pattern){
        int sz = (int)pattern.size();
        AC t = new AhoCorasick();
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
        return t;
    }
    void match(vector<int>& res, const string& s, int pos){
        if(pos == (int)s.size()) return;
        if(next[s[pos]]){
            next[s[pos]]->count(res);
            next[s[pos]]->match(res,s,pos+1);
        }else{
            if(!fail) match(res,s,pos+1);
            else fail->match(res,s,pos);
        }
    }
};
