//
// Created by alexey on 08.02.2020.
//

#include <iostream>
#include <sstream>

#include "graph.h"
#include "dsu.h"

using namespace std;

int main() {
    //std::cout << "Hello World!" << std::endl;

    //auto *gr = new Graph();
    // gr->read_graph("fd");
    //vector<vector<int>> a;
    //a.resize(4, vector<int>(4, 5));
    //a.clear();
    //a.shrink_to_fit();
    //a = {};
    //cout << a.capacity() << endl;

    auto *gr = new Graph();
    gr->read_graph("../graph-examples/in.txt");
//    Graph g = gr->get_spanning_tree_boruvka();
//    g.write_graph("../graph-examples/out.txt");
    bool circle_exist;
    cout << gr->check_euler(circle_exist) << endl;
    cout << "has cycle: " << circle_exist << endl;
    vector<int> v = gr->get_eulerian_tour_fleri();
    for (auto i : v) cout << i << ' ';
    delete(gr);


    return 0;
}
