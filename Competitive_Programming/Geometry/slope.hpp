// map や set につめて傾きでソートしたいときに誤差死しないために...
struct slope{
    int x, y;
    slope(){}
    slope(int a,int b) : x(a), y(b){}
    bool operator< (const slope& another) const {
        if((long long)x*another.x > 0){
            return (long long)y*another.x < (long long)x*another.y;
        }else if((long long)x*another.x < 0){
            return (long long)y*another.x > (long long)x*another.y;
        }else{
            return (x==0)?((y<0)&&(another.x||(another.y>0))):(another.y>0);
        }
    }
};

//-∞と∞を両方∞と考えたい場合
struct slope{
    int x,y;
    slope(){}
    slope(int a,int b){
        if(a == 0 && b < 0) b = -b;
        x = a, y = b;
    }
    bool operator< (const slope& another) const {
        if((long long)x*another.x > 0){
            return (long long)y*another.x < (long long)x*another.y;
        }else if((long long)x*another.x < 0){
            return (long long)y*another.x > (long long)x*another.y;
        }else{
            return (x==0)?((y<0)&&(another.x||(another.y>0))):(another.y>0);
        }
    }
};

// ラジアンベース
// https://judge.yosupo.jp/problem/sort_points_by_argument
struct slope{
    int x, y;
    slope(const int a, const int b) : x(a), y(b){}
    bool operator<(const slope& s) const {
        if((long long)y * s.y <= 0){
            if(y == 0 && s.y == 0) return (x > s.x);
            if(y == 0 && s.y > 0) return (x >= 0);
            if(y > 0 && s.y == 0)  return (s.x < 0);
            return (y < s.y);
        }else if(((long long)x * s.x <= 0)){
            return (y > 0) ? (x > s.x) : (x < s.x);
        }else{
            return ((long long)y * s.x < (long long)x * s.y);
        }
    }
    friend ostream& operator<<(ostream& os, const slope& s){
        return (os << "{" << s.x << "," << s.y << "}");
    }
};

map<slope,vector<int> > mp;

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    mp[slope(0,2)].pb(0);
    mp[slope(0,1)].pb(1);
    mp[slope(0,-1)].pb(2);
    mp[slope(1,1)].pb(3);
    mp[slope(1,-1)].pb(4);
    mp[slope(-1,1)].pb(5);
    mp[slope(-1,1)].pb(6);
    for(auto&it : mp){
        for(auto& it2 : it.second){
            cout << it2 << " ";
        }
        cout << "\n";
    }
    return 0;
}
