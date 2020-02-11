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

    void read_graph(const std::string& file_name);
    void write_graph(const std::string& file_name);

    void add_edge(int from, int to, int weight);
    void remove_edge(int from, int to);
    int change_edge(int from, int to, int new_weight);

    void transform_to_adj_list();
    void transform_to_adj_matrix();
    void transform_to_list_of_edges();

private:
    int n, m;
    char view;
    bool is_weighted;
    bool is_directed;

    std::vector<std::vector<int>> adj_matrix;

    std::vector<std::set<int>> unweighted_adj_list;
    std::vector<std::map<int, int>> adj_list;

    std::vector<std::pair<int, int>> unweighted_list_of_edges;
    std::vector<std::tuple<int, int, int>> list_of_edges;

    void release_memory(char view_to_release);
};

#endif //GRAPH_INSPECTOR_GRAPH_H
