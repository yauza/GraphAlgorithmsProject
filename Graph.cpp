
#include "Graph.h"



Graph :: Graph(int V, int B){
    this->V = V;
    this->Budget = B;
}


void Graph :: printGraph(){
    for(auto p : cost){
        printf("krawedz: %d do %d, koszt: %d\n", p.first.first, p.first.second, p.second);
    }
}
