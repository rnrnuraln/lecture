#include <stdio.h>


void mul(int l[][3], int r[][2], int a[][2], int n)
{
    int i;
    for (i = 0; i < n; i++){
     a[i][0] = l[i][0] * r[0][0] + l[i][1] * r[1][0] + l[i][2] * r[2][0];
     a[i][1] = l[i][0] * r[0][1] + l[i][1] * r[1][1] + l[i][2] * r[2][1]; 
}


/* �s��`a'���s��`l'�ƍs��`r'�̐ς�\���悤�ɂȂ�B */
    /* `n'��`l'��`a'�̍s����\���B */
    /* `r'�̍s���͐ς̒�`��`l'�̃T�C�Y���猈�܂�̂ŁA�����ɂ͊܂܂�Ȃ��B */
}



int main()

{ 
  int a[2][3];

  int b[3][2];

  int c[2][2];

  int i;
  
int j;
  
int l;

  int k;
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
 
 
mul(a, b, c, 2);

printf("%d %d\n%d %d", c[0][0], c[0][1], c[1][0], c[1][1]);
return 0;
}