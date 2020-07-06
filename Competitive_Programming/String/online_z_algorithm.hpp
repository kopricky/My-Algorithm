#include "../header.hpp"

class OnlineZalgo {
private:
    string s;
    vector<int> arr;
    list<int> unconfirm;
    vector<list<int>::iterator> it;
    vector<vector<int> > info;
public:
    OnlineZalgo() : arr(1), it(1), info(1){}
    void add(char c){
        s.push_back(c);
        if((int)s.size() == 1) return;
        it.push_back(list<int>::iterator()), info.push_back(vector<int>());
        int nx = -1;
        while(!unconfirm.empty()){
            const int id = *unconfirm.begin();
            if(s[(int)s.size() - 1 - id] != c){
                arr[id] += (int)s.size() - 1;
                unconfirm.pop_front();
                info.back().push_back(id);
            }else{
                nx = (int)s.size() - 1 - id;
                break;
            }
        }
        if(nx >= 0){
            for(const auto& e : info[nx]){
                const int id = (int)s.size() - 1 - nx + e;
                arr[id] += (int)s.size() - 1;
                unconfirm.erase(it[id]);
                info.back().push_back(id);
            }
        }
        arr.push_back(0);
        if(s[0] == c){
            unconfirm.push_back((int)s.size() - 1);
            it.back() = (--unconfirm.end());
            arr.back() = -(int)s.size() + 1;
        }
    }
    // s と s[index:] の最長共通接頭辞の長さを返す
    int query(int index){
        assert(index < (int)s.size());
        if(index == 0) return (int)s.size();
        return (arr[index] >= 0) ? arr[index] : (arr[index] + (int)s.size());
    }
};
