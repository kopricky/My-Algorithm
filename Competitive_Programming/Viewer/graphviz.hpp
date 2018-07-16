#include "../header.hpp"
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/graphviz.hpp>

//dot -Tpng preview.dot -o preview.png

//入力に対して
void view_graph()
{
    int node_size, edge_size;
    cin >> node_size >> edge_size;
    vector<string> name_vec(node_size);
    for(int i = 0; i < node_size; i++){
        stringstream ss;
        ss << i;
        name_vec[i] = ss.str();
    }
    vector<pair<int, int> > edge(edge_size);
    for(int i = 0; i < edge_size; i++){
            cin >> edge[i].first >> edge[i].second;
    }
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> g(edge.begin(), edge.end(), node_size);
    ofstream file("preview.dot");
    boost::write_graphviz(file, g, boost::make_label_writer(&name_vec[0]));
}

// 無向グラフGに対して
void view_graph(vvi& G)
{
    int node_size = len(G);
    vector<string> name_vec(node_size);
    for(int i = 0; i < node_size; i++){
        stringstream ss;
        ss << i;
        name_vec[i] = ss.str();
    }
    vector<pair<int, int> > edge;
    for(int i = 0; i < node_size; i++){
        for(int w : G[i]){
            if(w >= i){
                edge.pb(P(i,w));
            }
        }
    }
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> g(edge.begin(), edge.end(), node_size);
    ofstream file("preview.dot");
    boost::write_graphviz(file, g, boost::make_label_writer(&name_vec[0]));
}

//有向グラフGに対して
void view_graph(vvi& G)
{
    int node_size = len(G);
    vector<string> name_vec(node_size);
    for(int i = 0; i < node_size; i++){
        stringstream ss;
        ss << i;
        name_vec[i] = ss.str();
    }
    vector<pair<int, int> > edge;
    for(int i = 0; i < node_size; i++){
        for(int w : G[i]){
            edge.pb(P(i,w));
        }
    }
    boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> g(edge.begin(), edge.end(), node_size);
    ofstream file("preview.dot");
    boost::write_graphviz(file, g, boost::make_label_writer(&name_vec[0]));
}
