typedef complex<double> C;
typedef pair<int,int>P;

const double PI = 4*atan(1.0);

namespace std
{
    bool operator < (const C a, const C b) {
        return a.real() != b.real() ? a.real() < b.real() : a.imag() < b.imag();
    }
}

struct L : public vector<C>
{
    L(){}
    L(const C a, const C b) {
        push_back(a); push_back(b);
    }
};

bool eq(double a,double b)
{
  return (-EPS<a-b&&a-b<EPS);
}

bool eq(C c1,C c2)
{
  return (eq(c1.real(),c2.real()) && eq(c1.imag(),c2.imag()));
}

//条件付きsqrt
double Sqrt(double x)
{
    if(x<0) return 0;
    else    return sqrt(x);
}

//正規化
C normalize(C c)
{
    return c / abs(c);
}

//角度(rad)
double getarg(C a,C b){
    return arg(b*conj(a));
}

//外積
double cross(const C a, const C b)
{
    return imag(conj(a)*b);
}
//内積
double dot(const C a, const C b)
{
    return real(conj(a)*b);
}

int ccw(C a, C b, C c)
{
    b -= a; c -= a;
    if(cross(b, c) > 0)   return +1;       // counter clockwise
    if(cross(b, c) < 0)   return -1;       // clockwise
    if(dot(b, c) < 0)     return +2;       // c--a--b on line
    if(norm(b) < norm(c)) return -2;       // a--b--c on line
    return 0;   //b--a--c on line
}
//直線どうしの交差判定(同一直線はTrue)
bool intersectLL(const L &l, const L &m)
{
    return abs(cross(l[1]-l[0], m[1]-m[0])) > EPS || abs(cross(l[1]-l[0], m[0]-l[0])) < EPS;
}
//直線と線分の交差判定(一点共有も交差と判定)
bool intersectLS(const L &l, const L &s)
{
    return cross(l[1]-l[0], s[0]-l[0]) * cross(l[1]-l[0], s[1]-l[0]) < EPS;
}
//直線と点の交差(共有)判定
bool intersectLP(const L &l, const C p)
{
    return abs(cross(l[1]-p, l[0]-p)) < EPS;
}
//線分どうしの交差判定(一点共有も交差と判定)
bool intersectSS(const L &s, const L &t)
{
    return ccw(s[0],s[1],t[0])*ccw(s[0],s[1],t[1]) <= 0 && ccw(t[0],t[1],s[0])*ccw(t[0],t[1],s[1]) <= 0;
}
//線分と点の交差(共有)判定
bool intersectSP(const L &s, const C p)
{
    return abs(s[0]-p)+abs(s[1]-p)-abs(s[1]-s[0]) < EPS;
}
//直線および線分の交点
C crosspointLL(const L &l, const L &m)
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
C projection(const L &l, const C p)
{
    double t = dot(p-l[0], l[0]-l[1]) / norm(l[0]-l[1]);
    return l[0] + t*(l[0]-l[1]);
}
//crosspointCLに使用する関数(命名が謎です)
double gettime(C c1,C c2)
{
    return (dot(c1,c2) < 0 ? -1.0 : 1.0 ) * abs(c2) / abs(c1);
}
//円と直線の交点
vector<C> crosspointCL(C c1,double r1,L l)
{
    C a=l[0], b=l[1];
    vector<C> res;
    C base=b-a,  target=projection(L(a,b),c1);
    double length=abs(base), h=abs(c1-target);
    base/=length;
    if(r1+EPS<h)    return res;
    double w=Sqrt(r1*r1-h*h);
    double LL=gettime(normalize(b-a),target-a)-w,RR=LL+w*2.0;
    res.push_back(a+base*LL);
    if(eq(LL,RR))   return res;
    res.push_back(a+base*RR);
    return res;
}

//円と線分の交点
vector<C> crosspointCS(C c1,double r1,L s)
{
    vector<C> tmp=crosspointCL(c1,r1,s);
    vector<C> res;
    rep(i,tmp.size()){
        if(eq(abs(s[1]-s[0]),abs(s[0]-tmp[i])+abs(s[1]-tmp[i]))){
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
