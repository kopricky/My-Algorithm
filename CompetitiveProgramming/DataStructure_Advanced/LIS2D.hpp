#include "../header.hpp"
//2次元LISを保持する
template<typename T> class LIS2D
{
private:
    using ptt = pair<T,T>;
    set<ptt> st;
public:
    //実際に追加されたかどうかを返す
    bool insert(pair<T,T> val){
        bool in = false;
        auto it = st.lower_bound(val);
        if(it != st.end()){
            if(*it == val) return in;
            in = (it->second >= val.second);
        }
        if(in){
            while(it != st.end() && it->second >= val.second){
                it = st.erase(it);
            }
        }else{
            if(it != st.begin()){
                it--;
                in = (it->second > val.second);
            }else{
                in = true;
            }
        }
        if(in) st.insert(val);
        return in;
    }
    void print(){
        for(auto& val : st){
            cout << "{" << val.first << "," << val.second << "}" << " ";
        }
        cout << "\n";
    }
};
