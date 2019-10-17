#include "../header.hpp"

class SuffixArray {
public:
    int sz, index1, index2;
    vector<int> rnk, tmp, sa, lcp;
    SuffixArray(const string& arg)
        : sz((int)arg.size()), rnk(sz+1), tmp(sz+1), sa(sz+1), lcp(sz+1){
        make_sa(arg), make_lcp(arg);
    }
    void make_sa(const string& s){
        index1 = sz;
        for(int i = 0; i <= index1; ++i){
            sa[i] = i;
            rnk[i] = (i < index1) ? s[i] : -1;
        }
        auto comp = [&](const int i, const int j){
            if(rnk[i] != rnk[j]){
                return rnk[i] < rnk[j];
            }else{
                const int ri = (i + index2 <= index1) ? rnk[i + index2] : -1;
                const int rj = (j + index2 <= index1) ? rnk[j + index2] : -1;
                return ri < rj;
            }
        };
        for(index2 = 1; index2 <= index1; index2 *= 2){
            sort(sa.begin(), sa.end(), comp);
            tmp[sa[0]] = 0;
            for(int i = 1; i <= index1; ++i){
                tmp[sa[i]] = tmp[sa[i-1]] + (comp(sa[i-1], sa[i]) ? 1 : 0);
            }
            for(int i = 0; i <= index1; ++i){
                rnk[i] = tmp[i];
            }
        }
    }
    void make_lcp(const string& s){
        for(int i = 0; i <= sz; ++i){
            rnk[sa[i]] = i;
        }
        int h = 0;
        lcp[0] = 0;
        for(int i = 0; i < sz; ++i){
            int j = sa[rnk[i]-1];
            if(h > 0){
                --h;
            }
            for(; j + h < sz && i + h < sz; ++h){
                if(s[j+h] != s[i+h]){
                    break;
                }
            }
            lcp[rnk[i]-1] = h;
        }
    }
};

long long solve(const string& s)
{
    int n = (int)s.size();
    long long ans = 1;
    SuffixArray SA(s);
    for(int i = 0; i < n; ++i){
        ans += n - SA.sa[i+1] - SA.lcp[i];
    }
    return ans;
}
