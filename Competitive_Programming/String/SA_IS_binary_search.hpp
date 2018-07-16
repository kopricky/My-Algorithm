#include "SA_IS.hpp"

//criが連続部分文字列として含まれるかをチェック
bool contain(SA_IS& ag,string& cri){
    int a = 0,b = ag.sz;
    while(b-a>1){
        int c = (a+b)/2;
        if(ag.CS.compare(ag.sa[c],cri.length(),cri)<0){
            a = c;
        }else{
            b = c;
        }
    }
    return ag.CS.compare(ag.sa[b],cri.length(),cri) == 0;
}

//cri以上の値のスタートインデックス
int stsearch(SA_IS& ag, string& cri)
{
    int n = ag.sz;
    int l=0,r=n+1;
    int* sa = ag.sa;
    while(r-l>1){
        int mid = (l+r)/2;
        if(n-sa[mid] < (int)cri.size()){
            string t = ag.CS.substr(sa[mid],n-sa[mid]);
            if(t <= cri){
                l = mid;
            }else{
                r = mid;
            }
        }else{
            string t = ag.CS.substr(sa[mid],(int)cri.size());
            if(t < cri){
                l = mid;
            }else{
                r = mid;
            }
        }
    }
    return r;
}

//cri以下の値のエンドインデックス+1(開区間)
int edsearch(SA_IS& ag, string& cri)
{
    int n = ag.sz;
    int l=0,r=n+1;
    int* sa = ag.sa;
    while(r-l>1){
        int mid = (l+r)/2;
        if(n-sa[mid] < (int)cri.size()){
            string t = ag.CS.substr(sa[mid],n-sa[mid]);
            if(t < cri){
                l = mid;
            }else{
                r = mid;
            }
        }else{
            string t = ag.CS.substr(sa[mid],(int)cri.size());
            if(t <= cri){
                l = mid;
            }else{
                r = mid;
            }
        }
    }
    return r;
}
