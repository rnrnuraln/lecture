#include <stdio.h>


int main()

{ 
  int a[4][4];
  int i;
  
  for (i = 0; i < 3; i++)
    scanf("%d %d %d %d\n", &a[i][0], &a[i][1], &a[i][2], &a[i][3]);
  scanf("%d %d %d %d", &a[3][0], &a[3][1], &a[3][2], &a[3][3]);
  printf("%d", a[0][0] + a[1][1] + a[2][2] + a[3][3]);
return 0;
}