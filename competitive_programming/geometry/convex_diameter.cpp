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
