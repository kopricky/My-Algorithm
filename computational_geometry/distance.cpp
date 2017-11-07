//点pを直線l上に射影
C projection(const L &l, const C p)
{
    double t = dot(p-l[0], l[0]-l[1]) / norm(l[0]-l[1]);
    return l[0] + t*(l[0]-l[1]);
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
