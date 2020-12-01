#include <iostream>
#include <cstdlib>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>
#include "Graph.h"
#include <queue>

using namespace std;

bool bfs(Graph* G, int s, int t, vector<int>& parent){
    queue<int> Q;
    for(int i = 0; i < G->V; i++) parent[i] = -1;
    vector<bool> visited(G->V, false);

    visited[s] = true;
    Q.push(s);

    while(!Q.empty()){
        int u = Q.back();
        for(int v : G->edges[u]){
            if(!visited[v] && G->weights[make_pair(u, v)] > 0){
                visited[v] = True;
                parent[v] = u;
                Q.push(v);
            }
        }
    }

    if(parent[t] != -1) return true;
    else return false;
}

int maxflow(Graph* G, int s, int t){
    int max_flow = 0;
    vector<int> parent(G->V);

}

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
