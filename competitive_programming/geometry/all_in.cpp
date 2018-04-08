#define EPS 1e-10
#define rep(i,n) for(int i=0;i<(int)(n);++i)

typedef complex<double> C;

const double PI = 4*atan(1.0);

bool eq(double a,double b)
{
  return (-EPS<a-b&&a-b<EPS);
}

namespace std
{
    bool operator < (const C a, const C b) {
        return a.real() != b.real() ? a.real() < b.real() : a.imag() < b.imag();
    }
    //点をsetとかmapにつめたいとき(誤差を1e-10とか厳しくし過ぎるとバグる)
    // bool operator<(const C a, const C b){
    //     return abs(a.real()-b.real())>EPS ? a.real()<b.real()-EPS : a.imag()<b.imag()-EPS;
    // }
    bool operator==(const C a, const C b){
        return (eq(a.real(),b.real()) && eq(a.imag(),b.imag()));
    }
    bool operator!=(const C a, const C b){
        return !(a == b);
    }
}

struct L : public vector<C>
{
    L(){}
    L(const C a, const C b) {
        push_back(a); push_back(b);
    }
};

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
//回転
C rot(C c,double th)
{
    return c * C(cos(th),sin(th));
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
bool intersectLL(const L& l, const L& m)
{
    return abs(cross(l[1]-l[0], m[1]-m[0])) > EPS || abs(cross(l[1]-l[0], m[0]-l[0])) < EPS;
}
//直線と線分の交差判定(一点共有も交差と判定)
bool intersectLS(const L& l, const L& s)
{
    return cross(l[1]-l[0], s[0]-l[0]) * cross(l[1]-l[0], s[1]-l[0]) < EPS;
}
//直線と点の交差(共有)判定
bool intersectLP(const L& l, const C p)
{
    return abs(cross(l[1]-p, l[0]-p)) < EPS;
}
//線分どうしの交差判定(一点共有も交差と判定)
bool intersectSS(const L& s, const L& t)
{
    return ccw(s[0],s[1],t[0])*ccw(s[0],s[1],t[1]) <= 0 && ccw(t[0],t[1],s[0])*ccw(t[0],t[1],s[1]) <= 0;
}
//線分と点の交差(共有)判定
bool intersectSP(const L& s, const C p)
{
    return abs(s[0]-p)+abs(s[1]-p)-abs(s[1]-s[0]) < EPS;
}
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
//点pを直線lを軸として対称移動
C reflection(const L &l, const C p)
{
    return p + (projection(l, p) - p)*2.0;
}
//点と直線の距離
double distanceLP(const L &l, const C p)
{
    return abs(p - projection(l, p));
}
//直線と直線の距離
double distanceLL(const L &l, const L &m)
{
    return intersectLL(l, m) ? 0 : distanceLP(l, m[0]);
}
//直線と線分の距離
double distanceLS(const L &l, const L &s)
{
    if (intersectLS(l, s)) return 0;
    return min(distanceLP(l, s[0]), distanceLP(l, s[1]));
}
//線分と点の距離
double distanceSP(const L &s, const C p)
{
    const C r = projection(s, p);
    if (intersectSP(s, r)) return abs(r - p);
    return min(abs(s[0] - p), abs(s[1] - p));
}
//線分と線分の距離
double distanceSS(const L &s, const L &t)
{
    if (intersectSS(s, t)) return 0;
    return min(min(distanceSP(s, t[0]), distanceSP(s, t[1])),min(distanceSP(t, s[0]), distanceSP(t, s[1])));
}

double getarea(C c1,double r1,C a,C b)
{
    C va=c1-a,vb=c1-b;
    double A=abs(va),B=abs(vb);
    double f=cross(va,vb),d=distanceSP(L(a,b),c1),res=0;
    if(eq(f,0.0))   return 0;
    if(A < r1+EPS && B < r1+EPS)    return f*0.5;
    if(d>r1-EPS)    return r1*r1*M_PI*getarg(va,vb)/(2.0*M_PI);
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
//凸包を求める(O(nlogn))
vector<C> convex_hull(vector<C> ps)
{
    int n = (int)ps.size(), k = 0;
    sort(ps.begin(), ps.end());
    vector<C> ch(2*n);
    for (int i = 0; i < n; ch[k++] = ps[i++]){
        while (k >= 2 && ccw(ch[k-2], ch[k-1], ps[i]) <= 0) k--;
    }
    for (int i = n-2, t = k+1; i >= 0; ch[k++] = ps[i--]){
        while (k >= t && ccw(ch[k-2], ch[k-1], ps[i]) <= 0) k--;
    }
    ch.resize(k-1);
    return ch;
}
//凸性判定
bool isconvex(const vector<C>& ps)
{
    rep(i,ps.size()){
        if (ccw(ps[(i+ps.size()-1) % ps.size()],ps[i],ps[(i+1) % ps.size()])) return false;
    }
    return true;
}
//多角形の符号付き面積(左回りが正)
double area(const vector<C>& ps)
{
    double A = 0;
    rep(i,ps.size()){
        A += cross(ps[i],ps[(i+1) % ps.size()]);
    }
    return A / 2.0;
}
//凸多角形を直線で切断した時の左側の図形
vector<C> convex_cut(const vector<C>& ps, const L& l)
{
    vector<C> Q;
    rep(i,ps.size()){
        C A = ps[i], B = ps[(i+1)%ps.size()];
        if (ccw(l[0], l[1], A) != -1) Q.push_back(A);
        if (ccw(l[0], l[1], A)*ccw(l[0], l[1], B) < 0)
            Q.push_back(crosspointLL(L(A, B),l));
    }
    return Q;
}
//垂直二等分線
L bisector(C a, C b){
    C A = (a + b) * C(0.5, 0);
    return L(A, A + rot(b - a, PI/2));
}
//ボロノイ図(正確にはvoronoi_cellを求める)
//愚直であるため1つのvoronoi_cellを求めるのにかかる計算量がO(n^2)
vector<C> voronoi(vector<C> poly, vector<C>& p, int s){
    rep(i,(int)p.size()){
        if (i != s){
            poly = convex_cut(poly, bisector(p[s], p[i]));
        }
    }
    return poly;
}
//点が多角形に包含されているか(0は含まれない,1は辺上,2は含まれる)
int contains(const vector<C>& ps, const C p)
{
    bool flag = false;
    rep(i,ps.size()) {
        C a = ps[i] - p, b = ps[(i+1)%ps.size()] - p;
        if (imag(a) > imag(b)) swap(a, b);
        if (imag(a) <= 0 && 0 < imag(b)){
            if (cross(a, b) < 0) flag = !flag;
        }
        if (cross(a, b) == 0 && dot(a, b) <= 0) return 1;
  }
  return flag ? 2 : 0;
}
//凸多角形の交差
vector<C> convex_intersection(const vector<C>& ps,const vector<C>& qs)
{
	vector<C> rs;
	int a = ps.size(),b = qs.size();
	rep(i,a){
        if(contains(qs,ps[i])){
            rs.push_back(ps[i]);
        }
    }
	rep(i,b){
        if(contains(ps,qs[i])){
            rs.push_back(qs[i]);
        }
    }
    rep(i,a){
        rep(j,b){
            L l1(ps[i],ps[(i+1)%a]),l2(qs[j],qs[(j+1)%b]);
		    if(intersectSS(l1,l2)){
                rs.push_back(crosspointLL(l1,l2));
            }
        }
	}
	sort(rs.begin(),rs.end());
	rs.erase(unique(rs.begin(),rs.end()),rs.end());
	if(rs.size() <= 1){
        return rs;
    }
	return convex_hull(rs);
}
//凸多角形の直径を求める(キャリパー法)
//maxi,maxjが最遠点対となる
double convex_diameter(const vector<C>& ps)
{
    const int n = (int)ps.size();
    int is = 0, js = 0;
    for (int i = 1; i < n; ++i) {
        if (imag(ps[i]) > imag(ps[is])) is = i;
        if (imag(ps[i]) < imag(ps[js])) js = i;
    }
    double maxd = abs(ps[is]-ps[js]);
    int i, maxi, j, maxj;
    i = maxi = is;
    j = maxj = js;
    do{
        if (cross(ps[(i+1)%ps.size()]-ps[i],ps[(j+1)%ps.size()]-ps[j]) >= 0) j = (j+1) % n;
        else i = (i+1) % n;
        if (abs(ps[i]-ps[j]) > maxd) {
            maxd = abs(ps[i]-ps[j]);
            maxi = i; maxj = j;
        }
    } while (i != is || j != js);
    return maxd;
}

bool compyx(C c1,C c2)
{
    return c1.imag() != c2.imag() ? c1.imag() < c2.imag() : c1.real() < c2.real();
}

double closest_pair(C* a, int n)
{
    if(n<=1) return 1e100;
    int m=n/2;
    double x=a[m].real();
    double d=min(closest_pair(a,m),closest_pair(a+m,n-m));
    inplace_merge(a,a+m,a+n,compyx);
    vector<C> b;
    rep(i,n){
        if(abs(x-a[i].real())>=d) continue;
        rep(j,b.size()){
            C dp=a[i]-b[b.size()-1-j];
            if(dp.imag()>=d) break;
            d=min(d,abs(dp));
        }
        b.push_back(a[i]);
    }
    return d;
}
//最近点対を求める
double compute_shortest(C* a,int n)
{
    sort(a,a+n);
    return closest_pair(a,n);
}
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
    if(s == t){
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
