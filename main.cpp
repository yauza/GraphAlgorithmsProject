#include <iostream>
#include <cstdlib>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>
#include "Graph.h"
#include <queue>
#include <algorithm>
#include <bits/stdc++.h>

#define INT_MAX 2147483647

using namespace std;

struct edge {
    int res;
    int cost;

    edge(int a, int b){
        this->res = a;
        this->cost = b;
    }
};

void printVector(vector<int> v){
    for(auto i : v){
        printf("%d ", i);
    }
    printf("\n");
}

void printGraph(vector<vector<edge*>> g) {
    int n = g.size();
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(g[i][j]->res > 0) {
                printf("%d -> %d | res:%d  cost:%d \n", i, j, g[i][j]->res, g[i][j]->cost);
            }
        }
    }

}


vector<int> BellmanFord(vector<vector<edge*>> g){
    int n = g.size();
    vector<int> dist(n, INT_MAX);
    vector<int> parent(n, -1);
    int s = 0;
    dist[s] = 0;
    parent[s] = s;
    vector<pair<int, int>> E;

    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n; j++) {
            for(int k = 0; k < n; k++) {
                if (g[j][k]->res > 0) {
                    if(dist[j] != INT_MAX && dist[k] > dist[j] + g[j][k]->cost){
                        dist[k] = dist[j] + g[j][k]->cost;
                        parent[k] = j;
                    }
                }
            }
        }
    }


    return parent;

}


bool solve_NewApproach(vector<vector<edge*>> g, int value, int B){
    int cost = 0, flow = 0, t = g.size()-1, s = 0, n = g.size()-2;
    vector<int> parent;
    parent = BellmanFord(g);

    while(flow < value){
        parent = BellmanFord(g);
        //printGraph(g);
        printVector(parent);
        if(parent[t] == -1) break;
        int x = t, path_flow = INT_MAX, tempCost = 0;
        while(x != s){
            //cout << x << endl;
            path_flow = min(path_flow, g[parent[x]][x]->res);
            x = parent[x];
        }

        if(value - flow < path_flow) path_flow = value - flow;

        //cout << "path_flow: " << path_flow << endl;

        x = t;
        while(x != s){
            g[parent[x]][x]->res -= path_flow;
            g[x][parent[x]]->res += path_flow;
            tempCost += g[parent[x]][x]->cost * path_flow;
            g[x][parent[x]]->cost = -g[parent[x]][x]->cost;
            x = parent[x];
        }

        flow += path_flow;
        cost += tempCost;
        //cout << "cost: " << cost << endl << endl;
    }
    cout << "win cost: " << cost << endl;
    if(flow >= value) return cost <= B;
    else return false;
}


bool solveDiff(vector<vector<edge*>> g, int value, int B){
    int cost = 0, flow = 0, t = g.size()-1, s = 0, n = g.size()-2;
    vector<int> parent;
    parent = BellmanFord(g);
    //printGraph(g);
    while(flow < value){
        parent = BellmanFord(g);
        //printGraph(g);
        //printVector(parent);
        if(parent[t] == -1) break;
        int x = t, path_flow = INT_MAX, tempCost = 0;
        while(x != s){
            //cout << x << endl;
            path_flow = min(path_flow, g[parent[x]][x]->res);
            x = parent[x];
        }

        if(value - flow < path_flow) path_flow = value - flow;

        //cout << "path_flow: " << path_flow << endl;

        x = t;
        while(x != s){
            g[parent[x]][x]->res -= path_flow;
            g[x][parent[x]]->res += path_flow;
            tempCost += g[parent[x]][x]->cost * path_flow;
            g[x][parent[x]]->cost = -g[parent[x]][x]->cost;
            x = parent[x];
        }

        flow += path_flow;
        cost += tempCost;
        //cout << "cost: " << cost << endl << endl;
    }

    g[1][t]->res = 0;
    g[t][1]->res = 0;

    for(int i = 2; i < n+1; i++){
        g[i][t]->res = value;
        g[i][t]->cost = 0;
    }
    //printGraph(g);
    while(flow < ((n*(n-1))/2)){
        parent = BellmanFord(g);
        //printGraph(g);
        //printVector(parent);
        if(parent[t] == -1) break;
        int x = t, path_flow = INT_MAX, tempCost = 0;
        while(x != s){
            //cout << x << endl;
            path_flow = min(path_flow, g[parent[x]][x]->res);
            x = parent[x];
        }

        if((n*(n-1))/2 - flow < path_flow) path_flow = ((n*(n-1))/2) - flow;

        //cout << "path_flow: " << path_flow << endl;

        x = t;
        while(x != s){
            g[parent[x]][x]->res -= path_flow;
            g[x][parent[x]]->res += path_flow;
            tempCost += g[parent[x]][x]->cost * path_flow;
            g[x][parent[x]]->cost = -g[parent[x]][x]->cost;
            x = parent[x];
        }

        flow += path_flow;
        cost += tempCost;
        //cout << "cost: " << cost << endl << endl;
    }


    cout << "win cost: " << cost << endl;
    if(flow >= value) return cost <= B;
    else return false;
}



