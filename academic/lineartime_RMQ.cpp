//Harel and Tarjan 1984 Bender and Farach-Colton 2000 を参照
class RMQ
{
private:
    struct RMQNode
    {
        int id, val;    //インデックス, 値
        int par, left, right;   //親,　左の子, 右の子
    };
    std::vector<RMQNode> tree;
    std::vector<int> arr;
    std::vector<int> euler_tour, depth, diff, visit_id;
    int node_size, root, arr_len, kind;
    std::vector<int> Block_arr, diff_bit;   //各ブロックで深さが最小になるようなインデックス　各ブロックのdiffの情報をビットに詰めたもの
    std::vector<std::vector<int> > Sparse_Table;  //i番目から長さ2^kの区間に含まれるdepthのうち最小のインデックス
    std::vector<std::vector<std::vector<int> > > Table_Lookup;
    int block_size, block_rem, block_cnt, log_block_cnt;

    void make_cartesian_tree();
    void make_euler_tour(int cur_node, int& id, int cur_depth);
    void make_Block_arr();
    void make_Sparse_Table();
    void make_diff_bit();
    void make_Table_Lookup();
    std::pair<int, int> PM_RMQ(int st, int ed);
public:
    void build(std::vector<int>& arg1, int arg2);    //arg2=1のときrange_max_queryを表す
    int query(int st, int ed);
};

void RMQ::build(std::vector<int>& arg1, int arg2=0){
    node_size = (int)arg1.size();
    assert(node_size >= 2);
    arr.resize(node_size, 0);
    if(arg2){
        for(int i = 0; i < node_size; i++){
            arr[i] = -arg1[i];
        }
    }else{
        for(int i = 0; i < node_size; i++){
            arr[i] = arg1[i];
        }
    }
    kind = arg2;
    make_cartesian_tree();
    euler_tour.resize(2 * node_size - 1, -1), depth.resize(2 * node_size - 1, -1);
    diff.resize(2 * node_size - 2, -1), visit_id.resize(node_size, -1);
    int val = 0;
    make_euler_tour(root, val, 0);
    for(int i = 0;i < 2 * node_size - 2; i++){
        diff[i] = depth[i+1] - depth[i];
    }
    //±RMQの実装
    arr_len = 2 * node_size - 1;
    make_Block_arr();
    make_Sparse_Table();
    make_diff_bit();
    make_Table_Lookup();
}

void RMQ::make_cartesian_tree()
{
    tree.resize(node_size);
    for(int i = 0; i < node_size; i++){
        tree[i] = (RMQNode){i, arr[i], -1, -1, -1};
    }
    std::stack<std::pair<int, int> > st;    //インデックス,値
    st.push(std::make_pair(0,arr[0]));
    root = 0;
    for(int i = 1; i < node_size ; i++){
        int buff;
        while(1){
            //iが根となる場合
            if(st.empty()){
                st.push(std::make_pair(i, arr[i]));
                tree[i].left = buff;
                tree[buff].par = i;
                root = i;
                break;
            }
            std::pair<int, int> top = st.top();
            if(top.second <= arr[i]){
                tree[i].par = top.first; //iの親を変更
                int nd = tree[top.first].right;  //iの親の右下のノード
                //iの子となるものが存在する場合
                if(nd != -1){
                    tree[i].left = nd;  //iの親の右下のノードをiの左の子とする
                    tree[nd].par = i; //iの親の右下のノードの親をiとする
                }
                tree[top.first].right = i;  //iの親の右下の子をiとする
                st.push(std::make_pair(i,arr[i]));
                break;
            }
            buff = top.first;   //最後にpopされたものを暗記
            st.pop();
        }
    }
}

void RMQ::make_euler_tour(int cur_node, int& id, int cur_depth)
{
    visit_id[cur_node] = id;
    euler_tour[id] = cur_node;
    depth[id++] = cur_depth;
    if(tree[cur_node].left >= 0){
        make_euler_tour(tree[cur_node].left, id, cur_depth+1);
        euler_tour[id] = cur_node;
        depth[id++] = cur_depth;
    }
    if(tree[cur_node].right >= 0){
        make_euler_tour(tree[cur_node].right, id, cur_depth+1);
        euler_tour[id] = cur_node;
        depth[id++] = cur_depth;
    }
}

void RMQ::make_Block_arr()
{
    block_size = ceil(log2(arr_len)/2);
    block_cnt = (arr_len - 1) / block_size + 1;
    block_rem = arr_len % block_size;
    log_block_cnt = ceil(log2(block_cnt)) + 1;
    Block_arr.resize(block_cnt);
    for(int i = 0; i < block_cnt; i++){
        int mn = INT_MAX;
        int mn_id = -1;
        for(int j = 0; j < block_size; j++){
            int now_id = i * block_size + j;
            if(now_id >= arr_len){
                break;
            }
            if(depth[now_id] < mn){
                mn = depth[now_id];
                mn_id = now_id;
            }
        }
        Block_arr[i] = mn_id;
    }
}

