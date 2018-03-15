#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 点のデータ型 */
typedef struct vertex {
    char *name; /* 名前 */
    struct edge *adj; /* 隣接リスト */
    struct vertex *left; /* 頂点木の左の枝 */
    struct vertex *right; /* 頂点木の右の枝 */
  int distance; /*　始点からの距離 */
  struct vertex *footprint; /* dijkstraの際にたどった道 */
  int finish; /*深さ優先探索が終わったことをしめす*/
} *Vertex;
 
/* 辺のデータ型 */
typedef struct edge {
    struct vertex *targetp; /* もう一方の端点へのポインタ */
    int weight; /* 重み */
    struct edge *next; /* 残りの隣接リスト */
} *Edge;

typedef char *String;

Vertex vertex_empty(){
  Vertex a = (struct vertex *)malloc(sizeof(struct vertex));
  a->name = '\0';
  a->left = NULL;
  a->right = NULL;
  a->footprint = NULL;
  a->adj = NULL;
  a->distance = 0;
  a->finish = 1;
  return a;
}

Edge edge_empty(){
  Edge t = (struct edge *)malloc(sizeof(struct edge));
  t->targetp = vertex_empty();
  t->weight = 0;
  t->next = NULL;
  return t;
}
 
int vertex_isempty(Vertex t)
{
  if (t->name == '\0')
    return 4;
  else
    return 0;
}
 
Vertex vertex_insert(String key, Vertex t)
{
  Vertex a = (struct vertex *)malloc(sizeof(struct vertex));
  String b = (char *)malloc(100);
  strcpy(b, key);
  a->name = b;
  a->adj = edge_empty();
  a->left = vertex_empty();
  a->right = vertex_empty();
  a->distance = 0;
  a->footprint = vertex_empty();
  a->finish = 0;
  if (vertex_isempty(t) == 4){
    return a;
  }
  else if (strcmp(key, t->name) < 0){
    if (vertex_isempty(t->left)){
       t->left = a;
     return t;
      }
      else{
       t->left = vertex_insert(key, t->left);
       free(a);
       return t;
      }
    }
    else if (strcmp(key, t->name) > 0){
      if (vertex_isempty(t->right)){
	  t->right = a;
	  return t;
      }
      else{
	  t->right = vertex_insert(key, t->right);
	  free(a);
	  return t;
      }
      }
      else{
	t->name = key;
	free(a);
	return t;
      }
}

Vertex deletemin(Vertex t){
  if (vertex_isempty(t->left))
    if (vertex_isempty(t->right))
      return vertex_empty();
    else
      return t->right;
  else{
    t->left = deletemin(t->left);
    return t;
  }
}

String min_name(Vertex t){
  while (! vertex_isempty(t->left))
    t = t->left;
  return t->name;

}

 
Vertex vertex_delete(String key, Vertex t)
{
  if (vertex_isempty(t))
    return t;
  else if (strcmp(key, t->name) < 0){
    if (vertex_isempty(t->left))
      return t;
    else{
      t->left = vertex_delete(key, t->left);
      return t;
    }}
    else if (strcmp(key, t->name) > 0){
      if (vertex_isempty(t->right))
	return t;
      else{
	t->right = vertex_delete(key, t->right);
	return t;
      }}
    else if ((vertex_isempty(t->right)) && (vertex_isempty(t->left)))
	return vertex_empty();
    else if (vertex_isempty(t->left))
	return t->right;
    else if (vertex_isempty(t->right))
	return t->left;
      else{
        t->name = min_name(t->right);
	t->right = deletemin(t->right);
	return t;
      }
}
 
Vertex vertex_search(String key, Vertex t)
{
  if (vertex_isempty(t))
    return NULL;
  else if (strcmp(key, t->name) < 0){
    if (vertex_isempty(t->left))
      return NULL;
    else
      return vertex_search(key, t->left);
  }
  else if (strcmp(key, t->name) > 0){
    if (vertex_isempty(t->right))
      return NULL;
    else
      return vertex_search(key, t->right);
  }
  else
    return t;
}

