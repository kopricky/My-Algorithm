#include "./header.hpp"

//lcp(高さ配列)も計算するときはコメントアウトをはずす
class SA_IS
{
private:
    using byte = unsigned char;
    byte mask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
    #define tget(i) !!(t[(i)>>3]&mask[(i)&7])
    #define tset(i, b) t[(i)>>3]=(b) ? (mask[(i)&7]|t[(i)>>3]) : ((~mask[(i)&7])&t[(i)>>3])
    #define chr(i) (cs==sizeof(int)?((int*)s)[i]:((byte*)s)[i])
    #define isLMS(i) (i>0 && tget(i) && !tget(i-1))
    void getBuckets(byte *s, int *bkt, int n, int K, int cs, bool end=true){
        fill(bkt, bkt + K + 1, 0);
        for(int i = 0; i < n; i++){
            bkt[chr(i)]++;
        }
        for(int i = 0,tmp = 0; i < K+1; i++){
            tmp += bkt[i];
            bkt[i] = end ? tmp : tmp - bkt[i];
        }
    }
    void induceSAl(byte *t, byte *s, int *bkt, int n, int K, int cs){
        getBuckets(s, bkt, n, K, cs, false);
        for(int i = 0; i < n; i++){
            if(sa[i]>0 && !tget(sa[i]-1)){
                sa[bkt[chr(sa[i]-1)]++] = sa[i]-1;
            }
        }
    }
    void induceSAs(byte *t, byte *s, int *bkt, int n, int K, int cs){
        getBuckets(s, bkt, n, K, cs, true);
        for(int i = n-1; i >= 0; i--){
            if(sa[i] > 0 && tget(sa[i]-1)){
                sa[--bkt[chr(sa[i]-1)]] = sa[i]-1;
            }
        }
    }
    void make_sa(byte *s, int n, int K=128, int cs=1){
        byte t[(n >> 3)+1];
        int bkt[K+1], n1 = 0, name = 0;
        tset(n-2, 0), tset(n-1, 1);
        for(int i = n - 3; i >=0; i--){
            tset(i, (chr(i)<chr(i+1) || (chr(i)==chr(i+1) && tget(i+1))));
        }
        getBuckets(s, bkt, n, K, cs);
        fill(sa, sa+n, -1);
        for(int i = 1; i < n; i++){
            if(isLMS(i)){
                sa[--bkt[chr(i)]] = i;
            }
        }
        induceSAl(t, s, bkt, n, K, cs);
        induceSAs(t, s, bkt, n, K, cs);
        for(int i = 0; i < n; i++){
            if(isLMS(sa[i])){
                sa[n1++] = sa[i];
            }
        }
        fill(sa + n1, sa + n, -1);
        for(int i = 0, tmp = -1; i < n1; i++){
            int pos = sa[i], diff = false;
            for(int d = 0; d < n && !diff; d++){
                diff = chr(pos+d) != chr(tmp+d) || tget(pos+d) != tget(tmp+d);
                if(!diff && d && (isLMS(pos+d) || isLMS(tmp+d))) break;
            }
            if(diff){
                name++, tmp = pos;
            }
            sa[n1+((pos - (pos & 1)) >> 1)] = name - 1;
        }
        int* s1 = sa + n - n1;
        for(int i = n - 1,j = n - 1; i >= n1; i--){
            if(sa[i] >= 0){
                sa[j--] = sa[i];
            }
        }
        if(name < n1){
            make_sa((byte*)s1, n1, name - 1, sizeof(int));
        }else{
            for(int i = 0; i < n1; i++){
                sa[s1[i]] = i;
            }
        }
        getBuckets(s, bkt, n, K, cs);
        for(int i = 1, j = 0; i < n; i++){
            if(isLMS(i)){
                s1[j++] = i;
            }
        }
        for(int i = 0; i < n1; i++){
            sa[i] = s1[sa[i]];
        }
        fill(sa + n1, sa + n, -1);
        for(int i = n1 - 1; i >= 0; i--){
          int tmp = sa[i];
          sa[i] = -1, sa[--bkt[chr(tmp)]] = tmp;
        }
        induceSAl(t, s, bkt, n, K, cs);
        induceSAs(t, s, bkt, n, K, cs);
    }
    void make_lcp(){
        lcp = new int[sz+1]();
        rnk = new int[sz+1]();
        for(int i = 0; i <= sz; i++) rnk[sa[i]] = i;
        for(int i = 0, h = 0; i < sz; i++){
            int j = sa[rnk[i]-1];
            if(h > 0) h--;
            for(;j+h<sz&&i+h<sz;h++){
                if(CS[j+h] != CS[i+h]) break;
            }
            lcp[rnk[i]-1] = h;
        }
    }
public:
    bool contain(const string& T){
        int a = 0, b = sz;
        while(b - a > 1){
            int c = (a + b) / 2;
            if(CS.compare(sa[c], T.length(), T) < 0){
                a = c;
            }else{
                b = c;
            }
        }
        return CS.compare(sa[b], T.length(), T) == 0;
    }
    string CS;
    byte* S;
    int sz;
    int *sa, *lcp, *rnk;
    SA_IS(string& arg){
        CS = arg;
        sz = (int)arg.size();
        sa = new int[sz+1]();
        S = (byte*)arg.c_str();
        make_sa(S, sz+1);
        // make_lcp();
    }
    // ~SA_IS(){
    //     delete[] sa;
    //     delete[] lcp; delete[] rnk;
    // }
};
