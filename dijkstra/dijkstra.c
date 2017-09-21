/* Henrique Lima Cará de Oliveira RA 091518 */
/* quase que não dá =p */

#include <stdio.h>
#include <malloc.h>

#define INFINITY 100000

typedef enum bool {false, true} bool;

typedef struct pair {
  int key;
  int value;
} pair;

typedef struct heap {
  pair *v;
  int *map;
  int heap_size;
} heap;

typedef struct node node;
struct node {
  int v, w;
  node *next;
};

typedef struct graph {
  node **adj;
} graph;

void edge_add(graph *G, int u, int v, int w) {
  node *p;
  /* Adição de (u, v) */
  p = (node *)malloc(sizeof(node));
  p->v = v;
  p->w = w;
  p->next = NULL;
  if (G->adj[u] != NULL) {
    p->next = G->adj[u];
  }
  G->adj[u] = p;
}

void graph_destroy(graph *G, int n) {
  int i;
  node *q, *r;
  for (i = 0; i < n; i++) {
    q = G->adj[i];
    if (q != NULL) {
      r = q->next;
      if (r == NULL) {
        free(q);
      }
      else {
        for (q = G->adj[i], r = G->adj[i]->next; r != NULL; q = r, r = r->next) {
          free(q);
        }
      }
    }
  }
  free(G->adj);
  free(G);
}

void heapify(pair *v, int i, int n, int *map) {
  int esq, dir, menor;
  pair aux;
  /* Enquanto não é folha */
  while (i < n/2) {
    esq = i*2+1;
    dir = i*2+2;
    if (dir >= n) {
      menor = esq;
    }
    else if (v[esq].value < v[dir].value) {
      menor = esq;
    }
    else {
      menor = dir;
    }
    /* Se o valor do pai for maior que o valor do menor dos filhos, troca; mas se não ocorrer troca, quebra o laço. */
    if (v[i].value > v[menor].value) {
      map[v[i].key] = menor;
      map[v[menor].key] = i;
      aux = v[i];
      v[i] = v[menor];
      v[menor] = aux;
      i = menor;
    }
    else
      return;
  }
}

void build_heap(heap *h, int s, int n) {
  int i;
  h->v = (pair *)malloc(n * sizeof(pair));
  h->map = (int *)malloc(n * sizeof(int));
  for (i = 0; i < n; i++){
    h->v[i].key = i;
    h->v[i].value = INFINITY;
    h->map[i] = i;
  }
  h->v[s].value = 0;
  h->heap_size = n;
  /* Inspeciona desde o primeiro elemento que não é folha, até a raiz */
  for (i = n/2; i >= 0; i--) {
    heapify(h->v, i, n, h->map);
  }
}

bool heap_is_empty(heap *h) {
  if (!h->heap_size) {
    return true;
  }
  return false;
}

int heap_extract_min(heap *h) {
  int min = h->v[0].key;
  h->heap_size--;
  h->map[min] = -1;
  h->v[0] = h->v[h->heap_size];
  h->map[h->v[0].key] = 0;
  heapify(h->v, 0, h->heap_size, h->map);
  return min;
}

void decrease_value(heap *h, int key, int value) {
  int i = h->map[key], parent;
  pair aux;
  h->v[i].value = value;
  parent = (i-1)/2;
  while (i > 0 && h->v[parent].value > h->v[i].value) {
    // Põe o filho na posição do pai.
    h->map[h->v[parent].key] = i;
    // Põe o pai na posição do filho.
    h->map[h->v[i].key] = parent;
    aux = h->v[parent];
    h->v[parent] = h->v[i];
    h->v[i] = aux;
    i = (i-1)/2;
    parent = (i-1)/2;
  }
}

void heap_destroy(heap *h) {
  free(h->v);
  free(h->map);
  free(h);
}

void initialize_single_source(int s, int *d, int *pi, int n, heap *h) {
  int i;
  for (i = 0; i < n; i++) {
    d[i] = INFINITY;
    pi[i] = -1;
  }
  d[s] = 0;
  build_heap(h, s, n);
}

void relax(graph *G, int u, int v, int w, int *d, int *pi, heap *h) {
  if (d[v] > (d[u] + w)) {
    d[v] = d[u] + w;
    pi[v] = u;
    decrease_value(h, v, d[v]);
  }
}

void dijkstra(graph *G, int s, int *d, int *pi, int n) {
  int u;
  node *p;
  heap *h = (heap *)malloc(sizeof(heap));
  initialize_single_source(s, d, pi, n, h);
  while(!heap_is_empty(h)) {
    u = heap_extract_min(h);
    for (p = G->adj[u]; p != NULL; p = p->next) {
      relax(G, u, p->v, p->w, d, pi, h);
    }
  }
  heap_destroy(h);
}

int main(void) {
  int i, n, m, s, x, y, w, *d, *pi;
  graph *G;
  /* Leitura */
  scanf("%d %d %d", &n, &m, &s);
  /* Alocação */
  d = (int *)malloc(n * sizeof(int));
  pi = (int *)malloc(n * sizeof(int));
  G = (graph *)malloc(sizeof(graph));
  G->adj = (node **)malloc(n * sizeof(node *));
  /* Inicializações */
  for (i = 0; i < n; i++) {
    G->adj[i] = NULL;
  }
  /* Leitura e construção do grafo */
  for (i = 0; i < m; i++) {
    scanf("%d %d %d", &x, &y, &w);
    edge_add(G, x, y, w);
  }
  /* Dijkstra */
  dijkstra(G, s, d, pi, n);
  /* Impressão */
  for (i = 0; i < n-1; i++) {
    if (d[i] != INFINITY) {
      printf("%d ", d[i]);
    }
    else {
      printf("-1 ");
    }
  }
  if (d[n-1] != INFINITY) {
    printf("%d\n", d[n-1]);
  }
  else {
    printf("-1\n");
  }
  /* Desalocações */
  free(d);
  free(pi);
  graph_destroy(G, n);
  return 0;
}
