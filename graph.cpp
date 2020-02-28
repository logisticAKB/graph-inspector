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
//                        if (!is_directed) adj_list[v - 1].insert({i, w});
                    }
                } else {
                    while (ss >> v) {
                        unweighted_adj_list[i].insert(v - 1);
//                        if (!is_directed) unweighted_adj_list[v - 1].insert(i);
                    }
                }
            }
            break;
        }
        /*case 'E': { // добавил обратные ребра для неор. графов.
            in_file >> m >> is_directed >> is_weighted;
            if (!is_directed) m *= 2;
            if (is_weighted) list_of_edges.resize(m);
            else unweighted_list_of_edges.resize(m);
            int v1 ,v2, w;
            for (int i = 0; i < m; !is_directed ? i += 2 : i++) {
                if (is_weighted) {
                    in_file >> v1 >> v2 >> w;
                    list_of_edges[i] = std::make_tuple(v1, v2, w);
                    if (!is_directed) list_of_edges[i + 1] = std::make_tuple(v2, v1, w);
                } else {
                    in_file >> v1 >> v2;
                    unweighted_list_of_edges[i] = std::make_pair(v1, v2);
                    if (!is_directed) unweighted_list_of_edges[i + 1] = std::make_pair(v2, v1);
                }
            }
            break;
        }*/
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
        case 'L': { // TODO: удалить дубли для неор. графов
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
        /*case 'E': { // поправить 2е ребра
            out_file << m << std::endl;
            out_file << is_directed << ' ' << is_weighted << std::endl;
            for (int i = 0; i < m; i++) {
                if (is_weighted) {
                    out_file << std::get<0>(list_of_edges[i]) << ' '
                             << std::get<1>(list_of_edges[i]) << ' '
                             << std::get<2>(list_of_edges[i]) << std::endl;
                } else {
                    out_file << unweighted_list_of_edges[i].first << ' '
                             << unweighted_list_of_edges[i].second << std::endl;
                }
            }
            break;
        }*/
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
            
            break;
        }
        case 'E': {

            break;
        }
    }
}

int Graph::change_edge(int from, int to, int new_weight) { // вернуть старое
    int res = -1;
    switch (view) {
        case 'C': {
            res = adj_matrix[from - 1][to - 1];
            adj_matrix[from - 1][to - 1] = new_weight;
            if (!is_directed) adj_matrix[to - 1][from - 1] = new_weight;
            break;
        }
        case 'L': {

            break;
        }
        case 'E': {

            break;
        }
    }
    return res;
}

/*
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
                            adj_list[i].insert(std::make_pair(j, adj_matrix[i][j]));
                        else
                            unweighted_adj_list[i].insert(j);
                    }
                }
            }
            release_memory('C');
            break;
        }
        case 'E': { // хммм, походу работает / надо поделить на m
            view = 'L';
            for (int i = 0; i < m; i++) {
                if (is_weighted) {
                    adj_list[std::get<0>(list_of_edges[i]) - 1].insert(
                            std::make_pair(std::get<1>(list_of_edges[i]) - 1,
                                    std::get<2>(list_of_edges[i])));
                } else {
                    unweighted_adj_list[unweighted_list_of_edges[i].first - 1].insert(
                            unweighted_list_of_edges[i].second - 1);
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
        case 'E': { // хммм, походу работает / почему??? / надо поделить на m
            view = 'C';
            for (int i = 0; i < m; i++) {
                if (is_weighted) {
                    adj_matrix[std::get<0>(list_of_edges[i]) - 1][std::get<1>(list_of_edges[i]) - 1] =
                            std::get<2>(list_of_edges[i]);
                } else {
                    adj_matrix[unweighted_list_of_edges[i].first - 1][unweighted_list_of_edges[i].second - 1] = 1;
                }
            }
            release_memory('E');
            break;
        }
    }
}

void Graph::transform_to_list_of_edges() {
    if (!is_directed) m *= 2;
    if (is_weighted) list_of_edges.resize(m);
    else unweighted_list_of_edges.resize(m);

    switch (view) {
        case 'C': {
            view = 'E';
            int k = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (adj_matrix[i][j] != 0) {
                        if (is_weighted) {
                            list_of_edges[k] = std::make_tuple(i + 1, j + 1, adj_matrix[i][j]);
                            if (!is_directed) list_of_edges[k + 1] = std::make_tuple(j + 1, i + 1, adj_matrix[i][j]);
                        }
                        else {
                            unweighted_list_of_edges[k] = std::make_pair(i + 1, j + 1);
                            if (!is_directed) unweighted_list_of_edges[k + 1] = std::make_pair(j + 1, i + 1);
                        }
                        k += 2;
                    }
                }
            }
            release_memory('C');
            break;
        }
        case 'L': {
            view = 'E';
            for (int i = 0; i < n; i++) {
                if (is_weighted) {
                    for (auto it : adj_list[i]) {
                        // туда
                        // сюда
                    }
                } else {
                    for (auto it : unweighted_adj_list[i]) {

                    }
                }
            }

            release_memory('L');
            break;
        }
    }
}
*/