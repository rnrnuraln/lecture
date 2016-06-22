#include <stdio.h>


int main()

{ 
  int a[2][3];

  int b[3][2];

  int c[2][2];

  int i;
  
int j;
  
int l;

  int k;

  int m;
  
  int n;

  for (i = 0; i < 2; i++){
    for (j = 0; j < 3; j++){
      if (j < 2)
	scanf("%d ", &a[i][j]);
       else
	 scanf("%d\n", &a[i][j]);}}

  for (l = 0; l < 3; l++){
    for (k = 0; k < 2; k++){
      if (k < 1)
	scanf("%d ", &b[l][k]);
      else if (k == 1 && l == 2)
        scanf("%d", &b[l][k]);
      else
        scanf("%d\n", &b[l][k]);}}
 
 for (m = 0; m < 2; m++){
  
  for (n = 0; n < 2; n++){
   
    c[m][n] = (a[

m][0]) * (b[0][n]) + (a[m][1]) * (b[1][n]) + (a[m][2]) * (b[2][n]);
}}

printf("%d %d\n%d %d", c[0][0], c[0][1], c[1][0], c[1][1]);
return 0;
}