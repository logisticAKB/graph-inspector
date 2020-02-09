//
// Created by alexey on 08.02.2020.
//
#include <iostream>
#include <fstream>
#include <sstream>

#include "graph.h"

Graph::Graph() = default;

void Graph::read_graph(const std::string& file_name) {
    std::ifstream in_file(file_name);
    if (!in_file) {
        std::cerr << "Input file open error \"" << file_name << "\"" << std::endl;
        return;
    } else {
        in_file >> view >> n;
        switch (view) {
            case 'C': {
                m = 0;
                in_file >> is_weighted;
                adj_matrix.resize(n, std::vector<int>(n));
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        in_file >> adj_matrix[i][j];
                        if (adj_matrix[i][j]) m++;
                    }
                }
                break;
            }
            case 'L': {
                in_file >> is_directed >> is_weighted;
                if (is_weighted) {
                    adj_list.resize(n);
                    int v, w;
                    for (int i = 0; i < n; i++) {
                        std::string s;
                        getline(in_file, s);
                        std::istringstream ss(s);
                        while (ss >> v >> w) {
                            adj_list[i].insert(std::make_pair(v - 1, w));
                            if (!is_directed)
                                adj_list[v - 1].insert(std::make_pair(i, w));
                        }
                    }
                } else {
                    unweighted_adj_list.resize(n);
                    int v;
                    for (int i = 0; i < n; i++) {
                        std::string s;
                        getline(in_file, s);
                        std::istringstream ss(s);
                        while (ss >> v) {
                            unweighted_adj_list[i].insert(v - 1);
                            if (!is_directed)
                                unweighted_adj_list[i].insert(i);
                        }
                    }
                }
                break;
            }
            case 'E': { // возможно понадобится добавить обратные пары для неориентированных графов
                in_file >> m >> is_directed >> is_weighted;
                if (is_weighted) {
                    list_of_edges.resize(m);
                    int v1 ,v2, w;
                    for (int i = 0 ; i < m; i++) {
                        in_file >> v1 >> v2 >> w;
                        list_of_edges[i] = std::make_tuple(v1, v2, w);
                    }
                } else {
                    unweighted_list_of_edges.resize(m);
                    int v1 ,v2;
                    for (int i = 0 ; i < m; i++) {
                        in_file >> v1 >> v2;
                        unweighted_list_of_edges[i] = std::make_pair(v1, v2);
                    }
                }
                break;
            }
        }
    }
    in_file.close();
}

void Graph::write_graph(const std::string& file_name) {
    
}