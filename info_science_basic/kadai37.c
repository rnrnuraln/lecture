#include <stdio.h>
#define diff(x, y) (abs(x - y))

int main(){
  int x, y;
  scanf("%d%d", &x, &y);
  printf("%d", diff(x,y));
  return 0;
}
