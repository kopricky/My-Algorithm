#include "../header.hpp"

//01ナップザック問題に対する分枝限定法を用いたアルゴリズム
void dfs(const int n, const long long W, const long long value, const long long weight, long long& opt,
        const int index, const vector<long long>& rsumw, const vector<int>& rminw, const vector<pair<int, int> >& vec)
{
    if(index == n){
        opt = max(opt, value);
        return;
    }
    if(rsumw[index] <= W - weight){
        long long tvalue = value;
        for(int i = index; i < n; ++i){
            tvalue += vec[i].first;
        }
        opt = max(opt, tvalue);
        return;
    }
    if(rminw[index] > W - weight) return;
    //使う
    if(weight + vec[index].second <= W){
        opt = max(opt, value + vec[index].first);
        long long tweight = weight + vec[index].second, tvalue = value + vec[index].first;
        for(int i = index + 1; i < n; ++i){
            if(tweight + vec[i].second <= W){
                tweight += vec[i].second, tvalue += vec[i].first;
            }else{
                tvalue += vec[i].first * (W - tweight) / vec[i].second;
                break;
            }
        }
        if(opt < tvalue){
            dfs(n, W, value + vec[index].first, weight + vec[index].second, opt, index + 1, rsumw, rminw, vec);
        }
    }
    //使わない
    long long tweight = weight, tvalue = value;
    for(int i = index + 1; i < n; ++i){
        if(tweight + vec[i].second <= W){
            tweight += vec[i].second, tvalue += vec[i].first;
        }else{
            tvalue += vec[i].first * (W - tweight) / vec[i].second;
            break;
        }
    }
    if(opt < tvalue){
        dfs(n, W, value, weight, opt, index + 1, rsumw, rminw, vec);
    }
}

//vec には(価値, 重さ)の順で詰める
long long solve(const int n, const long long W, vector<pair<int, int> >& vec)
{
    sort(vec.begin(), vec.end(), [&](const pair<int, int>& a, const pair<int, int>& b){
        return (long long)a.first * b.second > (long long)a.second * b.first;
    });
    vector<long long> rsumw(n + 1, 0);
    vector<int> rminw(n + 1, numeric_limits<int>::max());
    for(int i = n - 1; i >= 0; --i){
        rsumw[i] = rsumw[i + 1] + vec[i].second, rminw[i] = min(rminw[i + 1], vec[i].second);
    }
    long long opt = 0;
    dfs(n, W, 0, 0, opt, 0, rsumw, rminw, vec);
    return opt;
}
