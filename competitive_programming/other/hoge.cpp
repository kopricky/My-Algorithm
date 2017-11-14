#include <bits/stdc++.h>
#define ll long long
#define INF 1000000005
#define MOD 1000000007
#define EPS 1e-10
#define rep(i,n) for(int i=0;i<(int)n;++i)
#define all(v) (v).begin(),(v).end()

using namespace std;

typedef pair<int,int>P;

const int MAX_N = 100005;

int a[MAX_N];
int n;

int main()
{
    //vectorのinsert
    vector<int> v1{2,3,5,1,6,7,9,1};
    vector<int> vv(5,0);
    v1.insert(v1.begin()+3,10); //id=3に挿入(挿入された値を指すイテレータを返す)
    cout << "insert1 ";
    rep(i,v1.size()){
        cout << v1[i] << " ";
    }
    cout << "\n";
    v1.insert(v1.begin()+5,11,5); //id=5から5個分11を挿入
    cout << "insert2 ";
    rep(i,v1.size()){
        cout << v1[i] << " ";
    }
    cout << "\n";
    v1.insert(v1.end(),all(vv));    //２つのvectorを結合
    cout << "insert3 ";
    rep(i,v1.size()){
        cout << v1[i] << " ";
    }
    cout << "\n";

    //count,count_if
    vector<int> v2{2,1,4,4,4,3,2,1};
    cout << "count ";
    cout << count(v2.begin(),v2.end(),3) << "\n";
    cout << "count_if ";
    cout << count_if(v2.begin(),v2.end(),[](int x){return x >= 3;}) << "\n";

    //find,find_if
    //vectorの関数としてでなくalgorithmの関数にある
    vector<int> v3{5,2,8,0,3,2,1};
    cout << "find ";
    cout << find(v3.begin(),v3.end(),3) - v3.begin() << "\n";
    cout << "find_if ";
    cout << find_if(v3.begin(),v3.end(),[](int c){return c < 2;}) - v3.begin() << "\n";

    //remove(ソート済みコンテナで使える)
    vector<int> v4{4,1,2,3,5,1,2,1};
    sort(v4.begin(),v4.end());
    v4.erase(remove(v4.begin(),v4.end(),1),v4.end());   //最後に残ったゴミを削除
    cout << "remove ";
    rep(i,v4.size()){
        cout << v4[i] << " ";
    }
    cout << "\n";

    //merge(ソート済みシーケンスで使える)
    vector<int> v5{5,2,9,14,8,7};
    vector<int> v6{10,6,1,4,0,3};
    vector<int> res(12,0);    //あらかじめ領域を確保しておく
    sort(v5.begin(),v5.end()),sort(v6.begin(),v6.end());
    merge(v5.begin(),v5.end(),v6.begin(),v6.end(),res.begin());
    cout << "merge1 ";
    rep(i,res.size()){
        cout << res[i] << " ";
    }
    cout << "\n";
    //以下降順の場合
    sort(v5.begin(),v5.end(),greater<int>()),sort(v6.begin(),v6.end(),greater<int>());
    merge(v5.begin(),v5.end(),v6.begin(),v6.end(),res.begin(),greater<int>());
    cout << "merge2 ";
    rep(i,res.size()){
        cout << res[i] << " ";
    }
    cout << "\n";

    //replace,replace_copy
    vector<int> v7{8,3,5,1,2,3,5,6,1,3,5,1,3,5,2,3,6,8};
    replace(v7.begin(),v7.end(),2,11);
    cout << "replace ";
    rep(i,v7.size()){
        cout << v7[i] << " ";
    }
    cout << "\n";
    replace_if(v7.begin(),v7.end(),[](int a){return a >= 5;},0);
    cout << "replace_if ";
    rep(i,v7.size()){
        cout << v7[i] << " ";
    }
    cout << "\n";

    //copy
    vector<int> v8{5,3,2,2,1,5,6,7,3,4,6,4};
    vector<int> v9(v8.size(),0);
    copy(v8.begin(),v8.end(),v9.begin());
    cout << "copy ";
    rep(i,v9.size()){
        cout << v9[i] << " ";
    }
    cout << "\n";
    //copy(set→vector) 昇順にコピーされる
    set<int> s{3,5,1,2,7,10,8,6,13};
    vector<int> res2(s.size(),0);
    copy(all(s),res2.begin());
    cout << "copy2 ";
    rep(i,res2.size()){
        cout << res2[i] << " ";
    }
    cout << "\n";

    //transform
    //i項演算子を用いる場合
    vector<int> v10{7,3,4,5,5,6,7,2,3,4,3,1};
    vector<double> v11(v10.size(),0);
    transform(v10.begin(),v10.end(),v11.begin(),[](int a){return 1.0/a;});
    cout << "transform1 ";
    rep(i,v11.size()){
        cout << v11[i] << " ";
    }
    cout << "\n";
    //2項演算子を用いる場合
    vector<int> v12{6,4,2,1,2,3,5,3,1,4,6,1,2,3};
    transform(all(v10),v12.begin(),v11.begin(),divides<double>());  //組み込み関数を用いた
    cout << "transform2 ";
    rep(i,v11.size()){
        cout << v11[i] << " ";
    }
    cout << "\n";

    //swapももちろん使える
    cout << "swap ";
    swap(v10,v12);
    rep(i,v10.size()){
        cout << v10[i] << " ";
    }
    cout << "\n";

    //assign(初期化にも使える)
    vector<int> v13{3,1,2,3,3,5,7,9,9};
    vector<int> v14;
    v14.assign(v13.begin(),v13.end());
    cout << "assign ";
    rep(i,v14.size()){
        cout << v14[i] << " ";
    }
    cout << "\n";

    //mismatch
    vector<int> v15{1,2,5,3,5,6,7,9,2,3};
    vector<int> v16{1,2,5,3,5,7,8,9,9,6,3,5};
    auto mis = mismatch(all(v15),v16.begin());
    cout << "mismatch ";
    cout << mis.first - v15.begin() << "\n";

    //search(部分シーケンスの検索)
    vector<int> v17{4,2,5,6,5,6,4,3,6,3,4};
    vector<int> v18{4,3};
    cout << "search ";
    cout << search(all(v17),all(v18)) - v17.begin() << "\n";

    //set_difference,set_union(set_intersection,set_symmetricもある)
    vector<int> v19{3,4,6,7,9};
    vector<int> v20{1,2,3,4,6};
    vector<int> res3(v19.size()+v20.size(),0);
    cout << "set_difference ";
    for(auto i = res3.begin();i != set_difference(all(v19),all(v20),res3.begin());i++){
        cout << *i << " ";
    }
    cout << "\n";
    cout << "set_union ";
    for(auto i = res3.begin();i != set_union(all(v19),all(v20),res3.begin());i++){
        cout << *i << " ";
    }
    cout << "\n";

    //mapを巡回する,mapのlower_bound(mapの宣言時にgreater<int>()を組み込むと逆順に並べられる)
    map<int,char> mp{{3,'a'},{4,'b'},{1,'c'}};
    cout << "map_loop ";
    for(auto &i:mp){
        cout << i.first << " " << i.second << "  "; //for_eachでの書き方
    }
    cout << "\n";
    auto it = mp.lower_bound(2);
    cout << "map_lower_bound ";
    cout << it->first << " " << it->second << "\n";

    //multimapで同じキーを持つ値の回り方
    multimap<int,char> mtp{{3,'a'},{4,'b'},{1,'c'},{3,'g'},{3,'e'},{1,'u'}};
    auto p = mtp.equal_range(1);
    cout << "equal_range ";
    for(auto i = p.first;i != p.second;i++){
        cout << i->second << " ";
    }
    cout << "\n";
    //boundを使った場合
    cout << "find ";
    for(auto i = mtp.lower_bound(3);i != mtp.upper_bound(3);i++){
        cout << i->second << " ";
    }
    cout << "\n";

    //inner_product(内積計算に使える)
    vector<int> v21{9,5,3};
    vector<int> v22{7,3,2};
    cout << "inner_product ";
    cout << inner_product(all(v21),v22.begin(),0) << "\n";

    //partial_sum(累積和に使える)
    vector<int> v23{5,3,6,7,2,4,6,7};
    vector<int> v24(v23.size(),0);
    partial_sum(all(v23),v24.begin());
    cout << "partial_sum ";
    rep(i,v24.size()){
        cout << v24[i] << " ";
    }
    cout << "\n";

    //集合TでSの部分集合全体をなめる(空集合は除く)
    for(int T=S;T>0;T=(T-1)&S){}
    __builtin_popcount();
	return 0;
}
