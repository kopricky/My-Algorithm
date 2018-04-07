//直線および線分の交点
C crosspointLL(const L& l, const L& m)
{
    double A = cross(l[1] - l[0], m[1] - m[0]);
    double B = cross(l[1] - l[0], l[1] - m[0]);
    //同一直線のとき
    if(abs(A) < EPS && abs(B) < EPS){
        return m[0];
    }
    return m[0] + B / A * (m[1] - m[0]);
}
//点pを直線l上に射影
C projection(const L& l, const C p)
{
    double t = dot(p-l[0], l[0]-l[1]) / norm(l[0]-l[1]);
    return l[0] + t*(l[0]-l[1]);
}
double get(C c1,C c2)
{
    return (dot(c1,c2) < 0 ? -1.0 : 1.0 ) * abs(c2) / abs(c1);
}
//円と直線の交点
vector<C> crosspointCL(C c1,double r1,const L& l)
{
    C a=l[0], b=l[1];
    vector<C> res;
    C base=b-a,  target=projection(L(a,b),c1);
    double length=abs(base), h=abs(c1-target);
    base/=length;
    if(r1+EPS<h)    return res;
    double w=Sqrt(r1*r1-h*h);
    double LL=get(normalize(b-a),target-a)-w,RR=LL+w*2.0;
    res.push_back(a+base*LL);
    if(eq(LL,RR))   return res;
    res.push_back(a+base*RR);
    return res;
}

//円と線分の交点
vector<C> crosspointCS(C c1,double r1,const L& s)
{
    vector<C> tmp=crosspointCL(c1,r1,s);
    vector<C> res;
    rep(i,tmp.size()){
        if(abs(s[1]-s[0]) == abs(s[0]-tmp[i])+abs(s[1]-tmp[i])){
            res.push_back(tmp[i]);
        }
    }
    return res;
}
//円どうしの交点
L crosspointCC(const C c1, const double r1, const C c2, const double r2)
{
    C a = conj(c2-c1), b = (r2*r2-r1*r1-(c2-c1)*conj(c2-c1)), c = r1*r1*(c2-c1);
    C d = b*b-4.0*a*c;
    C z1 = (-b+sqrt(d))/(2.0*a)+c1, z2 = (-b-sqrt(d))/(2.0*a)+c1;
    return L(z1, z2);
}
