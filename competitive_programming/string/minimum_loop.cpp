//各iについてS[0:i]の最小周期長(っぽいもの)を求める
//例えばababaの最小周期長は5ではなく2となるので注意
class minimum_loop
{
public:
    vector<int> table,res;
    minimum_loop(const string s){
        int slen = (int)s.size();
        table.resize(slen+1);
        table[0] = -1;
        int j = -1;
        rep(i,slen){
            while(j >= 0 && s[i] != s[j]){
                j = table[j];
            }
            table[i+1] = ++j;
        }
        res.resize(slen);
        rep(i,slen){
            res[i] = i+1-table[i+1];
        }
    }
};
