
#include "Graph.h"



Graph :: Graph(int V, int B){
    this->V = V;
    this->Budget = B;
}


void Graph :: printGraph(){
    for(auto p : weights){
        printf("krawedz: %d do %d, waga: %d\n", p.first.first, p.first.second, p.second);
    }
}
