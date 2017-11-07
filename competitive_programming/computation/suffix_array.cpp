int index1,index2;
int rnk[MAX_N+1];
int tmp[MAX_N+1];
vector<int> sa;

bool comp(int i,int j)
{
    if(rnk[i] != rnk[j]){
        return rnk[i] < rnk[j];
    }else{
        int ri = (i+index2<=index1)?rnk[i+index2]:-1;
        int rj = (j+index2<=index1)?rnk[j+index2]:-1;
        return ri < rj;
    }
}

void construct_sa(string S)
{
    index1 = S.length();
    sa.clear();
    sa.resize(index1+1);
    rep(i,index1+1){
        sa[i] = i;
        rnk[i] = i<index1?S[i]:-1;
    }
    for(index2=1;index2<=index1;index2*=2){
        sort(all(sa),comp);
        tmp[sa[0]] = 0;
        for(int i=1;i<=index1;i++){
            tmp[sa[i]] = tmp[sa[i-1]]+(comp(sa[i-1],sa[i])?1:0);
        }
        rep(i,index1+1){
            rnk[i] = tmp[i];
        }
    }
}

bool contain(string S,string T)
{
    int a = 0,b = S.length();
    while(b-a>1){
        int c = (a+b)/2;
        if(S.compare(sa[c],T.length(),T)<0){
            a = c;
        }else{
            b = c;
        }
    }
    return S.compare(sa[b],T.length(),T) == 0;
}

vector<int> lcp;

void construct_lcp(string S)
{
    int sz = S.length();
    lcp.clear();
    lcp.resize(sz+1);
    rep(i,sz+1){
        rnk[sa[i]] = i;
    }
    int h = 0;
    lcp[0] = 0;
    rep(i,sz){
        int j = sa[rnk[i]-1];
        if(h > 0){
            h--;
        }
        for(;j+h<sz&&i+h<sz;h++){
            if(S[j+h] != S[i+h]){
                break;
            }
        }
        lcp[rnk[i]-1] = h;
    }
}
