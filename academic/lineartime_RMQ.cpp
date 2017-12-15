class RMQ
{
private:
    class RMQNode
    {
    public:
        int id, val;
        int par, left, right;
        RMQNode(){};
        RMQNode(int arg1, int arg2, int arg3, int arg4, int arg5) :
                id(arg1), val(arg2), par(arg3), left(arg4), right(arg5){};
    };
    RMQNode** CartesianTree;
    int *Arr, *EulerTour, *depth, *diff, *VisitId;
    int NodeSize, root, ArrLen, kind;

    int *BlockArr, *DiffBit;   //各ブロックで深さが最小になるようなインデックス　各ブロックのdiffの情報をビットに詰めたもの
    int** SparseTable;  //i番目から長さ2^kの区間に含まれるdepthのうち最小のインデックス
    int*** TableLookup;
    int BlockSize, BlockRem, BlockCnt, LogBlockCnt;
    void MakeCartesianTree();
    void MakeEulerTour(int CurNode, int& id, int CurDepth);
    void MakeBlockArr();
    void MakeSparseTable();
    void MakeDiffBit();
    void MakeTableLookup();
    pair<int,int> PM_RMQ(int st, int ed);
public:
    void build(vector<int>& arg1, int arg2);     //arg2=1のときrange_max_queryを表す
    int query(int st, int ed);
};

void RMQ::build(vector<int>& Arr, int arg2=0){  //arg2=1のときrange_max_queryを表す
    NodeSize = (int)Arr.size();
    assert(NodeSize >= 2);
    if(arg2){
        for(int i = 0; i < NodeSize; i++){
            Arr[i] = -Arr[i];
        }
    }
    kind = arg2;
    MakeCartesianTree();
    EulerTour = new int[2 * NodeSize - 1], depth = new int[2 * NodeSize - 1];
    diff = new int[2 * NodeSize - 2], VisitId = new int[NodeSize];
    int val = 0;
    MakeEulerTour(root, val, 0);
    for(int i = 0;i < 2 * NodeSize - 2; i++){
        diff[i] = depth[i+1] - depth[i];
    }
    //±RMQの実装
    ArrLen = 2 * NodeSize - 1;
    MakeBlockArr();
    MakeSparseTable();
    MakeDiffBit();
    MakeTableLookup();
    if(arg2){
        for(int i = 0; i < NodeSize; i++){
            Arr[i] = -Arr[i];
        }
    }
}

void RMQ::MakeCartesianTree()
{
    CartesianTree = new RMQNode*[NodeSize];
    for(int i = 0; i < NodeSize; i++){
        CartesianTree[i] = new RMQNode(i, Arr[i], -1, -1, -1);
    }
    stack<pair<int,int> > st;    //インデックス,値
    st.push(make_pair(0,Arr[0]));
    root = 0;
    for(int i = 1; i < NodeSize ; i++){
        int buff;
        while(1){
            //iが根となる場合
            if(st.empty()){
                st.push(make_pair(i, Arr[i]));
                CartesianTree[i]->left = buff;
                CartesianTree[buff]->par = i;
                root = i;
                break;
            }
            pair<int, int> top = st.top();
            if(top.second <= Arr[i]){
                CartesianTree[i]->par = top.first; //iの親を変更
                int nd = CartesianTree[top.first]->right;  //iの親の右下のノード
                //iの子となるものが存在する場合
                if(nd != -1){
                    CartesianTree[i]->left = nd;  //iの親の右下のノードをiの左の子とする
                    CartesianTree[nd]->par = i; //iの親の右下のノードの親をiとする
                }
                CartesianTree[top.first]->right = i;  //iの親の右下の子をiとする
                st.push(make_pair(i,Arr[i]));
                break;
            }
            buff = top.first;   //最後にpopされたものを暗記
            st.pop();
        }
    }
}

void RMQ::MakeEulerTour(int CurNode, int& id, int CurDepth)
{
    VisitId[CurNode] = id;
    EulerTour[id] = CurNode;
    depth[id++] = CurDepth;
    if(CartesianTree[CurNode]->left >= 0){
        MakeEulerTour(CartesianTree[CurNode]->left, id, CurDepth+1);
        EulerTour[id] = CurNode;
        depth[id++] = CurDepth;
    }
    if(CartesianTree[CurNode]->right >= 0){
        MakeEulerTour(CartesianTree[CurNode]->right, id, CurDepth+1);
        EulerTour[id] = CurNode;
        depth[id++] = CurDepth;
    }
}

void RMQ::MakeBlockArr()
{
    BlockSize = ceil(log2(ArrLen)/2);
    BlockCnt = (ArrLen - 1) / BlockSize + 1;
    BlockRem = ArrLen % BlockSize;
    LogBlockCnt = ceil(log2(BlockCnt)) + 1;
    BlockArr = new int[BlockCnt];
    for(int i = 0; i < BlockCnt; i++){
        int mn = INT_MAX;
        int mn_id = -1;
        for(int j = 0; j < BlockSize; j++){
            int now_id = i * BlockSize + j;
            if(now_id >= ArrLen){
                break;
            }
            if(depth[now_id] < mn){
                mn = depth[now_id];
                mn_id = now_id;
            }
        }
        BlockArr[i] = mn_id;
    }
}

