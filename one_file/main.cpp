#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <algorithm>
#include <stack>
#include <queue>
#include <string>
#include <vector>
#include <set>
#include <map>

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

DSU::DSU(int n) {
    parent.resize(n);
    rank.resize(n);
    for (int i = 0; i < n; ++i) make(i);
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

class Graph {
public:
    Graph() {};
    Graph(int n);
    Graph(int n, bool flag);

    void readGraph(const std::string& file_name);
    void writeGraph(const std::string& file_name);

    void addEdge(int from, int to, int weight);
    void removeEdge(int from, int to);
    int changeEdge(int from, int to, int new_weight);

    void transformToAdjList();
    void transformToAdjMatrix();
    void transformToListOfEdges();

    Graph getSpaingTreePrima();
    Graph getSpaingTreeKruscal();
    Graph getSpaingTreeBoruvka();

    int checkEuler(bool &circle_exist);
    std::vector<int> getEuleranTourFleri();
    std::vector<int> getEuleranTourEffective();

    int checkBipart(std::vector<char> &marks);
    std::vector<std::pair<int, int>> getMaximumMatchingBipart();

    Graph flowFordFulkerson(int source, int sink);
    Graph flowDinitz(int source, int sink);

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

Graph::Graph(int n) {
    Graph::n = n;
    view = 'L';
    is_weighted = true;
    is_directed = false;

    adj_list.resize(n);
}

Graph::Graph(int n, bool flag) {
    Graph::n = n;
    view = 'L';
    is_weighted = true;
    is_directed = true;

    adj_list.resize(n);
}

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

void Graph::dfs(int u, int p, int time, std::vector<char> &used, std::vector<std::set<int>> &edges, std::vector<int> &enter, std::vector<int> &ret, std::vector<std::pair<int, int>> &bridges) {
    used[u] = true;
    enter[u] = ret[u] = time++;
    for (auto v : edges[u]) {
        if (v == p) continue;
        if (used[v])
            ret[u] = std::min(ret[u], enter[v]);
        else {
            dfs(v, u, time, used, edges, enter, ret, bridges);
            ret[u] = std::min(ret[u], ret[v]);
            if (ret[v] > enter[u]) bridges.emplace_back(u, v);
        }
    }
}

void Graph::dfs2(int v, char c, std::vector<char> &marks, int &res) {
    marks[v] = c;
    for (int u : unweighted_adj_list[v]) {
        if (marks[u] == ' ') dfs2(u, c == 'A' ? 'B' : 'A', marks, res);
        else if (marks[u] == c) res = 0;
    }
}

bool Graph::dfs3(int v, std::vector<char> &used, std::vector<int> &parent) {
    if (used[v]) return false;
    used[v] = true;
    for (int u : unweighted_adj_list[v]) {
        if (parent[u] == -1 || dfs3(parent[u], used, parent)) {
            parent[u] = v;
            return true;
        }
    }
    return false;
}

bool Graph::bfs(int source, int sink, std::vector<std::map<int, int>> &edges, std::vector<char> &used, std::vector<std::pair<int, int>> &parent) {
    std::queue<int> q;
    q.push(source);
    used[source] = true;
    parent[source].first = -1;
    parent[source].second = INT_MAX;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        if (v == sink) return true;
        for (auto edge : edges[v]) {
            int u = edge.first;
            int w = edge.second;
            if (!used[u] && w != 0) {
                used[u] = true;
                q.push(u);
                parent[u].first = v;
                parent[u].second = std::min(parent[v].second, w);
            }
        }
    }
    return false;
}

bool Graph::bfs2(int source, int sink, std::vector<std::map<int, int>> &edges, std::vector<std::set<int>> &layer_net) {
    std::queue<int> q;
    std::vector<int> layer(n, -1);
    q.push(source);
    layer[source] = 0;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (auto edge : edges[v]) {
            int u = edge.first;
            int w = edge.second;
            if ((layer[u] == -1 || layer[u] > layer[v]) && w != 0) {
                layer[u] = layer[v] + 1;
                q.push(u);
                layer_net[v].insert(u);
            }
        }
    }
    return layer[sink] != -1;
}

bool Graph::is_bridge(int u, int v, std::vector<std::set<int>> &edges) {
    std::vector<int> enter(n), ret(n);
    std::vector<char> used(n); // FIXME: assign(false)
    std::vector<std::pair<int, int>> bridges;
    dfs(u, -1, 0, used, edges, enter, ret, bridges);
    for (auto bridge : bridges)  {
        if (bridge == std::make_pair(u, v)) return true;
    }
    return false;
}

