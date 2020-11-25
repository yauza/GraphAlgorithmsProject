#include <iostream>
#include <stdlib.h>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>
#include "Graph.h"

using namespace std;


bool solve(int n, vector<int> playersScore){
    int King = 0;
    if(playersScore[King] < (n-1)/2){

    }
}

int main() {
    int T;
    scanf("%d", &T);

    while(T--){
        int Budget, n;
        scanf("%d", &Budget);
        scanf("%d", &n);


        auto* G = new Graph(n, Budget);

        vector<int> v(n, 0);
        G->playersScore = v;

        for(int i = 0; i < (n*(n-1))/2; i++) {
            int x, y, w, b;
            scanf("%d %d %d %d", &x, &y, &w, &b);
            if (w == x) {
                G->edges[y].push_back(x);
                G->edgesReversed[x].push_back(y);
                G->weights[make_pair(x, y)] = b;
            }else {
                G->edges[x].push_back(y);
                G->edgesReversed[y].push_back(x);
                G->weights[make_pair(y, x)] = b;
            }

            G->playersScore[w]++;
        }
        G->printGraph();


    }

    return 0;
}
