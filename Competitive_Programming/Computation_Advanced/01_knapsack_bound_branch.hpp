#include "../header.hpp"

//01ナップザック問題に対する分枝限定法を用いたアルゴリズム
void dfs(int n, long long W, long long value, long long weight, long long& opt, int index, vector<pair<long long,long long> >& vec)
{
    if(index == (int)vec.size()){
        opt = max(opt, value);
        return;
    }
    //使う
    if(weight + vec[index].second <= W){
        opt = max(opt, value + vec[index].first);
        long long tweight = weight + vec[index].second;;
        long long tvalue = value + vec[index].first;
        for(int i=index+1;i<(int)vec.size();i++){
            if(tweight + vec[i].second <= W){
                tweight += vec[i].second;
                tvalue += vec[i].first;
            }else{
                tvalue += vec[i].first * (W - tweight) / vec[i].second;
                break;
            }
        }
        if(tvalue > opt){
            dfs(n, W, value+vec[index].first,weight+vec[index].second,opt,index+1,vec);
        }
    }
    //使わない
    long long tweight = weight;
    long long tvalue = value;
    for(int i=index+1;i<(int)vec.size();i++){
        if(tweight + vec[i].second <= W){
            tweight += vec[i].second;
            tvalue += vec[i].first;
        }else{
            tvalue += vec[i].first * (W - tweight) / vec[i].second;
            break;
        }
    }
    if(tvalue > opt){
        dfs(n, W, value,weight,opt,index+1,vec);
    }
}
//vecには(価値, 重さ)の順で商品を詰める
long long solve(int n, long long W, vector<pair<long long,long long> >& vec)
{
    long long opt = 0;
    dfs(n,W,0,0,opt,0,vec);
    return opt;
}
