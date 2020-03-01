//
// Created by alexey on 08.02.2020.
//
#include <iostream>
#include <fstream>
#include <sstream>

#include "graph.h"

Graph::Graph() = default;

void Graph::release_memory(char view_to_release) {
    switch (view_to_release) {
        case 'C': {
            adj_matrix.clear();
            adj_matrix.shrink_to_fit();
            break;
        }
        case 'L': {
            if (is_weighted) {
                adj_list.clear();
                adj_list.shrink_to_fit();
            } else {
                unweighted_adj_list.clear();
                unweighted_adj_list.shrink_to_fit();
            }
            break;
        }
        case 'E': {
            if (is_weighted) {
                list_of_edges.clear();
            } else {
                unweighted_list_of_edges.clear();
            }
            break;
        }
        default:;
    }
}

void Graph::read_graph(const std::string& file_name) {
    std::ifstream in_file(file_name);
    in_file >> view >> n;
    switch (view) {
        case 'C': { // TODO: возможно дописать счётчик m
            in_file >> is_directed >> is_weighted;
            adj_matrix.resize(n, std::vector<int>(n));
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    in_file >> adj_matrix[i][j];
                }
            }
            break;
        }
        case 'L': { // TODO: возможно дописать счётчик m
            in_file >> is_directed >> is_weighted;
            if (is_weighted) adj_list.resize(n);
            else unweighted_adj_list.resize(n);
            int v, w;
            std::string s;
            getline(in_file, s);
            for (int i = 0; i < n; i++) {
                getline(in_file, s);
                if (s.empty()) continue;
                std::istringstream ss(s);
                if (is_weighted) {
                    while (ss >> v >> w) {
                        adj_list[i].insert({v - 1, w});
                    }
                } else {
                    while (ss >> v) {
                        unweighted_adj_list[i].insert(v - 1);
                    }
                }
            }
            break;
        }
        case 'E': { // добавил обратные ребра для неор. графов.
            in_file >> m >> is_directed >> is_weighted;
            int from, to, w;
            for (int i = 0; i < m; ++i) {
                in_file >> from >> to;
                if (is_weighted) {
                    in_file >> w;
                    list_of_edges.insert({{from - 1, to - 1}, w});
                    if (!is_directed) list_of_edges.insert({{to - 1, from - 1}, w});
                } else {
                    unweighted_list_of_edges.insert({from - 1, to - 1});
                    if (!is_directed) unweighted_list_of_edges.insert({to - 1, from - 1});
                }
            }
            break;
        }
    }
    in_file.close();
}