void RMQ::make_Sparse_Table()
{
    Sparse_Table.resize(block_cnt, std::vector<int>(log_block_cnt, -1));
    for(int i = 0; i < block_cnt; i++){
        Sparse_Table[i][0] = Block_arr[i];
    }
    for(int j = 0; j < log_block_cnt - 1; j++){
        for(int i = 0; i < block_cnt; i++){
            if(i + (1 << j) >= block_cnt){
                Sparse_Table[i][j + 1] = Sparse_Table[i][j];
            }else{
                if(depth[Sparse_Table[i][j]] <= depth[Sparse_Table[i + (1 << j)][j]]){
                    Sparse_Table[i][j + 1] = Sparse_Table[i][j];
                }else{
                    Sparse_Table[i][j + 1] = Sparse_Table[i + (1 << j)][j];
                }
            }
        }
    }
}

void RMQ::make_diff_bit()
{
    diff_bit.resize(block_cnt, -1);
    for(int i = 0; i < block_cnt; i++){
        int num = 0;
        for(int j = 0; j < block_size - 1; j++){
            int now_id = i * block_size + j;
            if(now_id >= arr_len){
                break;
            }
            if(diff[now_id] > 0){
                num += (1 << (block_size - j - 2));
            }
        }
        diff_bit[i] = num;
    }
}

void RMQ::make_Table_Lookup()
{
    Table_Lookup.resize((1 << block_size), std::vector<std::vector<int> >(block_size + 1, std::vector<int>(block_size + 1, 0)));
    //0は減少,1は増加
    for(int i = 0; i < (1 << block_size); i++){
        std::vector<int> vec(block_size - 1, -1);
        for(int j = 0; j < block_size; j++){
            if(i & (1 << (block_size - j - 2))){
                vec[j] = 1;
            }
        }
        for(int j = 0; j < block_size; j++){
            int nw = 0, mn = 0, mn_id = j;
            Table_Lookup[i][j][j+1] = j;
            for(int k = j + 2; k <= block_size; k++){
                nw += vec[k-2];
                if(nw < mn){
                    mn = nw;
                    mn_id = k-1;
                }
                Table_Lookup[i][j][k] = mn_id;
            }
        }
    }
}

//インデックスと値のpair
std::pair<int, int> RMQ::PM_RMQ(int st, int ed)
{
    int st_block_id = (st == 0)?0:((st - 1) / block_size + 1);  //ブロック区間の開始
    int ed_block_id = ed / block_size - 1;    //ブロック区間の終了
    int st_id = st / block_size;
    int ed_id = ed / block_size;
    int st_rem = st % block_size;
    int ed_rem = ed % block_size;
    if(ed_block_id - st_block_id < 0){    //間にブロック区間がひとつもない場合
        if(st_id == ed_id){ //同じブロック区間に存在する場合
            int id_kari = block_size * st_id + Table_Lookup[diff_bit[st_id]][st_rem][ed_rem + 1];
            return std::make_pair(euler_tour[id_kari], arr[euler_tour[id_kari]]);
        }else{
            int cand1 = block_size * st_id + Table_Lookup[diff_bit[st_id]][st_rem][block_size];
            int cand2 = block_size * ed_id + Table_Lookup[diff_bit[ed_id]][0][ed_rem + 1];
            if(depth[cand1] <= depth[cand2]){
                return std::make_pair(euler_tour[cand1], arr[euler_tour[cand1]]);
            }else{
                return std::make_pair(euler_tour[cand2], arr[euler_tour[cand2]]);
            }
        }
    }else{  //間にブロックっ区間が存在する場合
        int num = floor(log2(ed_block_id - st_block_id + 1));
        int cand1 = Sparse_Table[st_block_id][num];
        int cand2 = Sparse_Table[ed_block_id - (1 << num) + 1][num];
        int c1,c2;
        if(depth[cand1] <= depth[cand2]){
            c1 = cand1;
        }else{
            c1 = cand2;
        }
        int cand3 = block_size * st_id + Table_Lookup[diff_bit[st_id]][st_rem][block_size];
        int cand4 = block_size * ed_id + Table_Lookup[diff_bit[ed_id]][0][ed_rem + 1];
        if(depth[cand3] <= depth[cand4]){
            c2 = cand3;
        }else{
            c2 = cand4;
        }
        if(depth[c1] <= depth[c2]){
            return std::make_pair(euler_tour[c1], arr[euler_tour[c1]]);
        }else{
            return std::make_pair(euler_tour[c2], arr[euler_tour[c2]]);
        }
    }
}

int RMQ::query(int st, int ed)
{
    assert(st < ed);
    std::pair<int, int> res = PM_RMQ(std::min(visit_id[st], visit_id[ed-1]), std::max(visit_id[st], visit_id[ed-1]));
    if(kind){
        return -res.second;
    }else{
        return res.second;
    }
}
