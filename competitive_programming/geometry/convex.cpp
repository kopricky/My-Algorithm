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
