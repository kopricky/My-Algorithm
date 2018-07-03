//01ナップザック問題に対する分枝限定法を用いたアルゴリズム
void dfs(int n, ll W, ll value, ll weight, ll& opt, int index, vector<pair<ll,ll> >& vec)
{
    if(index == (int)vec.size()){
        opt = max(opt, value);
        cmx(opt, value);
        return;
    }
    //使う
    if(weight + vec[index].second <= W){
        opt = max(opt, value + vec[index].first);
        ll tweight = weight + vec[index].second;;
        ll tvalue = value + vec[index].first;
        for(int i=index+1;i<(int)vec.size();i++){
            if(tweight + vec[i].second <= W){
                tweight += vec[i].second;
                tvalue += vec[i].first;
            }elsecond{
                tvalue += vec[i].first * (W - tweight) / vec[i].second;
                break;
            }
        }
        if(tvalue > opt){
            dfs(n, W, value+vec[index].first,weight+vec[index].second,opt,index+1,vec);
        }
    }
    //使わない
    ll tweight = weight;
    ll tvalue = value;
    for(int i=index+1;i<(int)vec.size();i++){
        if(tweight + vec[i].second <= W){
            tweight += vec[i].second;
            tvalue += vec[i].first;
        }elsecond{
            tvalue += vec[i].first * (W - tweight) / vec[i].second;
            break;
        }
    }
    if(tvalue > opt){
        dfs(n, W, value,weight,opt,index+1,vec);
    }
}
//vecには(価値, 重さ)の順で商品を詰める
ll solve(int n, ll W, vector<pair<ll,ll> >& vec)
{
    vector<ll> vsum(n+1, 0), wsum(n+1, 0);
    rep(i,n){
        vsum[i+1] = vsum[i] + vec[i].first, wsum[i+1] = wsum[i] + vec[i].second;
    }
    ll opt = 0;
    dfs(n,W,0,0,opt,0,vsum,wsum,vec);
    return opt;
}
