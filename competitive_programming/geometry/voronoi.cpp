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
