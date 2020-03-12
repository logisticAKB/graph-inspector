//
// Created by alexey on 08.02.2020.
//

#ifndef GRAPH_INSPECTOR_GRAPH_H
#define GRAPH_INSPECTOR_GRAPH_H

#include <string>
#include <vector>
#include <set>
#include <map>

class Graph {
public:
    Graph();
    Graph(int n);
    Graph(int n, bool flag);

    void read_graph(const std::string& file_name);
    void write_graph(const std::string& file_name);

    void add_edge(int from, int to, int weight);
    void remove_edge(int from, int to);
    int change_edge(int from, int to, int new_weight);

    void transform_to_adj_list();
    void transform_to_adj_matrix();
    void transform_to_list_of_edges();

    Graph get_spanning_tree_prima();
    Graph get_spanning_tree_kruscal();
    Graph get_spanning_tree_boruvka();

    int check_euler(bool &circle_exist);
    std::vector<int> get_eulerian_tour_fleri();
    std::vector<int> get_eulerian_tour_effective();

    int check_bipart(std::vector<char> &marks);
    std::vector<std::pair<int, int>> get_max_matching_bipart();

    Graph flow_ford_fulkerson(int source, int sink);
    Graph flow_dinitz(int source, int sink);

private:
    int n, m;
    char view;
    bool is_weighted;
    bool is_directed;

    std::vector<std::vector<int>> adj_matrix;

    std::vector<std::set<int>> unweighted_adj_list;
    std::vector<std::map<int, int>> adj_list;

    std::set<std::pair<int, int>> unweighted_list_of_edges;
    std::map<std::pair<int, int>, int> list_of_edges;

    void release_memory(char view_to_release);
    void dfs(int u, int p, int time, std::vector<char> &used, std::vector<std::set<int>> &edges, std::vector<int> &enter, std::vector<int> &ret, std::vector<std::pair<int, int>> &bridges);
    void dfs2(int v, char c, std::vector<char> &marks, int &res);
    bool dfs3(int v, std::vector<char> &used, std::vector<int> &parent);
    bool bfs(int source, int sink, std::vector<std::map<int, int>> &edges, std::vector<char> &used, std::vector<std::pair<int, int>> &parent);
    bool bfs2(int source, int sink, std::vector<std::map<int, int>> &edges, std::vector<std::set<int>> &layer_net);
    bool is_bridge(int u, int v, std::vector<std::set<int>> &edges);
};

#endif //GRAPH_INSPECTOR_GRAPH_H
