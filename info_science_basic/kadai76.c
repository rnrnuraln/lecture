#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef char *String;
typedef struct tnode{
    String key;
    String value;
    struct tnode *left;
    struct tnode *right;
} *Btree;

Btree btree_empty()
{
  Btree a = (struct tnode *)malloc(sizeof(struct tnode));
  a->key = '\0';
  a->value = '\0';
  a->left = NULL;
  a->right = NULL;
  return a;
}
 
int btree_isempty(Btree t)
{
  if (t->key == '\0' && t->value == '\0')
    return 4;
  else
    return 0;
}
 
Btree btree_insert(String key, String val, Btree t)
{
  Btree a = (struct tnode *)malloc(sizeof(struct tnode));
  String b = (char *)malloc(50);
  String c = (char *)malloc(50);
  strcpy(b, key);
  strcpy(c, val);
  a->key = b;
  a->value = c;
  a->left = btree_empty();
  a->right = btree_empty();
  if (btree_isempty(t) == 4){
    return a;
  }
  else if (strcmp(key, t->key) < 0){
    if (btree_isempty(t->left)){
       t->left = a;
     return t;
      }
      else{
       t->left = btree_insert(key, val, t->left);
       free(a);
       return t;
      }
    }
    else if (strcmp(key, t->key) > 0){
      if (btree_isempty(t->right)){
	  t->right = a;
	  return t;
      }
      else{
	  t->right = btree_insert(key, val, t->right);
	  free(a);
	  return t;
      }
      }
      else{
	t->key = key;
	t->value = val;
	free(a);
	return t;
      }
}

Btree deletemin(Btree t){
  if (btree_isempty(t->left))
    if (btree_isempty(t->right))
      return btree_empty();
    else
      return t->right;
  else{
    t->left = deletemin(t->left);
    return t;
  }
}

String min_key(Btree t){
  while (! btree_isempty(t->left))
    t = t->left;
  return t->key;

}

String min_val(Btree t){
  while (! btree_isempty(t->left))
    t = t->left;
  return t->value;
}

 
Btree btree_delete(String key, Btree t)
{
  if (btree_isempty(t))
    return t;
  else if (strcmp(key, t->key) < 0){
    if (btree_isempty(t->left))
      return t;
    else{
      t->left = btree_delete(key, t->left);
      return t;
    }}
    else if (strcmp(key, t->key) > 0){
      if (btree_isempty(t->right))
	return t;
      else{
	t->right = btree_delete(key, t->right);
	return t;
      }}
    else if ((btree_isempty(t->right)) && (btree_isempty(t->left))){
      free(t->key);
      free(t->value);
      free(t);
	return btree_empty();
    }
    else if (btree_isempty(t->left)){
      Btree p = t->right;
      free(t->key);
      free(t->value);
      free(t);
	return p;
    }
    else if (btree_isempty(t->right)){
      Btree p = t->left;
      free(t->key);
      free(t->value);
      free(t);
	return p;
    }
      else{
	Btree p;
        p->key = min_key(t->right);
	p->value = min_val(t->right);
	p->right = deletemin(t->right);
	free(t->key);
	free(t->value);
	free(t);
	return p;
      }
}
 
String btree_search(String key, Btree t)
{
  if (btree_isempty(t))
    return NULL;
  else if (strcmp(key, t->key) < 0){
    if (btree_isempty(t->left))
      return NULL;
    else
      return btree_search(key, t->left);
  }
  else if (strcmp(key, t->key) > 0){
    if (btree_isempty(t->right))
      return NULL;
    else
      return btree_search(key, t->right);
  }
  else
    return t->value;
}

    

void free_btree(Btree t)
{
  if ((btree_isempty(t->right)) && (btree_isempty(t->left))){
    free(t->key);
    free(t->value);
    free(t);
  }
  else if (btree_isempty(t->left)){
    Btree a;
    a = t->right;
    free(t->key);
    free(t->value);
    free(t);
    free_btree(a);
  }
  else if (btree_isempty(t->right)){
    Btree b;
    b = t->left;
    free(t->key);
    free(t->value);
    free(t);
    free_btree(b);
  }
  else{
    Btree c;
    Btree d;
    c = t->left;
    d = t->right;
    free(t->key);
    free(t->value);
    free(t);
    free_btree(c);
    free_btree(d);
  }
}

int main()
{
  Btree t = (struct tnode *)malloc(sizeof(struct tnode));
  t = btree_empty();
  String s = (char *)malloc(50);
  String a = (char *)malloc(50);
  String b = (char *)malloc(50);
  while (scanf("%49s ", s) != EOF){
    if (strcmp(s, "insert") == 0){
      scanf("%49s ", a);
      scanf("%49s\n", b);
      t = btree_insert(a, b, t);
    }
    else if (strcmp(s, "delete") == 0){
      scanf("%49s\n", a);
      t = btree_delete(a, t);
    }
    else if (strcmp(s, "search") == 0){
      scanf("%49s\n", a);
      if (btree_search(a, t) == NULL)
	printf("(not found)\n");
      else
	printf("%s\n", (btree_search(a, t)));
    }
    else if (strcmp(s, "quit") == 0)
      break;
    else{
      printf("(unknown command)\n");
    }
  }

  free_btree(t);
  free(a);
  free(b);
  free(s);

return 0;

}