int main(){
    int T;
    scanf("%d", &T);

    while(T--) {
        int Budget, n;
        scanf("%d", &Budget);
        scanf("%d", &n);

        if (n <= 1) {
            printf("TAK\n");
            continue;
        }

        vector<vector<edge*>> graph;

        for(int i = 0; i < n+2; i++){
            vector<edge*> temp(n+2);
            //graph.push_back(temp);
            for(int j = 0; j < n+2; j++){
                edge* e = new edge(0,0);
                temp[j] = e;
            }
            graph.push_back(temp);
        }

        unordered_map<pair<int, int>, int, pair_hash> cost;
        vector<int> players(n+1, 0);
        int sum = 1;

        for(int i = 1; i < ((n*(n-1))/2)+1; i++) {
            int x, y, w, b;
            scanf("%d %d %d %d", &x, &y, &w, &b);
            x += 1;
            y += 1;
            w += 1;

            players[w]++;
            if(x == w){
                //cost[make_pair(x, y)] = b;
                graph[x][y] = new edge(1, b);
            }else{
                //cost[make_pair(y, x)] = b;
                graph[y][x] = new edge(1, b);
            }
            sum += b;
        }

        int s = 0, t = n+1;


        for(int i = 1; i < players.size(); i++){
            graph[s][i] = new edge(players[i], 0);
            //cost[make_pair(s, i+1)] = 0;
            //graph[i+1][t] = INT_MAX;
            if(i == 1) graph[i][t] = new edge(INT_MAX, 0);//cost[make_pair(i+1, t)] = 0;
            else graph[i][t] = new edge(INT_MAX, sum); //cost[make_pair(i+1, t)] = sum;
        }

        bool solved = false;
        cout << n-players[1] << " : " << ((n*(n-1))/2) << endl;
        int x;
        if(n%2 == 0) x = (n-1)/2 + 1;
        else x = (n-1)/2;

        //vector<vector<edge*>> gcopy;
        for(int j = x; j < n; j++){
            //int x = players[1] + j;
            vector<vector<edge*>> gcopy;
            for(int i = 0; i < graph.size(); i++){
                vector<edge*> temp;
                for(int j = 0; j < graph.size(); j++){
                    temp.push_back(new edge(graph[i][j]->res, graph[i][j]->cost));
                }
                gcopy.push_back(temp);
            }
            cout << "\nwywolujemy z x: " << j << endl;
            solved = solveDiff(gcopy, j, Budget);
            if(solved) break;
        }



        if(solved) printf("TAK\n");
        else printf("NIE\n");
        cout << "--------------------------------------kolejny zestaw\n\n" << endl;



        }

    return 0;

}




