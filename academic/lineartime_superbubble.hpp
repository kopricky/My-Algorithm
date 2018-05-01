// Brankovic et al. 2015 を参照
#include "header.hpp"
#include "lineartime_RMQ.hpp"
// #define rep(i,n) for(int i=0;i<(int)n;++i)
// #define show(x) std::cout<<#x<<" = "<<(x)<<std::endl
// #define spair(p) std::cout<<#p<<": "<<p.first<<" "<<p.second<<std::endl
// #define svec(v) std::cout<<#v<<":";for(int kbrni=0;kbrni<(int)v.size();kbrni++)std::cout<<" "<<v[kbrni];std::cout<<std::endl

void graph_input(int edge_size, std::vector<std::vector<int> >& graph, std::vector<std::vector<int> >& rgraph)
{
    for(int i = 0; i < edge_size; i++){
        int arg1, arg2;
        scanf("%d%d", &arg1, &arg2);
        graph[arg1].push_back(arg2);
        rgraph[arg2].push_back(arg1);
    }
}

void recursive_topological_sort(int u, int& index, std::vector<int>& visit, std::vector<int>& ord,
     std::vector<int>& ordD, std::vector<std::vector<int> >& graph)
{
    if(visit[u] == 1){
        return;
    }
    visit[u] = 0;
	for(int nx : graph[u]){
        recursive_topological_sort(nx, index, visit, ord, ordD, graph);
	}
    visit[u] = 1;
    ord[index] = u;
	ordD[u] = index--;
}

void topological_sort(int node_size, std::vector<int>& ord, std::vector<int>& ordD, std::vector<std::vector<int> >& graph)
{
    //与えられたグラフがconnected かつ DAGであることを仮定
    std::vector<int> visit(node_size, -1);
    int index = node_size - 1;
    for(int i = 0; i < node_size; i++){
        if(visit[i] == -1){
            recursive_topological_sort(i, index, visit, ord, ordD, graph);
        }
    }
}

void prepare_superbubble(int node_size, std::vector<bool>& ent, std::vector<bool>& exi,std::vector<int>& ordD, std::vector<int>& outparent,
     std::vector<int>& outchild, std::vector<std::vector<int> >& graph, std::vector<std::vector<int> >& rgraph)
{
    for(int i = 0; i < node_size; i++){
        bool ok = false;
        for(int j = 0; j < (int)rgraph[i].size(); j++){
            if((int)graph[rgraph[i][j]].size() == 1){
                ok = true;
                break;
            }
        }
        exi[i] = ok;
        ok = false;
        for(int j = 0; j < (int)graph[i].size(); j++){
            if((int)rgraph[graph[i][j]].size() == 1){
                ok = true;
                break;
            }
        }
        ent[i] = ok;
    }
    for(int i = 0; i < node_size; i++){
        int mn_ordD = node_size;
        for(int j = 0; j < (int)rgraph[i].size(); j++){
            mn_ordD = std::min(mn_ordD, ordD[rgraph[i][j]]);
        }
        outparent[ordD[i]] = mn_ordD;
        int mx_ordD = -1;
        for(int j = 0; j < (int)graph[i].size(); j++){
            mx_ordD = std::max(mx_ordD, ordD[graph[i][j]]);
        }
        outchild[ordD[i]] = mx_ordD;
    }
}

int validate_superbubble(int st_node, int ed_node, std::vector<bool>& ent, std::vector<int>& ord, std::vector<int>& ordD,
     std::vector<int>& prev_ent, RMQ<int>& parent_rmq, RMQ<int>& child_rmq)
{
    int st_id = ordD[st_node];
    int ed_id = ordD[ed_node];
    int mx_child = child_rmq.query(st_id, ed_id);
    int mn_parent = parent_rmq.query(st_id + 1, ed_id + 1);
    if(mx_child != ed_id){
        return -1;
    }
    if(mn_parent == st_id){
        return st_node;
    }else if(ent[ord[mn_parent]]){
        return ord[mn_parent];
    }else{
        return prev_ent[ord[mn_parent]];
    }
}