void Graph::readGraph(const std::string& file_name) {
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

void Graph::writeGraph(const std::string& file_name) {
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

void Graph::addEdge(int from, int to, int weight) {
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

void Graph::removeEdge(int from, int to) {
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

int Graph::changeEdge(int from, int to, int new_weight) { // возвращает прошлый вес ребра
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


void Graph::transformToAdjList() {
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

void Graph::transformToAdjMatrix() {
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

void Graph::transformToListOfEdges() {
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
            if (!is_directed) m /= 2; // возможно перенести эту строчку во writeGraph
            release_memory('C');
            break;
        }
        case 'L': {
            view = 'E';
            m = 0;
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
            if (!is_directed) m /= 2; // возможно перенести эту строчку во writeGraph
            release_memory('L');
            break;
        }
    }
}

Graph Graph::getSpaingTreePrima() {
    transformToAdjList();

    std::vector<std::pair<int, int>> min_weight(n, {INT_MAX, -1});
    std::set<std::pair<int, int>> q;
    Graph spanning_tree(n);

    min_weight[0].first = 0;
    q.insert({0, 0});
    for (int i = 0; i < n; ++i) {
        int v = q.begin()->second;
        q.erase(q.begin());

        if (min_weight[v].second != -1)
            spanning_tree.addEdge(v + 1, min_weight[v].second + 1, min_weight[v].first);

        for (auto edge : adj_list[v]) {
            int u = edge.first;
            int w = edge.second;
            if (w < min_weight[u].first) {
                q.erase({min_weight[u].first, u});
                min_weight[u].first = w;
                min_weight[u].second = v;
                q.insert({w, u});
            }
        }
    }
    return spanning_tree;
}

Graph Graph::getSpaingTreeKruscal() {
    transformToAdjList();

    Graph spanning_tree(n);
    DSU dsu(n);
    std::vector<std::pair<int, int>> cur_edges;
    for (int i = 0; i < n; ++i) {

        cur_edges.clear();
        cur_edges.reserve(adj_list[i].size());
        for (auto edge : adj_list[i]) {
            cur_edges.emplace_back(edge.second, edge.first);
        }
        sort(cur_edges.begin(), cur_edges.end());

        for (auto edge : cur_edges) {
            if (dsu.find(edge.second) != dsu.find(i)) {
                spanning_tree.addEdge(i + 1, edge.second + 1, edge.first);
                dsu.unite(i, edge.second);
            }
        }
    }

    return spanning_tree;
}

Graph Graph::getSpaingTreeBoruvka() {
    transformToAdjList();

    Graph spanning_tree(n);
    DSU dsu(n);
    int edges_in_tree = 0;
    while (edges_in_tree < n - 1) {
        for (int i = 0; i < n; ++i) {
            std::pair<std::pair<int, int>, int> min_edge = {{-1, -1}, INT_MAX};
            for (auto edge : adj_list[i]) {
                int u = i;
                int v = edge.first;
                int w = edge.second;
//                if (spanning_tree.adj_list[v].find(u) != spanning_tree.adj_list[v].end()) break;
                if (dsu.find(u) != dsu.find(v)) {
                    if (w < min_edge.second) {
                        min_edge = {{u, v}, w};
                    }
                } else break;
            }
            int u = min_edge.first.first;
            int v = min_edge.first.second;
            int w = min_edge.second;
            if (u != -1 && v != -1) {
                dsu.unite(u, v);
                spanning_tree.addEdge(u + 1, v + 1, w);
                edges_in_tree++;
            }
        }
    }

    return spanning_tree;
}

int Graph::checkEuler(bool &circle_exist) {
    transformToAdjList();

    int k_odd_vert = 0, res = 0;
    for (int i = 0; i < n; ++i) {
        if (unweighted_adj_list[i].size() & 1) {
            k_odd_vert++;
            res = i + 1;
        }
    }
    if (k_odd_vert <= 2) {
        DSU dsu(n);
        for (int i = 0; i < n; ++i) {
            for (auto edge : unweighted_adj_list[i]) dsu.unite(i, edge);
        }
        std::map<int, int> comp;
        for (int i = 0; i < n; ++i) comp[dsu.find(i)]++;
        bool is_ok = true;
        for (auto it : comp) {
            if (it.second > 1) {
                if (is_ok) {
                    if (!res) res = it.first + 1;
                    is_ok = false;
                }
                else {
                    res = 0;
                    break;
                }
            }
        }
    }
    circle_exist = (k_odd_vert == 0) && res;
    return res;
}

std::vector<int> Graph::getEuleranTourFleri() {
    transformToAdjList();

    std::vector<std::set<int>> edges(unweighted_adj_list);
    std::vector<int> tour;
    tour.reserve(n);

    bool has_cycle;
    int u = checkEuler(has_cycle) - 1;
    tour.push_back(u + 1);
    while (!edges[u].empty()) {
        int to = -1;
        for (int v : edges[u]) {
            to = v;
            if (!is_bridge(u, v, edges)) break;
        }
        tour.push_back(to + 1);
        edges[u].erase(to);
        edges[to].erase(u);
        u = to;
    }

    return tour;
}

std::vector<int> Graph::getEuleranTourEffective() {
    transformToAdjList();

    std::vector<std::set<int>> edges(unweighted_adj_list);
    std::stack<int> s;
    std::vector<int> tour;
    tour.reserve(n);

    bool has_cycle;
    int u = checkEuler(has_cycle) - 1;
    s.push(u);
    while (!s.empty()) {
        int v = s.top();
        if (!edges[v].empty()) {
            u = *edges[v].begin();
            edges[v].erase(u);
            edges[u].erase(v);
            s.push(u);
        } else {
            tour.push_back(s.top() + 1);
            s.pop();
        }
    }

    return tour;
}

int Graph::checkBipart(std::vector<char> &marks) {
    int res = 1;
    for (int i = 0; i < n; ++i) {
        if (marks[i] == ' ' && res) dfs2(i, 'A', marks,res);
    }
    return res;
}

std::vector<std::pair<int, int>> Graph::getMaximumMatchingBipart() {
    transformToAdjList();

    std::vector<char> used(n);
    std::vector<char> used1(n);
    std::vector<int> parent(n, -1);

    for (int i = 0; i < n; ++i) {
        for (int u : unweighted_adj_list[i]) {
            if (parent[u] == -1) {
                parent[u] = i;
                used1[i] = true;
                break;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (used1[i]) continue;
        used.assign(n, false);
        dfs3(i, used, parent);
    }

    std::vector<std::pair<int, int>> res;
    for (int i = 0; i < n; ++i) {
        if (parent[i] != -1) {
            res.emplace_back(parent[i] + 1, i + 1);
            parent[parent[i]] = -1;
        }
    }

    return res;
}

Graph Graph::flowFordFulkerson(int source, int sink) {
    transformToAdjList();
    source--; sink--;

    std::vector<std::map<int, int>> edges(adj_list);
    for (int i = 0; i < n; ++i) {
        for (auto edge : edges[i]) {
            edges[edge.first].insert({i, 0});
        }
    }

    std::vector<char> used(n, false);
    std::vector<std::pair<int, int>> parent(n, {-1, INT_MAX});
    while (bfs(source, sink, edges, used, parent)) {
        int flow = parent[sink].second;
        for (int v = sink; parent[v].first != -1; v = parent[v].first) {
            edges[parent[v].first][v] -= flow;
            edges[v][parent[v].first] += flow;
        }
        used.assign(n, false);
        parent.assign(n, {-1, INT_MAX});
    }

    Graph res(n, true);
    for (int i = 0; i < n; ++i) {
        for (auto edge : adj_list[i]) {
            if (edges[edge.first][i])
                res.addEdge(i + 1, edge.first + 1, edges[edge.first][i]);
        }
    }

    return res;
}

Graph Graph::flowDinitz(int source, int sink) {
    transformToAdjList();
    source--; sink--;

    std::vector<std::map<int, int>> edges(adj_list);
    for (int i = 0; i < n; ++i) {
        for (auto edge : edges[i]) {
            edges[edge.first].insert({i, 0});
        }
    }

    std::vector<std::set<int>> layer_net(n);
    std::vector<int> parent(n, -1);
    while (bfs2(source, sink, edges, layer_net)) {
        while (!layer_net[0].empty()) {
            int v = 0, flow = INT_MAX;
            parent.assign(n, -1);
            while (layer_net[v].begin() != layer_net[v].end()) {
                int u = *layer_net[v].begin();
                parent[u] = v;
                flow = std::min(flow, edges[v][u]);
                v = u;
            }
            layer_net[parent[v]].erase(v);
            if (v == sink) {
                for (;parent[v] != -1; v = parent[v]) {
                    edges[parent[v]][v] -= flow;
                    edges[v][parent[v]] += flow;
                }
            }
        }
        layer_net.assign(n, {});
    }

    Graph res(n, true);
    for (int i = 0; i < n; ++i) {
        for (auto edge : adj_list[i]) {
            if (edges[edge.first][i])
                res.addEdge(i + 1, edge.first + 1, edges[edge.first][i]);
        }
    }

    return res;
}

int main()
{

    return 0;
}
