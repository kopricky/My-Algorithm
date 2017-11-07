//2円の位置関係を示す(返り値は2円の間の共通接線の数)
int getstateCC(C c1,double r1,C c2,double r2)
{
    double d=abs(c1-c2);
    if(d>r1+r2+EPS)return 4;
    if(d>r1+r2-EPS)return 3;
    if(d>abs(r1-r2)+EPS)return 2;
    if(d>abs(r1-r2)-EPS)return 1;
    return 0;
}
//点から円へ接線を引いた時の接点
C gettangentCP_(C c1,double r1,C p,int flg){
    C base=c1-p;
    double w=Sqrt(norm(base)-r1*r1);
    C s=p+base*C(w,r1 * flg)/norm(base)*w;
    return s;
}
//点から円への接線
vector<L> gettangentCP(C c1,double r1,C p){
    vector<L> res;
    C s=gettangentCP_(c1,r1,p,1);
    C t=gettangentCP_(c1,r1,p,-1);
    //点が円の周上にある場合
    if(eq(s,t)){
        res.push_back(L(s,s+(c1-p)*C(0,1)));
    }else{
        res.push_back(L(p,s));
        res.push_back(L(p,t));
    }
    return res;
}

//2円の共通内接線を求める
L getintangent(C c1,double r1,C c2,double r2,double flg)
{
    C base=c2-c1;
    double w=r1+r2;
    double h=Sqrt(norm(base)-w*w);
    C k=base*C(w,h*flg)/norm(base);
    return L(c1+k*r1,c2-k*r2);
}
//2円の共通外接線を求める
L getouttangent(C c1,double r1,C c2,double r2,double flg)
{
    C base=c2-c1;
    double h=r2-r1;
    double w=Sqrt(norm(base)-h*h);
    C k=base*C(w,h*flg)/norm(base)*C(0,flg);
    return L(c1+k*r1,c2+k*r2);
}
//2円の共通接線を求める(各直線の２点はそれぞれの円の接点)
vector<L> gettangentCC(C c1,double r1,C c2,double r2)
{
    vector<L> res;
    double d=abs(c1-c2);
    if(d>r1+r2+EPS)  res.push_back(getintangent(c1,r1,c2,r2,1));
    if(d>r1+r2-EPS)  res.push_back(getintangent(c1,r1,c2,r2,-1));
    if(d>abs(r1-r2)+EPS)    res.push_back(getouttangent(c1,r1,c2,r2,1));
    if(d>abs(r1-r2)-EPS)    res.push_back(getouttangent(c1,r1,c2,r2,-1));
    return res;
}
