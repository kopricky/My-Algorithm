#include "../header.hpp"

//セグ木で持てないくらい大きい範囲の(0,1)の区間更新のクエリに答える
template<typename T> class UpdateInterval
{
private:
    using ptt = pair<T,T>;
    T inf;
public:
    //1の区間をsetで保持
    set<ptt> st;
    UpdateInterval() : inf(numeric_limits<T>::max()){};
    //[l,r)をk(0,1)に更新
    void update(T l,T r,int k){
        T lb = l, rb = r;
        while(1){
            auto it = st.lower_bound(ptt(l,-inf));
            if(it == st.end()){
                break;
            }
            ptt p = *it;
            if(p.fi > r){
                break;
            }
            st.erase(it);
            if(p.se > r){
                rb = max(rb, p.se);
                if(!k) st.insert(ptt(r,p.se));
                break;
            }
        }
        auto it = st.lower_bound(ptt(l,-inf));
        if(it != st.begin()){
            --it;
            ptt p = *it;
            if(p.se >= l){
                st.erase(it);
                if(!k) st.insert(ptt(p.fi,l));
                lb = min(lb, p.fi);
            }
            if(p.se > r){
                if(!k) st.insert(ptt(r,p.se));
                rb = max(rb, p.se);
            }
        }
        if(k) st.insert(ptt(lb,rb));
    }
};
