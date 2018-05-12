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
//bを中心としてaをth(rad)回転
C rot(C a,C b,double th)
{
    return b + (a-b) * C(cos(th),sin(th));
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
