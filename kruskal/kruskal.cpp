/* Henrique Lima Cará de Oliveira RA 091519 */

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

/* Estrutura do grafo */

typedef pair<pair<int, int>, int> edge;

typedef struct graph {
  vector<edge> E;
} graph;

void make_set(int v, int *pi) {
  pi[v] = v;
}
// find_set utilizando compression path
int find_set(int v, int *pi) {
  if (v != pi[v]) {
    pi[v] = find_set(pi[v], pi);
  }
  return pi[v];
}

void union_by_rank(int u, int v, int *pi, int *rank) {
  int x = find_set(u, pi); 
  int y = find_set(v, pi);
  if (rank[x] <= rank[y]) {
    if (rank[x] == rank[y]) {
      rank[y]++;
    }
    pi[x] = y;
  }
  else {
    pi[y] = x;
  }
}

/* Ordenação com base no valor do segundo elemento do par externo */
struct predicate {
  bool operator()(const pair<pair<int, int>, int> &left, const pair<pair<int, int>, int> &right) {
    return left.second < right.second;
  }
};

void kruskal(graph *G, int *pi, int *rank, int n, int m, int *result) {
  int i, u, v, weight;
  /* Para cada vértice v pertencente a V, faça MAKE-SET(v) */
  for (i = 0; i < n; i++) {
    make_set(i, pi);
  }
  /* Ordene as arestas em ordem nao-decrescente de peso */
  sort(G->E.begin(), G->E.end(), predicate());
  /* Para cada (u, v) pertencente a E faça se FIND-SET(u) != FIND-SET(v),
     então some o peso da aresta ao peso do caminho resultante e faça UNION(u, v) */
  for (i = 0; i < m; i++) {
    u = G->E.at(i).first.first;
    v = G->E.at(i).first.second;
    weight = G->E.at(i).second;
    if (find_set(u, pi) != find_set(v, pi)) {
      *result += weight;
      union_by_rank(u, v, pi, rank);
    }
  }
}

int main(void) {
  int i, n, m, u, v, weight, *pi, *rank, result = 0;
  graph *G;
  /* Leitura */
  scanf("%d %d", &n, &m);
  /* Alocação */  
  pi = (int *)malloc(n*sizeof(int));
  rank = (int *)malloc(n*sizeof(int));
  G = (graph *)malloc(sizeof(graph));
  /* Leitura e construção do grafo */
  for (i = 0; i < m; i++) {
    scanf("%d %d %d", &u, &v, &weight);
    G->E.push_back(make_pair(make_pair(u, v), weight));
  }
  /* Kruskal */
  kruskal(G, pi, rank, n, m, &result);
  /* Impressão */
  printf("%d\n", result);
  /* Desalocação */
  free(pi);
  free(rank);
  G->E.clear();
  free(G);
  return 0;
}
