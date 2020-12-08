
#include "Graph.h"


Graph :: Graph(int V, int B){
    this->V = V;
    this->Budget = B;
}


void Graph :: printGraph(){
    for(auto p : flow_capacity){
        printf("krawedz: %d do %d, up-low: %d-%d\n", p.first.first, p.first.second, p.second.first, p.second.second);
    }
}
