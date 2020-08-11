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
    const int width;
    void add(const int id);
    void del(const int id);

public:
    vector<int> ans;

    Mo(const int n) : width((int)sqrt(n)){}
    // クエリ[l,r)
    void insert(const int l, const int r){
        left.push_back(l), right.push_back(r);
    }
    void solve(){
        const int sz = (int)left.size();
        int nl = 0, nr = 0;
        vector<int> ord(sz);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](const int a, const int b){
            const int c = left[a] / width, d = left[b] / width;
            return (c == d) ? ((c & 1) ? (right[b] < right[a]) : (right[a] < right[b])) : (c < d);
        });
        ans.resize(sz);
        for(const int id : ord){
            while(nl > left[id]) add(--nl);
            while(nr < right[id]) add(nr++);  //add
            while(nl < left[id]) del(nl++);
            while(nr > right[id]) del(--nr);  //del
            ans[id] = res;
        }
    }
};

//idは元の配列のインデックス
void Mo::add(const int id)
{
    ++cnt[a[id]];
    if(cnt[a[id]] == 1) ++res;
}

void Mo::del(const int id)
{
    --cnt[a[id]];
    if(cnt[a[id]] == 0) --res;
}
