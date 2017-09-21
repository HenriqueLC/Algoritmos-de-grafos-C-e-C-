#include <stdio.h>
#include <stdlib.h>
#include <queue>

/* Henrique Lima Cará de Oliveira
 * RA 091518 
 */

using namespace std;

/* Estrutura do grafo */
typedef struct graph {
  int **adj;
  int *nadj;
} graph;

void bfs(int s, int *d, graph *G, int n) {
  bool *vis;
  int i, u, v;
  vis = (bool *)malloc(n*sizeof(bool));
  queue <int> q;
  /* Inicialização */
  q.push(s);
  d[s] = 0;
  vis[s] = true;
  /* BFS */
  while (!q.empty()) {
    u = q.front();
    q.pop();
    for (i = 0; i < G->nadj[u]; i++) { 
      v = G->adj[u][i];
      if (!vis[v]) {
        q.push(v);
        d[v] = d[u] + 1;
        vis[v] = true;
      }
    }
  }
  /* Desalocação */
  free(vis);
}

int main(void) {
  int i, n, m, s, from, to, *d;
  graph *G;

  scanf("%d %d %d", &n, &m, &s);
  /* Alocação */  
  d = (int *)malloc(n*sizeof(int));
  G = (graph *)malloc(sizeof(graph));
  G->adj = (int **)malloc(n*sizeof(int *));
  G->nadj = (int *)malloc(n*sizeof(int));
  for (i = 0; i < n; i++)
    G->adj[i] = (int *)malloc(m*sizeof(int));
  /* Inicialização */
  for (i = 0; i < n; i++) {
    G->nadj[i] = 0;
    d[i] = n;
  }
  /* Leitura e construção do grafo */
  for (i = 0; i < m; i++) {
    scanf("%d %d", &from, &to);
    G->adj[from][(G->nadj[from])++] = to;
  }
  /* BFS */
  bfs(s, d, G, n);
  /* Impressão */
  for (i = 0; i < n-1; i++) {
    printf("%d ", d[i]);
  }
  printf("%d", d[n-1]); // gambiarra
  printf("\n");
  /* Desalocação */
  free(d);
  for (i = 0; i < n; i++) {
    free(G->adj[i]);
  }
  free(G->adj);
  free(G->nadj);
  free(G);
  return 0;
}
