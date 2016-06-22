#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct lnode {
    int data;
    struct lnode *next;
} *List;
 
List stack_push(List l, int a){
  List p;
  if ((p = (struct lnode *)malloc(sizeof(struct lnode))) == NULL) {
    printf("malloc error\n");
    exit(EXIT_FAILURE);
  }
  p->data = a;
  p->next = l;
  return p;
}
int stack_read(List l){
  return l->data;}

List calc_add(List l){
  List s;
   if ((s = (struct lnode *)malloc(sizeof(struct lnode))) == NULL) {
    printf("malloc error\n");
    exit(EXIT_FAILURE);
  }
   int a = (l->data) + ((l->next)->data);
   s = l->next;
   free(l);
   s->data = a;
  return s;
}

  
List calc_sub(List l){
  List s;
  if ((s = (struct lnode *)malloc(sizeof(struct lnode))) == NULL) {
    printf("malloc error\n");
    exit(EXIT_FAILURE);
  }
  int a = ((l->next)->data) - (l->data);
  s = l->next;
  free(l);
  s->data = a;
  return s;
}

List calc_mul(List l){
  List s;
  if ((s = (struct lnode *)malloc(sizeof(struct lnode))) == NULL) {
    printf("malloc error\n");
    exit(EXIT_FAILURE);
  }
  int a = (l->data) * ((l->next)->data);
  s = l->next;
  free(l);
  s->data = a;
  return s;
}

void list_free(List l){
   List s;
   while ((l->next) != NULL ) {
      s = l->next;
      free(l);
      l = s;
    }
 }


int main(){
  char *s = (char *)malloc(10);
  List l;
  l = (struct lnode *)malloc(sizeof(struct lnode));
  l->data = 0;
  l->next = NULL;
  while (scanf("%s", s) != EOF){
    if (strcmp(s,"=") == 0)
      break;
    else if (strcmp(s, "+") == 0)
      l = calc_add(l);
    else if (strcmp(s, "-") == 0)
      l = calc_sub(l);
    else if (strcmp(s, "*") == 0)
      l = calc_mul(l);
    else if (strcmp(s, "0") == 0)
      l = stack_push(l, 0);
    else if (atoi(s) != 0)
      l = stack_push(l, atoi(s));
    else
      puts("unknown command");
  }
  printf("%d", stack_read(l));
  list_free(l);
  return 0;
}