//int main() {
//    int T;
//    scanf("%d", &T);
//
//    while(T--){
//        int Budget, n;
//        scanf("%d", &Budget);
//        scanf("%d", &n);
//
//        if(n <= 1){
//            printf("TAK\n");
//            continue;
//        }
//
//        int players = n;
//        int V = 2 + n + ((n*(n-1))/2);
//        auto* G = new Graph(V, Budget);
//        G->players = n;
//
//        vector<int> v(n, 0);
//        G->playersScore = v;
//
//        int T = 0, s = 0, t = ((n*(n-1))/2) + n + 1, King = ((n*(n-1))/2) + 1;
//        G->King = King;
//        for(int i = 1; i < ((n*(n-1))/2)+1; i++) {
//            int x, y, w, b;
//            scanf("%d %d %d %d", &x, &y, &w, &b);
//
//            x += ((n*(n-1))/2)+1;
//            y += ((n*(n-1))/2)+1;
//            w += ((n*(n-1))/2)+1;
//
//            G->edges[s].push_back(i);
//            G->edges[i].push_back(s);
//            G->edges[i].push_back(x);
//            G->edges[i].push_back(y);
//            G->edges[x].push_back(i);
//            G->edges[y].push_back(i);
//
//            G->residual[make_pair(i, x)] = 1;
//            G->residual[make_pair(i, y)] = 1;
//            //---------nowe
////            G->residual[make_pair(x, i)] = 0;
////            G->residual[make_pair(y, i)] = 0;
//
//            G->flow_capacity[make_pair(i, x)] = make_pair(0, 1);
//            G->flow_capacity[make_pair(i, y)] = make_pair(0, 1);
//            G->flow_capacity[make_pair(x, i)] = make_pair(0, 0);
//            G->flow_capacity[make_pair(y, i)] = make_pair(0, 0);
//            G->flow_capacity[make_pair(s, i)] = make_pair(1, 1);
//            G->flow_capacity[make_pair(i, s)] = make_pair(0, 0);
//
//            G->cost[make_pair(s, i)] = 0;
//            G->cost[make_pair(i, s)] = 0;
//
//            G->residual[make_pair(s, i)] = 1;
//            G->residual[make_pair(i, s)] = 0;
//            if(w == x) {
//                G->cost[make_pair(i, x)] = 0;
//                G->cost[make_pair(i, y)] = b;
//                G->cost[make_pair(y, i)] = -b;  //było -b
//            }else{
//                G->cost[make_pair(i, y)] = 0;
//                G->cost[make_pair(i, x)] = b;
//                G->cost[make_pair(x, i)] = -b;  //było -b
//            }
//
//            if(w == King) T++;
//
//        }
//
//        bool solved = false;
//
//        for(int j = 0; j < n-T; j++){
//            int x = T + j;
//
//            for(int k = 0; k < n; k++){
//                int player = k + ((n*(n-1))/2) + 1;
//                if(G->edges[player].back() != t) G->edges[player].push_back(t);
//                if(j == 0) G->edges[t].push_back(player);
//                //G->residual[make_pair(t, player)] = 0;
//                if(player != King){
//                    G->residual[make_pair(t, player)] = 0;
//                    G->residual[make_pair(player, t)] = x;
//                }
//
//                if(player != King) G->flow_capacity[make_pair(player, t)] = make_pair(0, x);
//                else G->flow_capacity[make_pair(player, t)] = make_pair(x, x);
//                G->flow_capacity[make_pair(t, player)] = make_pair(0, 0);
//                //G->residual[make_pair(player, t)] = x;
//            }
//
//
//            //cout << "-----------------" << x << endl;
//            int cost = 0;
//            if(j == 0) extendTheNetwork(G, s, t, false);
//            else extendTheNetwork(G, s, t, true);
//
//           // int res = maxflow(G, G->s, G->t);
//
//            //printf("\n\n Maksymalny przeplyw: %d\n\n", res);
//            //solved = solve(G);
//
//            //if(solved) break;
//
//            solve_NewApproach(G, x);
//
//        }
//
////        if(solved) {
////            printf("*****\n");
////            printf("*TAK*\n");
////            printf("*****\n");
////        }else {
////            printf("*****\n");
////            printf("*NIE*\n");
////            printf("*****\n");
////        }
//
////        if(solve(G)) printf("TAK\n");
////        else printf("NIE\n");
//
////        G->printGraph();
////        //printf("\n\n %d \n", G->V);
////        int res = maxflow_with_lower_and_upper_bounds(G, s, t);
////
////        printf("\n\n Maksymalny przeplyw: %d\n\n", res);
//
//
////        G->printGraph();
////        int cost = 0;
////        int res = maxflow(G, s, t, cost);
////
////        printf("\n\n Maksymalny przeplyw: %d", res);
////        if(solve(G, s, t)) printf("\nTAK");
////        else printf("\nNIE");
//        cout << endl;
//        cout << "********************************************************************************************************" << endl;
//        cout << "Zestaw kolejny " << endl;
//        cout << "********************************************************************************************************" << endl;
//    }
//
//    return 0;
//}
