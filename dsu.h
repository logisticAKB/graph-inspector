//
// Created by alexey on 05.03.2020.
//

#ifndef GRAPH_INSPECTOR_DSU_H
#define GRAPH_INSPECTOR_DSU_H

#include <vector>

class DSU {
public:
    DSU(int n);

    void make(int x);
    int find(int x);
    void unite(int x, int y);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

#endif //GRAPH_INSPECTOR_DSU_H
