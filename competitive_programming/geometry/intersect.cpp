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
