#include "../header.hpp"

// insert→solveでansに全クエリに対する結果が格納
// add,del を変更していろいろなクエリに対応する
// 以下は区間内の数の種類数についてのクエリが飛んでくる場合


//現在の状態および値
const int MAX_VAL = 30000;
int a[MAX_VAL]; //要素
int cnt[MAX_VAL]; //区間内のiの個数
int res;        //区間内の種類の数

class Mo{
private:
    vector<int> left, right;
    int w;
    void add(int id);
    void del(int id);

public:
    vector<int> ans;

    Mo(int n) : w((int)sqrt(n)){}
    //クエリ[l, r)
    void insert(int l, int r){
        left.push_back(l), right.push_back(r);
    }
    void solve(){
        int sz = (int)left.size();
        int nl = 0, nr = 0;
        vector<int> ord(sz);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b){
            const int c = left[a] / w, d = left[b] / w;
            return (c == d) ? ((c & 1) ? (right[a] < right[b]) : (right[b] < right[a])) : (c < d);
        });
        ans.resize(sz);
        for(int i = 0; i < sz; i++){
            const int id = ord[i];  //order:クエリのsort後のorder→元のorder
            while(nl > left[id]) add(--nl);
            while(nr < right[id]) add(nr++);  //add
            while(nl < left[id]) del(nl++);
            while(nr > right[id]) del(--nr);  //del
            ans[id] = res;
        }
    }
};

//idは元の配列のインデックス
void Mo::add(int id)
{
    if(cnt[a[id]] == 0){
        cnt[a[id]]++;
        res++;
    }
}

void Mo::del(int id)
{
    if(cnt[a[id]] == 1){
        cnt[a[id]]--;
        res--;
    }
}
