#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *String;
typedef struct lnode{
  String data;
  struct lnode *next;
}*List;
 
int list_isempty(List l)
{
    /* `l'が空リストなら0以外を、そうでないなら0を返す。 */

  if ((l->data == '\0') && (l->next == NULL))
    return 4;
  else
    return 0;
    
}

List list_cons(String s, List l)
{
    /*
     * リスト`l'の先頭に文字列`s'を要素として追加したリストを返す。
     * 必要なメモリはmallocやcallocで確保する。
     */
   List p;
   if ((p = (struct lnode *)malloc(sizeof(struct lnode))) == NULL) {
    printf("malloc error\n");
    exit(EXIT_FAILURE);
  }
   if ((p->data = (char *)malloc(100)) == NULL) {
     printf("malloc error\n");
     exit(EXIT_FAILURE);
   }
   strcpy(p->data, s);
  p->next = l;
  l = p;
  return l;
  
}
 
void list_free(List l)
{
    List s;
    /*
     * リスト`l'の使用しているメモリを全て解放する。
     * この関数を呼び出して以降、`l'を使用してはいけない。
     */
    while (! list_isempty(l)) {
      s = l->next;
      free(l->data);
      free(l);
      l = s;
    }
}

void list_putl(List l)
{
  while (! list_isempty(l)) {
      printf("%s\n", l->data);
        l = l->next;
    }
}

List little(List l, String s)
{
  if (list_isempty(l) == 4)
    return l;
  else{
    if (strcmp(l->data, s) < 0)
      return list_cons(l->data, little(l->next, s));
    else
      return little(l->next, s);
  }
}

List big(List l, String s)
{
  if (list_isempty(l) == 4 )
    return l;
  else{
    if (strcmp(l->data, s) < 0)
      return big(l->next, s);
    else
      return list_cons(l->data, big(l->next, s));
  }
}

List append(List a, List b)
{
  if (list_isempty(a) == 4)
    return b;
  else
    return list_cons(a->data, append(a->next, b));
}
  

List sort(List l)
{
  if (list_isempty(l) == 4)
      return l;
  else {if (list_isempty(l->next) == 4)
      return l;
    else
      {
	if (list_isempty(little(l, l->data)) == 4)
	  return list_cons(l->data, sort(l->next));
	else
	  return append(sort(little(l, l->data)), sort(big(l, l->data)));
	    }
  }
}
 
int main(int argc, char *argv[])
{
    List l;
 
    /* 変数lに空リストを代入。 */
    l = (struct lnode *)malloc(sizeof(struct lnode));
      l->data = '\0';
    l->next = NULL;
 
    /*
     * 以下の操作をEOFまで繰り返す。
     * ・文字列を読み込む。
     * ・その文字列をlist_consでリストの先頭に追加して、lを更新する。
     */
    String s = (char *)malloc(100);
    while(scanf("%s", s) != EOF)
      l = list_cons(s, l);
    l = sort(l);
    list_putl(l);
    list_free(l);
    
    free(s);
    return 0;
}
