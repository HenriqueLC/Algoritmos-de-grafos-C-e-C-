#include <stdio.h>
#include <stdlib.h>

/* Henrique Lima Cará de Oliveira
 * RA 091518 
 */

#define INFINITY 10000

typedef enum bool {false, true} bool;

typedef struct queue {
  int *data;
  int size, head, tail;
} queue;

/* Estrutura do grafo */
typedef struct graph {
  int **adj;
  int *nadj;
} graph;

/* Estrutura da matrix */
typedef struct matrix {
  int **elem;
} matrix;

queue *queue_create(int n) {
  queue *q = (queue *)malloc(sizeof(queue));
  q->data = (int *)malloc(n * sizeof(int));
  q->size = n;
  q->head = 0;
  q->tail = 0;
  return q;
}

int front(queue *q) {
  return q->data[q->head % q->size];
}

void push(queue *q, int elem) {
  q->data[(q->tail % q->size)] = elem;
  q->tail++;
}

void pop(queue *q) {
  q->head++;
}

bool is_empty(queue *q) {
  if (q->head == q->tail) {
    return true;
  }
  return false;
}

void queue_destroy(queue *q) {
  free(q->data);
  free(q);
  q = NULL;
}

/* Cria uma matriz quadrada n x n*/
void matrix_create(matrix *M, int n) {
  int i;
  M->elem = (int **)malloc(n * sizeof(int *));
  for (i = 0; i < n; i++) {
    M->elem[i] = (int *)malloc(n * sizeof(int));
  }
}

void matrix_destroy(matrix *M, int n) {
  int i;
  for (i = 0; i < n; i++) {
    free(M->elem[i]);
  }
  free(M->elem);
  free(M);
  M = NULL;
}

bool bfs(graph *G, matrix *C, int s, int t, int *pi, int n) {
  bool *vis;
  int i, u, v;
  queue *q = queue_create(n);
  vis = (bool *)malloc(n * sizeof(bool));
  for (i = 0; i < n; i++) {
    vis[i] = false;
  }
  push(q, s);
  pi[s] = -1;
  vis[s] = true;
  /* BFS */
  while (!is_empty(q)) {
    u = front(q);
    pop(q);
    for (i = 0; i < G->nadj[u]; i++) { 
      v = G->adj[u][i];
      if (!vis[v] && C->elem[u][v] > 0) {
        push(q, v);
        pi[v] = u;
        vis[v] = true;
        if (v == t) {
          free(vis);
          queue_destroy(q);
          return true;
        }
      }
    }
  }
  free(vis);
  queue_destroy(q);
  return false;
}

int min(int x, int y) {
  return x < y ? x : y;  // returns minimum of x and y
}

int max_flow(matrix *E, graph *G, matrix *C, int s, int t, int *pi, int n, matrix *F) {

  int max_flow = 0;
  int i, alpha;
  while (bfs(G, C, s, t, pi, n)) {
    /* Determina o custo mínimo do caminho mais curto de s à t */
    alpha = INFINITY;
    for (i = t; i != s; i = pi[i]) {
      alpha = min(alpha, C->elem[pi[i]][i]);
    }
    /* Atualiza os custos e os fluxos */
    for (i = t; i != s; i = pi[i]) {
      C->elem[pi[i]][i] -= alpha;
      C->elem[i][pi[i]] += alpha;
      if (E->elem[pi[i]][i]) {
        F->elem[pi[i]][i] += alpha;
      }
      else {
        F->elem[pi[i]][i] -= alpha;
      }
    }
    max_flow += alpha;
  }
  return max_flow;
}

int main(void) {

  int i, n, m, s, t, from, to, cust, *pi;
  graph *G;
  matrix *E, *C, *F;

  scanf("%d %d %d %d", &n, &m, &s, &t);
  /* Alocação */  
  pi = (int *)malloc(n * sizeof(int));
  G = (graph *)malloc(sizeof(graph));
  G->adj = (int **)malloc(n * sizeof(int *));
  G->nadj = (int *)malloc(n * sizeof(int));
  E = (matrix *)malloc(sizeof(matrix));
  matrix_create(E, n);
  C = (matrix *)malloc(sizeof(matrix));
  matrix_create(C, n);
  F = (matrix *)malloc(sizeof(matrix));
  matrix_create(F, n);
  for (i = 0; i < n; i++) {
    G->adj[i] = (int *)malloc(m * sizeof(int));
  }
  /* Inicialização */
  for (i = 0; i < n; i++) {
    G->nadj[i] = 0;
  }
  /* Leitura e construção do grafo e das matrizes*/
  for (i = 0; i < m; i++) {
    scanf("%d %d %d", &from, &to, &cust);
    E->elem[from][to] = 1;
    G->adj[from][(G->nadj[from])++] = to;
    G->adj[to][(G->nadj[to])++] = from;
    C->elem[from][to] = cust;
  }
  /* max_flow */
  printf("%d\n", max_flow(E, G, C, s, t, pi, n, F));
  /* Desalocações */
  free(pi);
  for (i = 0; i < n; i++) {
    free(G->adj[i]);
  }
  free(G->adj);
  free(G->nadj);
  free(G);
  matrix_destroy(E, n);
  matrix_destroy(C, n);
  matrix_destroy(F, n);

  return 0;
}