void RMQ::MakeSparseTable()
{
    SparseTable = new int*[BlockCnt];
    for(int i = 0; i < BlockCnt; i++){
        SparseTable[i] = new int[LogBlockCnt];
    }
    for(int i = 0; i < BlockCnt; i++){
        SparseTable[i][0] = BlockArr[i];
    }
    for(int j = 0; j < LogBlockCnt - 1; j++){
        for(int i = 0; i < BlockCnt; i++){
            if(i + (1 << j) >= BlockCnt){
                SparseTable[i][j + 1] = SparseTable[i][j];
            }else{
                if(depth[SparseTable[i][j]] <= depth[SparseTable[i + (1 << j)][j]]){
                    SparseTable[i][j + 1] = SparseTable[i][j];
                }else{
                    SparseTable[i][j + 1] = SparseTable[i + (1 << j)][j];
                }
            }
        }
    }
}

void RMQ::MakeDiffBit()
{
    DiffBit = new int[BlockCnt];
    for(int i = 0; i < BlockCnt; i++){
        int num = 0;
        for(int j = 0; j < BlockSize - 1; j++){
            int now_id = i * BlockSize + j;
            if(now_id >= ArrLen){
                break;
            }
            if(diff[now_id] > 0){
                num += (1 << (BlockSize - j - 2));
            }
        }
        DiffBit[i] = num;
    }
}

void RMQ::MakeTableLookup()
{
    TableLookup = new int**[(1 << BlockSize)];
    for(int i = 0; i < (1 << BlockSize); i++){
        TableLookup[i] = new int*[BlockSize + 1];
        for(int j = 0; j < BlockSize + 1; j++){
            TableLookup[i][j] = new int[BlockSize + 1];
        }
    }
    //0は減少,1は増加
    for(int i = 0; i < (1 << BlockSize); i++){
        int* arr = new int[BlockSize - 1];
        for(int j = 0; j < BlockSize; j++){
            if(i & (1 << (BlockSize - j - 2))){
                arr[j] = 1;
            }
        }
        for(int j = 0; j < BlockSize; j++){
            int nw = 0, mn = 0, mn_id = j;
            TableLookup[i][j][j+1] = j;
            for(int k = j + 2; k <= BlockSize; k++){
                nw += arr[k-2];
                if(nw < mn){
                    mn = nw;
                    mn_id = k-1;
                }
                TableLookup[i][j][k] = mn_id;
            }
        }
    }
}

//インデックスと値のpair
pair<int, int> RMQ::PM_RMQ(int st, int ed)
{
    int st_block_id = (st == 0)?0:((st - 1) / BlockSize + 1);  //ブロック区間の開始
    int ed_block_id = ed / BlockSize - 1;    //ブロック区間の終了
    int st_id = st / BlockSize;
    int ed_id = ed / BlockSize;
    int st_rem = st % BlockSize;
    int ed_rem = ed % BlockSize;
    int st_val = BlockArr[st_id];
    int ed_val = BlockArr[ed_id];
    int res, mn = INT_MIN;
    if(ed_block_id - st_block_id < 0){    //間にブロック区間がひとつもない場合
        if(st_id == ed_id){ //同じブロック区間に存在する場合
            int id_kari = BlockSize * st_id + TableLookup[DiffBit[st_id]][st_rem][ed_rem + 1];
            return make_pair(EulerTour[id_kari], Arr[EulerTour[id_kari]]);
        }else{
            int cand1 = BlockSize * st_id + TableLookup[DiffBit[st_id]][st_rem][BlockSize];
            int cand2 = BlockSize * ed_id + TableLookup[DiffBit[ed_id]][0][ed_rem + 1];
            if(depth[cand1] <= depth[cand2]){
                return make_pair(EulerTour[cand1], Arr[EulerTour[cand1]]);
            }else{
                return make_pair(EulerTour[cand2], Arr[EulerTour[cand2]]);
            }
        }
    }else{  //間にブロックっ区間が存在する場合
        int num = floor(log2(ed_block_id - st_block_id + 1));
        int cand1 = SparseTable[st_block_id][num];
        int cand2 = SparseTable[ed_block_id-(1 << num) + 1][num];
        int c1,c2;
        if(depth[cand1] <= depth[cand2]){
            c1 = cand1;
        }else{
            c1 = cand2;
        }
        int cand3 = BlockSize * st_id + TableLookup[DiffBit[st_id]][st_rem][BlockSize];
        int cand4 = BlockSize * ed_id + TableLookup[DiffBit[ed_id]][0][ed_rem + 1];
        if(depth[cand3] <= depth[cand4]){
            c2 = cand3;
        }else{
            c2 = cand4;
        }
        if(depth[c1] <= depth[c2]){
            return make_pair(EulerTour[c1], Arr[EulerTour[c1]]);
        }else{
            return make_pair(EulerTour[c2], Arr[EulerTour[c2]]);
        }
    }
}

//範囲は[st,ed)
int RMQ::query(int st, int ed)
{
    assert(st < ed);
    pair<int, int> res = PM_RMQ(min(VisitId[st], VisitId[ed-1]), max(VisitId[st], VisitId[ed-1]));
    if(kind){
        return -res.second;
    }else{
        return res.second;
    }
}
