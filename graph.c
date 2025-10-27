#include "graph.h"
#include <limits.h>

int graph[MAXN][MAXN];

void dijkstra(int nodes, int src, int dist[]) {
    int visited[MAXN];
    for (int i = 0; i < nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;

    for (int count = 0; count < nodes - 1; count++) {
        int u = -1;
        int min = INF;
        for (int v = 0; v < nodes; v++) {
            if (!visited[v] && dist[v] < min) {
                min = dist[v];
                u = v;
            }
        }
        if (u == -1) break;
        visited[u] = 1;
        for (int v = 0; v < nodes; v++) {
            if (!visited[v] && graph[u][v] > 0 && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
}