void report_superbubble(std::list<std::pair<int, int> >& cand, int st_node, int ed_node, std::vector<bool>& ent, std::vector<bool>& exi,
     std::vector<int>& ord, std::vector<int>& ordD, std::vector<int>& prev_ent, std::vector<int>& next_ent, std::vector<int>& alt_ent,
      RMQ<int>& parent_rmq, RMQ<int>& child_rmq, std::vector<std::pair<int, int> >& superbubble_set)
     //ex_nodeで終わるsuperbubbleを存在すれば報告する
{
    if(st_node < 0 || ed_node < 0 || ordD[st_node] >= ordD[ed_node]){
        cand.pop_back();
        return;
    }
    int prevent_node = prev_ent[ed_node];
    int valid = -1;
    while(ordD[prevent_node] >= ordD[st_node]){
        valid = validate_superbubble(prevent_node, ed_node, ent, ord, ordD, prev_ent, parent_rmq, child_rmq);
        if(valid == prevent_node || valid == alt_ent[prevent_node] || valid == -1){
            break;
        }
        alt_ent[prevent_node] = valid;
        prevent_node = valid;
    }
    cand.pop_back();
    if(valid == prevent_node){
        superbubble_set.push_back(std::make_pair(prevent_node, ed_node));
        while((cand.back()).first != prevent_node){
            if((cand.back()).second == 1){
                //nestされたsuperbubbleについて調べる
                report_superbubble(cand, next_ent[prevent_node], (cand.back()).first, ent, exi, ord, ordD, prev_ent, next_ent, alt_ent,
                 parent_rmq, child_rmq, superbubble_set);
            }else{
                cand.pop_back();
            }
        }
    }
}

void superbubble(int node_size, std::vector<std::vector<int> >& graph, std::vector<std::vector<int> >& rgraph,
 std::vector<std::pair<int, int> >& superbubble_set)
{
    std::vector<int> ord(node_size, -1);
    std::vector<int> ordD(node_size, -1);
    topological_sort(node_size, ord, ordD, graph);
    std::vector<bool> ent(node_size, false), exi(node_size, false);
    std::vector<int> outparent(node_size, -1), outchild(node_size, -1);
    std::vector<int> alt_ent(node_size, -1), prev_ent(node_size, -1), next_node(node_size, -1);
    prepare_superbubble(node_size, ent, exi, ordD, outparent, outchild, graph, rgraph);
    std::list<std::pair<int, int> > cand;   //(頂点の値,entかexiのフラグ)
    std::vector<int> next_ent(node_size, -1);
    RMQ<int> parent_rmq, child_rmq;
    parent_rmq.build(outparent, 0), child_rmq.build(outchild, 1);
    int prev_entrance = -1;
    int next_entrance = -1;
    for(int ver : ord){
        alt_ent[ver] = -1;
        prev_ent[ver] = prev_entrance;
        if(exi[ver]){
            cand.push_back(std::make_pair(ver, 1));
        }
        if(ent[ver]){
            cand.push_back(std::make_pair(ver, 0));
            prev_entrance = ver;
            if(next_entrance >= 0){
                next_ent[next_entrance] = ver;
            }
            next_entrance = ver;
        }
    }
    while(!cand.empty()){
        if((cand.back()).second == 0){
            cand.pop_back();
        }else{
            report_superbubble(cand, (cand.front()).first, (cand.back()).first, ent, exi, ord, ordD, prev_ent, next_ent, alt_ent,
             parent_rmq, child_rmq, superbubble_set);
        }
    }
}

int main()
{
    int node_size, edge_size;
	scanf("%d%d", &node_size, &edge_size);
    std::vector<std::vector<int> > graph(node_size);
    std::vector<std::vector<int> > rgraph(node_size);
    graph_input(edge_size, graph, rgraph);
    std::vector<std::pair<int, int> > superbubble_set;
    superbubble(node_size, graph, rgraph, superbubble_set);
    for(int i = 0; i < (int)superbubble_set.size(); i++){
        printf("%d %d\n",superbubble_set[i].first, superbubble_set[i].second);
    }
}
