//
// Created by alexey on 08.02.2020.
//

#include <iostream>
#include <sstream>

#include "graph.h"

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
    gr->add_edge(4, 1, 5);
    gr->write_graph("../graph-examples/out.txt");
    delete(gr);


    return 0;
}
