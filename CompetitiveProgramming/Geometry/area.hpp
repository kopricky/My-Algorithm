//多角形の符号付き面積(左回りが正)
double area(const vector<C>& ps)
{
    double A = 0;
    rep(i,ps.size()){
        A += cross(ps[i],ps[(i+1) % ps.size()]);
    }
    return A / 2.0;
}
double getarea(C c1,double r1,C a,C b)
{
    C va=c1-a,vb=c1-b;
    double A=abs(va),B=abs(vb);
    double f=cross(va,vb),d=distanceSP(L(a,b),c1),res=0;
    if(eq(f,0.0))   return 0;
    if(A < r1+EPS && B < r1+EPS)    return f*0.5;
    if(d>r1-EPS)    return r1*r1*PI*getarg(va,vb)/(2.0*PI);
    vector<C> u=crosspointCS(c1,r1,L(a,b));
    u.insert(u.begin(),a),u.push_back(b);
    for(int i=0;i+1<(int)u.size();i++){
        res+=getarea(c1,r1,u[i],u[i+1]);
    }
    return res;
}
//円と多角形の共通部分の面積
double getcrossarea(const vector<C>& t,C c1,double r1)
{
    int n = (int)t.size();
    if(n<3) return 0;
    double res=0;
    rep(i,n){
      C a=t[i], b=t[(i+1)%n];
      res += getarea(c1,r1,a,b);
    }
    return res;
}
