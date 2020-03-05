//
// Created by alexey on 05.03.2020.
//
#include "dsu.h"

DSU::DSU(int n) {
    parent.resize(n);
    rank.resize(n);
}

void DSU::make(int x) {
    parent[x] = x;
    rank[x] = 0;
}

int DSU::find(int x) {
    if (parent[x] == x) return x;
    return find(parent[x]);
}

void DSU::unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x != y) {
        if (rank[x] >= rank[y]) parent[y] = x;
        else parent[x] = y;
        if (rank[x] == rank[y]) rank[y]++;
    }
}
