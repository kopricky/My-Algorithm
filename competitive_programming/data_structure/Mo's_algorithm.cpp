// insertの後にbuildを忘れずに g++11でコンパイルする
//現在の状態および値
int cnt[MAX_N]; //区間内のiの個数
int kind;        //区間内の種類の数

struct Mo{
    vector<int> left,right,order; vector<bool> v; int width,nl,nr,ptr;
    Mo(int n) : width((int) sqrt(n)), nl(0), nr(0), ptr(0), v(n,false) {}
    void insert(int l, int r){ left.push_back(l); right.push_back(r); }   //クエリ[l,r)
    void build(){
        order.resize(left.size()); iota(begin(order), end(order), 0);
        sort(begin(order), end(order), [&](int a, int b){
            if(left[a] / width != left[b] / width) return left[a] < left[b];
            return right[a] < right[b]; }); }
    int process(){    //クエリを１つ進め、クエリのidを返す
        if(ptr == order.size()) return (-1);    //クエリ処理が終了
        const int id = order[ptr];  //order:クエリのsort後のorder→元のorder
        while(nl > left[id]) nl--,distribute(nl); while(nr < right[id]) distribute(nr),nr++;  //add
        while(nl < left[id]) distribute(nl),nl++; while(nr > right[id]) nr--,distribute(nr);  //del
        return (order[ptr++]); }
    inline void distribute(int id){
        v[id].flip();  //現在保持しているクエリ内に入っているかいないか
        if(v[id]) add(id); else del(id); }
    void add(int id); void del(int id);
};

//idは元の配列のインデックス
void Mo::add(int id)
{
}

void Mo::del(int id)
{
}
