#include <iostream>
#include <cstdlib>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>
#include "Graph.h"
#include <queue>

#define INT_MAX 2147483647

using namespace std;

void printVector(vector<int> v){
    for(auto i : v){
        printf("%d ", i);
    }
    printf("\n");
}

void printPairVector(vector<pair<int, int>> v){
    for(auto i : v){
        printf("  %d-%d  ", i.first, i.second);
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
        int u = Q.front();
        Q.pop();
//        cout << "u:" << u << "  - ";
//        printVector(G->edges[u]);
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


int maxflow(Graph*& G, int s, int t){

    int max_flow = 0;
    //int test_cost = 0;
    vector<int> parent(G->V);

    bool isPath = bfs(G, s, t, parent);
    //printVector(parent);
    while(isPath){
        //int test_cost = 0;
        int x = t;
        int path_flow = INT_MAX;

        //printVector(parent);
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

//        for(auto r : G->residual){
//            cout << r.first.first << "-" << r.first.second << "   ->>>   ";
//            cout << r.second;
//            cout << endl;
//        }

        max_flow += path_flow;
        //printf("\nkazdy po kolei: %d \n", path_flow);
        isPath = bfs(G, s, t, parent);
    }

    //printf("AAA  AAA\n");

    return max_flow;
}


// bellman ford used to detect negative cycles
vector<pair<int, int>> bellman_ford(Graph*& G){
    int n = G->V;
    vector<pair<int, int>> cycle;
    vector<int> dist(n, INT_MAX);
    vector<int> parent(n, -1);

    int s = G->s, t = G->t;
    dist[s] = 0;
    parent[s] = s;

    for(int i = 1; i < n; i++){
        for(auto edge : G->edges){
            for(auto j : edge.second){
                int u = edge.first;
                int weight = G->cost[make_pair(u, j)];
                if(dist[u] != INT_MAX && dist[u] + weight < dist[j]){
                    dist[j] = dist[u] + weight;
                    parent[j] = u;
                }
            }
        }
    }
    unordered_set<pair<int, int>, pair_hash> sTemp;
    for(int i = 0; i < n; i++){
        for(auto v : G->edges[i]){
            int weight = G->cost[make_pair(i, v)];
            if(dist[v] > dist[i] + weight){
                pair<int, int> edge = make_pair(i, v);

                while(!sTemp.count(edge)){
                    sTemp.insert(edge);
                    cycle.push_back(edge);
                    edge = make_pair(parent[edge.first], edge.first);
                }

                return cycle;
            }
        }
    }

    return cycle;
}


void extendTheNetwork(Graph*& G, int s, int t, bool onlyUpdate){
    unordered_map<pair<int, int>, int, pair_hash> bounds;
    unordered_map<int, vector<int>> edges = G->edges;
    if(!onlyUpdate) {
        int n = G->V;
        int S = n, T = n + 1;
        G->s = S;
        G->t = T;
        G->V += 2;
    }
    int S = G->s, T = G->t;
//    vector<int> SAux(n, 0);
//    vector<int> TAux(n, 0);

    for(auto e : G->flow_capacity){
        bounds[make_pair(e.first.first, e.first.second)] = e.second.second - e.second.first;
    }

    bounds[make_pair(s, t)] = 1000000;
    bounds[make_pair(t, s)] = 1000000;
    if(!onlyUpdate) {
        edges[s].push_back(t);
        edges[t].push_back(s);
    }

    int temp = 0;
    for(int i = 1; i < G->players+1; i++){
        bounds[make_pair(S, i)] = 1;
        G->cost[make_pair(S, i)] = 0;
        if(!onlyUpdate) edges[S].push_back(i);
        if(G->flow_capacity[make_pair(i, G->King)] == make_pair(0, 1)) temp += 1;
    }


    bounds[make_pair(S, t)] = temp;


    bounds[make_pair(G->King, T)] = G->flow_capacity[make_pair(G->King, t)].first;
    bounds[make_pair(s, T)] = G->players;
    G->cost[make_pair(s, T)] = 0;
    G->cost[make_pair(G->King, T)] = 0;
    if(!onlyUpdate){
        edges[S].push_back(t);
        edges[s].push_back(T);
        edges[G->King].push_back(T);
    }

    //auto* GAux = new Graph(n+2, G->Budget);
    G->residual = bounds;
    G->edges = edges;

//    for(auto e : edges){
//        cout << e.first << "  -  ";
//        printVector(e.second);
//        cout << endl;
//    }
//
//    for(auto b : bounds){
//        cout << b.first.first << "-" << b.first.second << " -> ";
//        cout << b.second;
//        cout << endl;
//    }
//    cout << endl << endl;

//    return GAux;
}

//find min cost feasible flow
bool solve(Graph* G){
    int n = G->V;
    int cost = 0;
//    for(auto i : G->residual){
//        cout << i.first.first << "->" << i.first.second << " : " << i.second << " koszt: " << G->cost[make_pair(i.first.first, i.first.second)] << endl;
//    }
//    printf("-----------------------------------------\n");
    int maxflow_val = maxflow(G, G->s, G->t);
    cout << "MAXFLOW: " << maxflow_val << endl;
//    for(auto i : G->residual){
//        cout << i.first.first << "->" << i.first.second << " : " << i.second << " koszt: " << G->cost[make_pair(i.first.first, i.first.second)] << endl;
//    }
    vector<pair<int, int>> cycle = bellman_ford(G);
    printf("---------------------------------------------\n");
    printPairVector(cycle);
    printf("---------------------------------------------\n");
    vector<int> parent(G->V, -1);
//    while(bfs(G, G->s, G->t, parent)){
//        int path_flow = INT_MAX;
//
//        int x = G->t;
//        while(x != G->s){
//            path_flow = min(path_flow, G->residual[make_pair(parent[x], x)]);
//            x = parent[x];
//        }
//
//        int u = G->t;
//
//        while(u != G->s){
//            int v = parent[u];
//            G->residual[make_pair(v, u)] -= path_flow;
//            G->residual[make_pair(u, v)] += path_flow;
//            u = parent[u];
//            cost += path_flow * G->cost[make_pair(v, u)];
//        }
//        printf("path_flow: %d     cost: %d\n", path_flow, cost);
//    }

    for(int i = G->V - G->players - 3; i < G->V-3; i++){
        //cout << i << endl;
        for(auto v : G->edges[i]){
            //printVector(G->edges[i]);
            if(G->residual[make_pair(i, v)] == 1) cost += G->cost[make_pair(i, v)];
        }
    }


    printf("\n \n koszt: %d \n \n", abs(cost));
    return abs(cost) <= G->Budget;
}


int maxflow_with_lower_and_upper_bounds(Graph* G, int s, int t){
    unordered_map<pair<int, int>, int, pair_hash> bounds;
    unordered_map<int, vector<int>> edges = G->edges;
    int n = G->V;
    int S = n, T = n+1;
    vector<int> SAux(n, 0);
    vector<int> TAux(n, 0);

    for(auto e : G->flow_capacity){
        bounds[make_pair(e.first.first, e.first.second)] = e.second.second - e.second.first;
    }

    bounds[make_pair(s, t)] = 1000000;
    bounds[make_pair(t, s)] = 1000000;
    edges[s].push_back(t);
    edges[t].push_back(s);

    int temp = 0;
    for(int i = 1; i < G->players+1; i++){
        bounds[make_pair(S, i)] = 1;
        edges[S].push_back(i);
        if(G->flow_capacity[make_pair(i, G->King)] == make_pair(0, 1)) temp += 1;
    }
    edges[S].push_back(t);
    bounds[make_pair(S, t)] = temp;


    bounds[make_pair(G->King, T)] = temp;
    bounds[make_pair(s, T)] = G->players;
    edges[s].push_back(T);
    edges[G->King].push_back(T);

    auto* GAux = new Graph(n+2, G->Budget);
    GAux->residual = bounds;
    GAux->edges = edges;

    for(auto e : edges){
        cout << e.first << "  -  ";
        printVector(e.second);
        cout << endl;
    }

    for(auto b : bounds){
        cout << b.first.first << "-" << b.first.second << " -> ";
        cout << b.second;
        cout << endl;
    }
    cout << endl << endl;

    return maxflow(GAux, S, T);

}


int main() {
    int T;
    scanf("%d", &T);

    while(T--){
        int Budget, n;
        scanf("%d", &Budget);
        scanf("%d", &n);

        if(n <= 1){
            printf("TAK\n");
            continue;
        }

        int players = n;
        int V = 2 + n + ((n*(n-1))/2);
        auto* G = new Graph(V, Budget);
        G->players = n;

        vector<int> v(n, 0);
        G->playersScore = v;

        int T = 0, s = 0, t = ((n*(n-1))/2) + n + 1, King = ((n*(n-1))/2) + 1;
        G->King = King;
        for(int i = 1; i < ((n*(n-1))/2)+1; i++) {
            int x, y, w, b;
            scanf("%d %d %d %d", &x, &y, &w, &b);

            x += ((n*(n-1))/2)+1;
            y += ((n*(n-1))/2)+1;
            w += ((n*(n-1))/2)+1;

            G->edges[s].push_back(i);
            G->edges[i].push_back(s);
            G->edges[i].push_back(x);
            G->edges[i].push_back(y);
            G->edges[x].push_back(i);
            G->edges[y].push_back(i);

            G->residual[make_pair(i, x)] = 1;
            G->residual[make_pair(i, y)] = 1;

            G->flow_capacity[make_pair(i, x)] = make_pair(0, 1);
            G->flow_capacity[make_pair(i, y)] = make_pair(0, 1);
            G->flow_capacity[make_pair(x, i)] = make_pair(0, 0);
            G->flow_capacity[make_pair(y, i)] = make_pair(0, 0);
            G->flow_capacity[make_pair(s, i)] = make_pair(1, 1);
            G->flow_capacity[make_pair(i, s)] = make_pair(0, 0);

            G->cost[make_pair(s, i)] = 0;
            G->cost[make_pair(i, s)] = 0;

            G->residual[make_pair(s, i)] = 1;
            G->residual[make_pair(i, s)] = 0;
            if(w == x) {
                G->cost[make_pair(i, x)] = 0;
                G->cost[make_pair(i, y)] = b;
                G->cost[make_pair(y, i)] = -b;
            }else{
                G->cost[make_pair(i, y)] = 0;
                G->cost[make_pair(i, x)] = b;
                G->cost[make_pair(x, i)] = -b;
            }

            if(w == King) T++;

        }

        bool solved = false;

        for(int j = 0; j < n-T; j++){
            int x = T + j;

            for(int k = 0; k < n; k++){
                int player = k + ((n*(n-1))/2) + 1;
                if(G->edges[player].back() != t) G->edges[player].push_back(t);
                if(j == 0) G->edges[t].push_back(player);
                G->residual[make_pair(t, player)] = 0;
                //G->edges[player].push_back(t);
                //printVector(G->edges[player]);

                //G->flow_capacity[make_pair(player, t)] = make_pair();

                if(player != King) G->flow_capacity[make_pair(player, t)] = make_pair(0, x);
                else G->flow_capacity[make_pair(player, t)] = make_pair(x, x);
                G->flow_capacity[make_pair(t, player)] = make_pair(0, 0);
                G->residual[make_pair(player, t)] = x;
            }


            //cout << "-----------------" << x << endl;
            int cost = 0;
            if(j == 0) extendTheNetwork(G, s, t, false);
            else extendTheNetwork(G, s, t, true);

           // int res = maxflow(G, G->s, G->t);

            //printf("\n\n Maksymalny przeplyw: %d\n\n", res);
            solved = solve(G);

            if(solved) break;



        }

        if(solved) {
            printf("*****\n");
            printf("*TAK*\n");
            printf("*****\n");
        }else {
            printf("*****\n");
            printf("*NIE*\n");
            printf("*****\n");
        }

//        if(solve(G)) printf("TAK\n");
//        else printf("NIE\n");

//        G->printGraph();
//        //printf("\n\n %d \n", G->V);
//        int res = maxflow_with_lower_and_upper_bounds(G, s, t);
//
//        printf("\n\n Maksymalny przeplyw: %d\n\n", res);


//        G->printGraph();
//        int cost = 0;
//        int res = maxflow(G, s, t, cost);
//
//        printf("\n\n Maksymalny przeplyw: %d", res);
//        if(solve(G, s, t)) printf("\nTAK");
//        else printf("\nNIE");
        cout << endl;
        cout << "********************************************************************************************************" << endl;
        cout << "Zestaw kolejny " << endl;
        cout << "********************************************************************************************************" << endl;
    }

    return 0;
}