Edge edge_cons(int s, Vertex t, Edge l)
{
                 Edge p = (struct edge *)malloc(sizeof(struct edge));
                 p->next = l;
                 p->weight = s;
                 p->targetp = t;
		 return p;
}


int edge_isempty(Edge l)
{
    /* `l'が空リストなら0以外を、そうでないなら0を返す。 */

  if ((l->weight == 0) && (l->next == NULL))
    return 4;
  else
    return 0;
    
}

void edge_free(Edge l)
{
    Edge s;
    /*
     * リスト`l'の使用しているメモリを全て解放する。
     * この関数を呼び出して以降、`l'を使用してはいけない。
     */
    while (! edge_isempty(l)) {
      s = l->next;
      free(l);
      l = s;
    }
}

Edge little(Edge l, int i)
{
  if (edge_isempty(l) == 4)
    return l;
  else{
    if ((l->weight < i) && (l->weight != 0))
      return edge_cons(l->weight, l->targetp, little(l->next, i));
    else
      return little(l->next, i);
  }
}

Edge big(Edge l, int i)
{
  if (edge_isempty(l) == 4 )
    return l;
  else{
    if ((l->weight < i) && (l->weight != 0))
      return big(l->next, i);
    else
      return edge_cons(l->weight, l->targetp, big(l->next, i));
  }
}


Edge append(Edge a, Edge b)
{
  if (edge_isempty(a) == 4)
    return b;
  else
    return edge_cons(a->weight, a->targetp, append(a->next, b));
}
  


Edge sort(Edge l)
{
  if (edge_isempty(l) == 4)
      return l;
  else {if (edge_isempty(l->next) == 4)
      return l;
    else
      { Edge a = little(l, l->weight);
	if (edge_isempty(a) == 4)
	  return edge_cons(l->weight, l->targetp, sort(l->next));
	else{
	  Edge b = big(l, l->weight);
	  edge_free(l);
	  Edge c = append(sort(a), sort(b));
          edge_free(a);
          return c;
	}
	    }
  }
}



void free_vertex(Vertex t)
{
  if ((vertex_isempty(t->right)) && (vertex_isempty(t->left))){
    free(t->name);
    edge_free(t->adj);
    free(t);
  }
  else if (vertex_isempty(t->left)){
    Vertex a;
    a = t->right;
    free(t->name);
    edge_free(t->adj);
    free(t);
    free_vertex(a);
  }
  else if (vertex_isempty(t->right)){
    Vertex b;
    b = t->left;
    free(t->name);
    edge_free(t->adj);
    free(t);
    free_vertex(b);
  }
  else{
    Vertex c;
    Vertex d;
    c = t->left;
    d = t->right;
    free(t->name);
    edge_free(t->adj);
    free(t);
    free_vertex(c);
    free_vertex(d);
  }
}

Vertex searching(Vertex t){
  if ((! vertex_isempty(t->left)) && ((t->left)->finish == 0))
    return searching(t->left);
    else if ((! vertex_isempty(t->right)) && ((t->right)->finish == 0))
      return searching(t->right);
    else
      return t;
}

int edge_search(Vertex t, Edge a){
  if (edge_isempty(a))
    return 0;
  else if ((a->targetp) == t)
    return a->weight;
  else
    return edge_search(t, a->next);
}


void swap_vertex(Vertex u[1000], int i, int j){
  Vertex a = u[i];
  u[i] = u[j];
  u[j] = a;
}

void heap_insert(Vertex u[1000], int i){
  while (i > 0){
    if ((u[(i-1)/2]->distance > u[i]->distance) || (u[(i-1)/2]->distance == 0)){
      swap_vertex(u, (i-1)/2, i);
      i = (i-1)/2;
    }
    else break;
  }
}


