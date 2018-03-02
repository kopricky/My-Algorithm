//mapやsetにつめて傾きでソートしたいときに誤差死しないために...
struct slope{
    int x,y;
    slope(){}
    slope(int a,int b) : x(a), y(b){}
    bool operator< (const slope& another) const {
        if((ll)x*another.x > 0){
            return (ll)y*another.x < (ll)x*another.y;
        }else if((ll)x*another.x < 0){
            return (ll)y*another.x > (ll)x*another.y;
        }else{
            return (x==0)?((y<0)&&(another.x||(another.y>0))):(another.y>0);
        }
    }
};

//-∞と∞を両方∞と考えたい場合
struct slope{
    int x,y;
    slope(){}
    slope(int a,int b) : x(a), y(b){}
    bool operator< (const slope& another) const {
        if((ll)x*another.x > 0){
            return (ll)y*another.x < (ll)x*another.y;
        }else if((ll)x*another.x < 0){
            return (ll)y*another.x > (ll)x*another.y;
        }else{
            return (another.x != 0 && y < 0) || (x != 0 && another.y > 0);
        }
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
