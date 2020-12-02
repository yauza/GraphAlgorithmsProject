#include <iostream>
#include <cstdlib>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>
#include "Graph.h"
#include <queue>

using namespace std;

void printVector(vector<int> v){
    for(auto i : v){
        printf("%d ", i);
    }
    printf("\n");
}

bool bfs(Graph* G, int s, int t, vector<int>& parent){
    queue<int> Q;
    for(int i = 0; i < G->V; i++) parent[i] = -1;
    vector<bool> visited(G->V, false);

    visited[s] = true;
    Q.push(s);

    while(!Q.empty()){
        int u = Q.back();
        Q.pop();

        for(int v : G->edges[u]){
            if(!visited[v] && G->residual[make_pair(u, v)] > 0){
                visited[v] = true;
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

    bool isPath = bfs(G, s, t, parent);

    while(isPath){
        int x = t;
        int path_flow = INT_MAX;

        while(x != s){
            path_flow = min(path_flow, G->residual[make_pair(parent[x], x)]);
            x = parent[x];
        }

        int u = t;

        while(u != s){
            int v = parent[u];
            G->residual[make_pair(v, u)] -= path_flow;
            G->residual[make_pair(u, v)] += path_flow;
            u = parent[u];
        }

        max_flow += path_flow;

        isPath = bfs(G, s, t, parent);
    }

    return max_flow;
}

bool solve(){

}

int main() {
    int T;
    scanf("%d", &T);

    while(T--){
        int Budget, n;
        scanf("%d", &Budget);
        scanf("%d", &n);

        int V = 2 + n + ((n*(n-1))/2);
        auto* G = new Graph(V, Budget);

        vector<int> v(n, 0);
        G->playersScore = v;

        int T = 0, s = 0, t = ((n*(n-1))/2) + n + 1, King = ((n*(n-1))/2) + 1;

        for(int i = 1; i < ((n*(n-1))/2)+1; i++) {
            int x, y, w, b;
            scanf("%d %d %d %d", &x, &y, &w, &b);

            x += ((n*(n-1))/2)+1;
            y += ((n*(n-1))/2)+1;
            w += ((n*(n-1))/2)+1;

            G->edges[s].push_back(i);
            G->edges[i].push_back(x);
            G->edges[i].push_back(y);

            G->residual[make_pair(i, x)] = 1;
            G->residual[make_pair(i, y)] = 1;

            G->cost[make_pair(s, i)] = 1;
            G->residual[make_pair(s, i)] = 1;
            if(w == x) {
                G->cost[make_pair(i, x)] = 0;
                G->cost[make_pair(i, y)] = b;
            }else{
                G->cost[make_pair(i, y)] = 0;
                G->cost[make_pair(i, x)] = b;
            }

            if(w == King) T++;

        }

        for(int j = 0; j < n-T; j++){
            int x = T + j;

            for(int k = 0; k < n; k++){
                int player = k + ((n*(n-1))/2) + 1;
                G->edges[player].push_back(t);

                if(player != King) G->cost[make_pair(player, t)] = 0;
                else G->cost[make_pair(player, t)] = x;

                G->residual[make_pair(player, t)] = x;
            }


        }


        G->printGraph();

        int res = maxflow(G, s, t);
        printf("\n\n Maksymalny przeplyw: %d", res);

    }

    return 0;
}