void dijkstra(Vertex u[1000], Vertex w, int i){
  Edge k = w->adj;
  while (! edge_isempty(k)){
    if (((((k->weight) + (w->distance)) < ((k->targetp)->distance)) || ((k->targetp)->distance == 0)) && ((k->targetp)->finish == 1) && (w->distance != 0)){
      (k->targetp)->distance = (k->weight) + (w->distance);
      (k->targetp)->footprint = w;
      int j = 0;
      while ((! ((k->targetp) == u[j])) && (j < i))
	j++;
      heap_insert(u, j);
    }
    k = k->next;
  }

}


void heap_delete(Vertex u[1000], int i){
  u[0] = u[i];
  int j = 0;
  while (j < i / 2){
    if ((((u[j]->distance) > (u[j*2+1]->distance)) && (u[j*2+1]->distance != 0)) || (u[j]->distance == 0)){
      if ((((u[j*2+2]->distance) < (u[j*2+1]->distance)) && (u[j*2+2]->distance != 0)) || (u[j*2+1]->distance == 0)){
        swap_vertex(u, j, j*2 + 2);
        j = j*2 + 2;
      }
      else{
	swap_vertex(u, j, j*2+1);
	j = j*2 + 1;
      }
    }
  else if ((((u[j]->distance) > (u[j*2+2]->distance)) && (u[j*2+2]->distance != 0)) || (u[j]->distance == 0)){
      swap_vertex(u, j, j*2+2);
      i = i*2+2;
    }
    else
      break;
  }
}

int main(){
  String a = (char *)malloc(100);
  String b = (char *)malloc(100);
  String d = (char *)malloc(100);
  String e = (char *)malloc(100);
  int c;
  Vertex t = vertex_empty();/* 変数を入力*/
  scanf("%99s",a);
  scanf("%99s", b);
  t = vertex_insert(a, t);
  t = vertex_insert(b, t);
  /* loading data */
  /* tの初期設定 */
  Vertex m;
  Vertex n;
  while (scanf("%99s", d) != EOF)
    { if (strcmp(d, "quit") == 0)
	break;
      scanf("%99s", e);
      scanf("%d", &c);
      if (vertex_search(d,t) == NULL)
        t = vertex_insert(d, t);
      if (vertex_search(e,t) == NULL)
	t = vertex_insert(e, t);
      m = vertex_search(d, t);
      n = vertex_search(e, t);
      m->adj = edge_cons(c, n, m->adj);
      n->adj = edge_cons(c, m, n->adj);
    }
      /* データをロード */
  Edge S = sort((vertex_search(a, t))->adj);
  Vertex U[1000];
  int i = 0;
  for (i = 0; i < 1000; i++)
    U[i] = vertex_empty();
  Vertex p;
  i = 0;
  while (((t->right)->finish == 0) || ((t->left)->finish == 0)){
    p = searching(t);
    p->finish = 1;
    p->distance = edge_search(p, S);
    U[i] = p;
    if (p->distance != 0)
    heap_insert(U, i);
    i = i + 1;
  /* ヒープの作成。 */
      }
  /* dijkstra start */
  i--;
  Vertex w;
  while (i > -1){
    w = U[0];
    heap_delete(U,i);
  /*除外状態にないもので一番始点に近いものをとってくる */
  /* ループ開始 */
    dijkstra(U, w, i);
    w->finish = 2;
    i = i-1;
  /*　始点からの距離を更新 */
  /* 元々の点を消して、新たに距離を更新した点をinsertする */
  /* その際、爪あとをのこしておきたい */
  }
  i = 0;
  Vertex x = vertex_search(b, t);
  if (x->distance == 0)
    puts("(no route)");
  else{
    while (! vertex_isempty(x)){
      U[i] = x;
      x = x->footprint;
      i++;
      }
  /* 爪あとを元にたどって完成　*/
  printf("%s\n", a);
  i--;
  while (i > -1){
    printf("%s\n", (U[i]->name));
	   i--;
	   }
  }
  free_vertex(t);
  free(a);
  free(b);
  free(e);
  free(d);
  Vertex adfw = vertex_empty();
  Edge fawe = edge_empty();
  free(adfw);
  free(fawe);
  return 0;
}
