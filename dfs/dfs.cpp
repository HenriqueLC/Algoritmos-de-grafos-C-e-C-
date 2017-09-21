/* Henrique Lima Cará de Oliveira RA 091518 */

#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

enum {WHITE, GRAY, BLACK};

/* Estrutura do grafo */
typedef struct graph {
  int **adj;
  int *nadj;
} graph;

void dfs(int s, graph *G, int *d, int *f, int *pi, int n) {
  int *color, time, i, t, u, v;
  stack <int> p;
  /* Alocação */
  color = (int *)malloc(n*sizeof(int));
  /* Inicialização */
  for (i = 0; i < n; i++) {
    color[i] = WHITE;
    pi[i] = -1;
  }
  time = 0;
  for (t = s; t < s+n; t++) {
    if (color[t%n] == WHITE) {
      p.push(t%n);
      /* DFS */
      while (!p.empty()) {
        u = p.top();
        p.pop();
        if (color[u] == WHITE) {
          d[u] = ++time;
          color[u] = GRAY;
          p.push(u);
          // Empilha todos os vizinhos brancos
          for (i = 0; i < G->nadj[u]; i++) {
            v = G->adj[u][i];
            if (color[v] == WHITE) {
              pi[v] = u;
              p.push(v);
            }
          }
        }
        else if (color[u] == GRAY) {
          f[u] = ++time;
          color[u] = BLACK;
        }
      }
    }
  }
  /* Desalocação */
  free(color);
}

/* Ordenação com base no valor do segundo elemento do par */
struct predicate {    
  bool operator()(const pair<int,int> &left, const pair<int,int> &right) {
    return left.second < right.second;
  }
};

int main(void) {
  int i, n, m, from, to, *d, *f, *pi;
  graph *G;
  vector<pair<int, int> > v;
  /* Leiturais e Alocações */  
  scanf("%d %d", &n, &m);
  d = (int *)malloc(n*sizeof(int));
  f = (int *)malloc(n*sizeof(int));
  pi = (int *)malloc(n*sizeof(int));
  G = (graph *)malloc(sizeof(graph));
  G->adj = (int **)malloc(n*sizeof(int *));
  G->nadj = (int *)malloc(n*sizeof(int));
  for (i = 0; i < n; i++) {
    G->adj[i] = (int *)malloc(m*sizeof(int));
  }
  /* Inicialização */
  for (i = 0; i < n; i++) {
    G->nadj[i] = 0;
  }
  /* Leitura e construção do grafo */
  for (i = 0; i < m; i++) {
    scanf("%d %d", &from, &to);
    G->adj[from][(G->nadj[from])++] = to;
//    G->adj[to][(G->nadj[to])++] = from;
  }
  /* DFS */
  dfs(0, G, d, f, pi, n);
  for (i = 0; i < n; i++) {
    v.push_back(make_pair(i, f[i]));
  }
  sort(v.begin(), v.end(), predicate());
  reverse(v.begin(), v.end());
  for (i = 0; i < n-1; i++) {
    printf("%d ", (v.at(i)).first);
  }
  printf("%d", (v.at(n-1)).first);
  printf("\n");
  /* Desalocação */
  free(d);
  free(f);
  free(pi);
  v.clear();
  for (i = 0; i < n; i++) {
    free(G->adj[i]);
  }
  free(G->adj);
  free(G->nadj);
  free(G);
  return 0;
}
