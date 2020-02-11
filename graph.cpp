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
            case 'L': { // возможно понадобится добавить обратные значения для неориентированных графов
                in_file >> is_directed >> is_weighted;
                if (is_weighted) adj_list.resize(n);
                else unweighted_adj_list.resize(n);
                int v, w;
                std::string s;
                for (int i = 0; i < n; i++) {
                    getline(in_file, s);
                    while (s.empty()) getline(in_file, s); // wtf
                    std::istringstream ss(s);
                    if (is_weighted) {
                        while (ss >> v >> w)
                            adj_list[i].insert(std::make_pair(v - 1, w));
                    } else {
                        while (ss >> v)
                            unweighted_adj_list[i].insert(v - 1);
                    }
                }
                break;
            }
            case 'E': { // возможно понадобится добавить обратные пары для неориентированных графов
                in_file >> m >> is_directed >> is_weighted;
                if (is_weighted) list_of_edges.resize(m);
                else unweighted_list_of_edges.resize(m);
                int v1 ,v2, w;
                for (int i = 0 ; i < m; i++) {
                    if (is_weighted) {
                        in_file >> v1 >> v2 >> w;
                        list_of_edges[i] = std::make_tuple(v1, v2, w);
                    } else {
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
    std::ofstream out_file(file_name);
    if (!out_file) {
        std::cerr << "Output file open error \"" << file_name << "\"" << std::endl;
        return;
    } else {
        out_file << view << ' ' << n << ' ';
        switch (view) {
            case 'C': {
                out_file << is_weighted << std::endl;
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        out_file << adj_matrix[i][j] << ' ';
                    }
                    out_file << std::endl;
                }
                break;
            }
            case 'L': {
                out_file << std::endl << is_directed << ' ' << is_weighted << std::endl;
                for  (int i = 0; i < n; i++) {
                    if (is_weighted) {
                        for (auto it : adj_list[i]) {
                            out_file << it.first + 1 << ' ' << it.second << ' ';
                        }
                    } else {
                        for (auto it : unweighted_adj_list[i]) {
                            out_file << it + 1 << ' ';
                        }
                    }
                    out_file << std::endl;
                }
                break;
            }
            case 'E': {
                out_file << m << std::endl;
                out_file << is_directed << ' ' << is_weighted << std::endl;
                for (int i = 0; i < m; i++) {
                    if (is_weighted) {
                        out_file << std::get<0>(list_of_edges[i]) << ' '
                                 << std::get<1>(list_of_edges[i]) << ' '
                                 << std::get<2>(list_of_edges[i]) << std::endl;
                    } else {
                        out_file << std::get<0>(list_of_edges[i]) << ' '
                                 << std::get<1>(list_of_edges[i]) << std::endl;
                    }
                }
                break;
            }
        }
    }
    out_file.close();
}

void Graph::transform_to_adj_list() {
    switch (view) {
        case 'C': {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {

                }
            }
            break;
        }
        case 'E': {

            break;
        }
    }
}