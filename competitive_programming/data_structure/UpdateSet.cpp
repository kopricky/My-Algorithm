//セグ木で持てないくらい大きい範囲の(0,1)の区間更新のクエリに答える
//この実装では最終的に和を答える
template<typename T> class UpdateSet
{
private:
    using ptt = pair<T,T>;
    set<ptt> st;
    T inf,sm;
public:
    UpdateSet() : inf(numeric_limits<T>::max()),sm(0){};
    //[l,r)をk(0,1)に更新
    void update(T l,T r,int k){
        while(1){
            auto it = st.lower_bound(ptt(l,-inf));
            if(it == st.end()){
                break;
            }
            ptt p = *it;
            if(p.fi >= r){
                break;
            }
            st.erase(it);
            sm -= p.se - p.fi;
            if(p.se > r){
                st.insert(ptt(r,p.se));
                sm += p.se - r;
            }
        }
        auto it = st.lower_bound(ptt(l,-inf));
        if(it != st.begin()){
            --it;
            P p = *it;
            if(p.se > l){
                st.erase(it);
                sm -= p.se - p.fi;
                st.insert(ptt(p.fi,l));
                sm += l - p.fi;
            }
            if(p.se > r){
                st.insert(ptt(r,p.se));
                sm += p.se-r;
            }
        }
        if(k == 1){
            st.insert(ptt(l,r));
            sm += r-l;
        }
    }
    T query(){
        return sm;
    }
};
