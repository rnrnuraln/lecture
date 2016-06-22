#include <stdio.h>

int gcd(int x,int y){
int r;
int s;
  if (x < y){
   r = x; x = y; y = r;}
while ( x % y != 0){
  s = x % y ;
  x = y;
  y = s;}
return y;
}

int main(int argc, char *argv[])
{
    int x = 1, y = 1;
 
    scanf("%d%d", &x, &y);
 
    printf("%d\n", gcd(x, y));
 
    return 0;
}