#include "../header.hpp"

template<typename T, int NUM> class SyntaxRangeParser {
private:
    class segtree {
    private:
        int opr, size;
        const SyntaxRangeParser* obj;
        vector<T> val;
    public:
        void init(const int opr_, const SyntaxRangeParser* obj_){
            obj = obj_, opr = opr_;
            size = 1;
            while(size < obj->n){
                size *= 2;
            }
            val.resize(2 * size, obj->ie[opr]);
        }
        void update(int k, const T a) {
        	val[k+=obj->n] = a;
        	while(k >>= 1){
        		val[k] = obj->func[opr](val[2 * k], val[2 * k + 1]);
        	}
        }
        T query(int a, int b) {
            T res1 = obj->ie[opr], res2 = obj->ie[opr];
            a += obj->n, b += obj->n;
            while(a != b){
                if(a % 2) res1 = obj->func[opr](res1, val[a++]);
                if(b % 2) res2 = obj->func[opr](res2, val[--b]);
                a >>= 1, b >>= 1;
            }
            return obj->func[opr](res1, res2);
        }
    };
    using pti = pair<T, int>;
    string s;
    int n;
    T ie[NUM];
    map<char, int> optoi;
    function<T(T, T)> func[NUM];
    vector<int> opr_id[NUM], st[NUM - 1], ed[NUM - 1], num_id;
    int opr_cnt[NUM], num_cnt;
    segtree sg[NUM];
    pti factor(const int l, const int r) {
        num_cnt++;
        T res = 0;
        for(int i = l; i < r; i++){
            res = res * 10 + s[i] - '0';
            num_id[i] = num_cnt;
        }
        return make_pair(res, num_cnt);
    }
    pti term(const int opr, const int l, const int r) {
        if(opr < 0) return factor(l, r);
        opr_cnt[opr]++;
        int pos = l;
        bool flag = true;
        T res = ie[opr];
        for(int i = l; i < r; i++){
            if(optoi[s[i]] == opr + 1){
                pti p = term(opr - 1, pos, i);
                pos = i + 1;
                sg[opr].update(p.second, p.first);
                res = func[opr](res, p.first);
                opr_id[opr][p.second] = opr_cnt[opr];
                if(flag) flag = false, st[opr][opr_cnt[opr]] = p.second;
            }
        }
        pti p = term(opr - 1, pos, r);
        sg[opr].update(p.second, p.first);
        res = func[opr](res, p.first);
        opr_id[opr][p.second] = opr_cnt[opr];
        if(flag) st[opr][opr_cnt[opr]] = p.second;
        ed[opr][opr_cnt[opr]] = p.second + 1;
        return make_pair(res, opr_cnt[opr]);
    }
    pti expr(const int l, const int r) {
        opr_cnt[NUM - 1]++;
        int pos = l;
        T res = ie[NUM - 1];
        for(int i = l; i < r; i++){
            if(optoi[s[i]] == NUM){
                pti p = term(NUM - 2, pos, i);
                pos = i + 1;
                sg[NUM - 1].update(p.second, p.first);
                res = func[NUM - 1](res, p.first);
                opr_id[NUM - 1][p.second] = opr_cnt[NUM - 1];
            }
        }
        pti p = term(NUM - 2, pos, r);
        sg[NUM - 1].update(p.second, p.first);
        res = func[NUM - 1](res, p.first);
        opr_id[NUM - 1][p.second] = opr_cnt[NUM - 1];
        return make_pair(res, opr_cnt[NUM - 1]);
    }
    T number(const int l, const int r){
        T res = 0;
        int pos = l;
        while(pos < r && isdigit(s[pos])){
            res = res * 10 + s[pos++] - '0';
        }
        return res;
    }
    T number_rev(const int l, const int r){
        T res = 0, dig = 1;
        int pos = r-1;
        while(l <= pos && isdigit(s[pos])){
            res += (s[pos--] - '0') * dig, dig *= 10;
        }
        return res;
    }
    void init(){
        func[0] = [&](T x, T y){ return x ^ y; }, optoi['^'] = 1, ie[0] = 0;
        func[1] = [&](T x, T y){ return x & y; }, optoi['&'] = 2, ie[1] = numeric_limits<T>::max();
        func[2] = [&](T x, T y){ return x | y; }, optoi['|'] = 3, ie[2] = 0;
    }
public:
    SyntaxRangeParser(string& s_) : s(s_), n((int)(s_.size())), num_id(n), opr_cnt{}, num_cnt(0){
        for(int i = 0; i < NUM; i++) sg[i].init(i, this), opr_id[i].resize(n);
        for(int i = 0; i < NUM - 1; i++) st[i].resize(n), ed[i].resize(n);
        init();
        expr(0, n);
    }
    T query(const int l, const int r) {
        T lnum = number(l, n), rnum = number_rev(0, r);
        int lpre = num_id[l], rpre = num_id[r - 1];
        if(lpre == rpre) return number(l, r);
        for(int i = 0; i < NUM; i++){
            int lnow = opr_id[i][lpre], rnow = opr_id[i][rpre];
            if(lnow == rnow) return func[i](func[i](lnum, sg[i].query(lpre + 1, rpre)), rnum);
            if(i == NUM - 1) break;
            lnum = func[i](lnum, sg[i].query(lpre + 1, ed[i][lnow]));
            rnum = func[i](sg[i].query(st[i][rnow], rpre), rnum);
            lpre = lnow, rpre = rnow;
        }
        return -1;
    }
};
