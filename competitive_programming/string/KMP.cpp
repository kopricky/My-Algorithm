//resに文字列Sの中にある検索文字列sのすべての一致箇所の開始位置が入る(0_indexed)
//table は次にどこから見るか(1_indexed)
class KMP
{
public:
    vector<int> table, res;
    string pattern;
    int plen;
    KMP(const string s){
        pattern = s;
        plen = (int)pattern.size();
        table.resize(plen+1);
        table[0] = -1;
        int j = -1;
        rep(i,plen){
            while(j >= 0 && pattern[i] != pattern[j]){
                j = table[j];
            }
            table[i+1] = ++j;
        }
    }
    void search(const string& text){
        int head = 0, j = 0, tlen = (int)text.size();
        while(head + j < tlen){
            if(pattern[j] == text[head + j]) {
                if(++j == plen){
                    res.push_back(head);
                    head = head + j - table[j];
                    j = table[j];
                }
            }else{
                head = head + j - table[j];
                if(j){
                    j = table[j];
                }
            }
        }
    }
};
