#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>

using namespace std;

#ifndef ALGGRAFPROJECT_GRAPH_H
#define ALGGRAFPROJECT_GRAPH_H

struct pair_hash{
    inline size_t operator()(const pair<int, int> & v) const{
        return v.first * 67 + v.second;
    }
};

class Graph {
public:
    int V;
    int Budget;
    unordered_map<int, vector<int>> edges;                  //jeżeli x wygrywa z y to krawędź jest od y do x
    unordered_map<int, vector<int>> edgesReversed;          //na odwrót
    unordered_map<pair<int, int>, int, pair_hash> weights;
    vector<int> playersScore;

    Graph(int V, int B);

    void printGraph();
};


#endif //ALGGRAFPROJECT_GRAPH_H
