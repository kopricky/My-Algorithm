const int N = 1 << 17;	//10^5程度

int n_,dat[2*N-1];

//index:iがdat[n_-1+i]と対応
void init(int x)
{
	n_=1;
	while(n_<x){
		n_*=2;
	}
	rep(i,2*n_-1){
		dat[i] = INF;
	}
}

void update(int k,int a)
{
	k += n_-1;
	dat[k] = a;
	while(k>0){
		k = (k-1)/2;
		dat[k] = min(dat[2*k+1],dat[2*k+2]);
	}
}

//外からはquery(a,b,0,0,n_)として呼ぶ。(n_とすることに注意)
int query(int a,int b,int k,int l,int r)
{
	if(r <= a || b <= l){
		return INF;
	}
	if(a <= l && r <= b){
		return dat[k];
	}else{
		int vl = query(a,b,2*k+1,l,(l+r)/2);
		int vr = query(a,b,2*k+2,(l+r)/2,r);
		return min(vl,vr);
	}
}
