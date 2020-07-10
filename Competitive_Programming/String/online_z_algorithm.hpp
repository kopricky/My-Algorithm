#include "../header.hpp"

class OnlineZalgo {
private:
    string s;
    vector<int> arr;
    queue<int> unconfirm;
    vector<vector<int> > info;
public:
    OnlineZalgo() : arr(1), info(1){}
    void add(char c){
        s.push_back(c);
        if((int)s.size() == 1) return;
        info.push_back(vector<int>());
        int nx = -1;
        while(!unconfirm.empty()){
            const int id = unconfirm.front();
            if(arr[id] >= 0){
                unconfirm.pop();
            }else if(s[(int)s.size() - 1 - id] != c){
                unconfirm.pop();
                arr[id] += (int)s.size() - 1;
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
                info.back().push_back(id);
            }
        }
        arr.push_back(0);
        if(s[0] == c){
            unconfirm.push((int)s.size() - 1);
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
