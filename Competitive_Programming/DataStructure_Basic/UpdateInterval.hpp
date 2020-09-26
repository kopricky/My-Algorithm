#include "../header.hpp"

//セグ木で持てないくらい大きい範囲の(0,1)の区間更新のクエリに答える
template<typename T> class UpdateInterval
{
private:
    using ptt = pair<T,T>;
public:
    //1の区間をsetで保持
    set<ptt> st;
    UpdateInterval(){};
    //[l,r)をk(0,1)に更新
    void update(T l, T r, int k){
        T lb = l, rb = r;
        while(1){
            auto it = st.lower_bound(ptt(l, numeric_limits<T>::min()));
            if(it == st.end()){
                break;
            }
            ptt p = *it;
            if(p.first > r){
                break;
            }
            st.erase(it);
            if(p.second > r){
                rb = max(rb, p.second);
                if(!k) st.insert(ptt(r, p.second));
                break;
            }
        }
        auto it = st.lower_bound(ptt(l, numeric_limits<T>::min()));
        if(it != st.begin()){
            --it;
            ptt p = *it;
            if(p.second >= l){
                st.erase(it);
                if(!k) st.insert(ptt(p.first, l));
                lb = min(lb, p.first);
            }
            if(p.second > r){
                if(!k) st.insert(ptt(r, p.second));
                rb = max(rb, p.second);
            }
        }
        if(k) st.insert(ptt(lb, rb));
    }
};
