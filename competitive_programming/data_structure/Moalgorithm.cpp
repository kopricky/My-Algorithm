// insert→build→solveでansに全クエリに対する結果が格納
// 以下は区間内の数の種類数についてのクエリが飛んでくる場合
// 別の場合はadd,del,solveをいじる

//現在の状態および値
int cnt[MAX_VAL]; //区間内のiの個数
int res;        //区間内の種類の数

struct Mo{
    vector<int> left,right,order,ans; vector<bool> v; int width,nl,nr,ptr,sz;
    Mo(int n) : v(n,false), width((int) sqrt(n)), nl(0), nr(0), ptr(0){}
    void insert(int l, int r){ left.push_back(l); right.push_back(r); }   //クエリ[l,r)
    void build(){
        sz = (int)left.size();
        order.resize(sz),ans.resize(sz);
        iota(begin(order), end(order), 0);
        sort(begin(order), end(order), [&](int a, int b){
            if(left[a] / width != left[b] / width) return left[a] < left[b];
            return right[a] < right[b]; });
    }
    int process(){    //クエリを１つ進め、クエリのidを返す
        if(ptr == (int)order.size()) return (-1);    //クエリ処理が終了
        const int id = order[ptr];  //order:クエリのsort後のorder→元のorder
        while(nl > left[id]) nl--,distribute(nl); while(nr < right[id]) distribute(nr),nr++;  //add
        while(nl < left[id]) distribute(nl),nl++; while(nr > right[id]) nr--,distribute(nr);  //del
        return (order[ptr++]);
    }
    inline void distribute(int id){
        v[id].flip();  //現在保持しているクエリ内に入っているかいないか
        if(v[id]) add(id); else del(id);
    }
    void solve(){
        for(int i=0;i<(int)sz;i++){
            ans[process()] = res;
        }
    }
    void add(int id); void del(int id);
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
