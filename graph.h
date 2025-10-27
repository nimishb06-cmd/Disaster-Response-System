#ifndef GRAPH_H
#define GRAPH_H

#define MAXN 100
#define INF 1000000000

extern int graph[MAXN][MAXN];

/* Dijkstra: compute dist[] of size nodes from src */
void dijkstra(int nodes, int src, int dist[]);

#endif