void Graph::write_graph(const std::string& file_name) {
    std::ofstream out_file(file_name);
    out_file << view << ' ' << n << ' ';
    switch (view) {
        case 'C': {
            out_file << std::endl << is_directed << ' ' << is_weighted << std::endl;
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
        case 'E': { // обратные ребра для неор. графов не записываются в файл
            out_file << m << std::endl;
            out_file << is_directed << ' ' << is_weighted << std::endl;

            std::set<std::pair<int, int>> printed;
            if (is_weighted) {
                for (auto edge : list_of_edges) {
                    int from = edge.first.first;
                    int to = edge.first.second;
                    if (printed.find({from, to}) == printed.end()) {
                        out_file << from + 1 << ' ' << to + 1 << ' ' << edge.second << std::endl;
                        printed.insert({to, from});
                    }
                }
            } else {
                for (auto edge : unweighted_list_of_edges) {
                    int from = edge.first;
                    int to = edge.second;
                    if (printed.find({from, to}) == printed.end()) {
                        out_file << from + 1 << ' ' << to + 1 << std::endl;
                        printed.insert({to, from});
                    }
                }
            }
            break;
        }
    }
    out_file.close();
}

void Graph::add_edge(int from, int to, int weight) {
    switch (view) {
        case 'C': {
            adj_matrix[from - 1][to - 1] = is_weighted ? weight : 1;
            if (!is_directed) adj_matrix[to - 1][from - 1] = is_weighted ? weight : 1;
            break;
        }
        case 'L': {
            if (is_weighted) {
                adj_list[from - 1].insert({to - 1, weight});
                if (!is_directed) adj_list[to - 1].insert({from - 1, weight});
            } else {
                unweighted_adj_list[from - 1].insert(to - 1);
                if (!is_directed) unweighted_adj_list[to - 1].insert(from - 1);
            }
            break;
        }
        case 'E': {
            if (is_weighted) {
                list_of_edges.insert({{from - 1, to - 1}, weight});
                if (!is_directed) list_of_edges.insert({{to - 1, from - 1}, weight});
            } else {
                unweighted_list_of_edges.insert({from - 1, to - 1});
                if (!is_directed) unweighted_list_of_edges.insert({to - 1, from - 1});
            }
            break;
        }
    }
}

void Graph::remove_edge(int from, int to) {
    switch (view) {
        case 'C': {
            adj_matrix[from - 1][to - 1] = 0;
            if (!is_directed) adj_matrix[to - 1][from - 1] = 0;
        }
        case 'L': {
            if (is_weighted) {

                auto to_remove = adj_list[from - 1].find(to - 1);
                if (to_remove != adj_list[from - 1].end()) adj_list[from - 1].erase(to_remove);

                to_remove = adj_list[to - 1].find(from - 1);
                if (!is_directed && to_remove != adj_list[to - 1].end()) adj_list[to - 1].erase(to_remove);

            } else {

                auto to_remove = unweighted_adj_list[from - 1].find(to - 1);
                if (to_remove != unweighted_adj_list[from - 1].end()) unweighted_adj_list[from - 1].erase(to_remove);

                to_remove = unweighted_adj_list[to - 1].find(from - 1);
                if (!is_directed && to_remove != unweighted_adj_list[to - 1].end())
                    unweighted_adj_list[to - 1].erase(to_remove);

            }
            break;
        }
        case 'E': {
            if (is_weighted) {

                auto to_remove = list_of_edges.find({from - 1, to - 1});
                if (to_remove != list_of_edges.end()) list_of_edges.erase(to_remove);

                to_remove = list_of_edges.find({to - 1, from - 1});
                if (!is_directed && to_remove != list_of_edges.end())
                    list_of_edges.erase(to_remove);

            } else {

                auto to_remove = unweighted_list_of_edges.find({from - 1, to - 1});
                if (to_remove != unweighted_list_of_edges.end()) unweighted_list_of_edges.erase(to_remove);

                to_remove = unweighted_list_of_edges.find({to - 1, from - 1});
                if (!is_directed && to_remove != unweighted_list_of_edges.end())
                    unweighted_list_of_edges.erase(to_remove);

            }
            break;
        }
    }
}

int Graph::change_edge(int from, int to, int new_weight) { // возвращает прошлый вес ребра
    int res = -1;
    switch (view) {
        case 'C': {
            res = adj_matrix[from - 1][to - 1];
            adj_matrix[from - 1][to - 1] = new_weight;
            if (!is_directed) adj_matrix[to - 1][from - 1] = new_weight;
            break;
        }
        case 'L': {
            if (is_weighted) {
                res = adj_list[from - 1][to - 1];
                adj_list[from - 1][to - 1] = new_weight;
                if (!is_directed) adj_list[to - 1][from - 1] = new_weight;
            }
            break;
        }
        case 'E': {
            if (is_weighted) {
                res = list_of_edges[{from - 1, to - 1}];
                list_of_edges[{from - 1, to - 1}] = new_weight;
                if (!is_directed) list_of_edges[{to - 1, from - 1}] = new_weight;
            }
            break;
        }
    }
    return res;
}


void Graph::transform_to_adj_list() {
    if (is_weighted) adj_list.resize(n);
    else unweighted_adj_list.resize(n);

    switch (view) {
        case 'C': {
            view = 'L';
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (adj_matrix[i][j] != 0) {
                        if (is_weighted)
                            adj_list[i].insert({j, adj_matrix[i][j]});
                        else
                            unweighted_adj_list[i].insert(j);
                    }
                }
            }
            release_memory('C');
            break;
        }
        case 'E': {
            view = 'L';
            if (is_weighted) {
                for (auto edge : list_of_edges) {
                    int from = edge.first.first;
                    int to = edge.first.second;
                    int w = edge.second;
                    adj_list[from].insert({to, w});
                }
            } else {
                for (auto edge : unweighted_list_of_edges) {
                    int from = edge.first;
                    int to = edge.second;
                    unweighted_adj_list[from].insert(to);
                }
            }
            release_memory('E');
            break;
        }
    }
}

void Graph::transform_to_adj_matrix() {
    adj_matrix.resize(n, std::vector<int>(n, 0));

    switch (view) {
        case 'L': {
            view = 'C';
            for (int i = 0; i < n; i++) {
                if (is_weighted) {
                    for (auto it : adj_list[i]) {
                        adj_matrix[i][it.first] = it.second;
                    }
                } else {
                    for (auto it : unweighted_adj_list[i]) {
                        adj_matrix[i][it] = 1;
                    }
                }
            }
            release_memory('L');
            break;
        }
        case 'E': {
            view = 'C';
            if (is_weighted) {
                for (auto edge : list_of_edges) {
                    int from = edge.first.first;
                    int to = edge.first.second;
                    int w = edge.second;
                    adj_matrix[from][to] = w;
                }
            } else {
                for (auto edge : unweighted_list_of_edges) {
                    int from = edge.first;
                    int to = edge.second;
                    adj_matrix[from][to] = 1;
                }
            }
            release_memory('E');
            break;
        }
    }
}

void Graph::transform_to_list_of_edges() {
    switch (view) {
        case 'C': {
            view = 'E';
            m = 0;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (adj_matrix[i][j]) {
                        if (is_weighted) {
                            list_of_edges.insert({{i, j}, adj_matrix[i][j]});
                        } else {
                            unweighted_list_of_edges.insert({i, j});
                        }
                        if (i == j && !is_directed) m++;
                        m++;
                    }
                }
            }
            if (!is_directed) m /= 2; // возможно перенести эту строчку во write_graph
            release_memory('C');
            break;
        }
        case 'L': {
            view = 'E';
            for (int i = 0; i < n; i++) {
                if (is_weighted) {
                    for (auto it : adj_list[i]) {
                        list_of_edges.insert({{i, it.first}, it.second});

                        if (i == it.first && !is_directed) m++;
                        m++;
                    }
                } else {
                    for (auto it : unweighted_adj_list[i]) {
                        unweighted_list_of_edges.insert({i, it});

                        if (i == it && !is_directed) m++;
                        m++;
                    }
                }
            }
            if (!is_directed) m /= 2; // возможно перенести эту строчку во write_graph
            release_memory('L');
            break;
        }
    }
}
