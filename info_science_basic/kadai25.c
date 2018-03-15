#include <stdio.h>


void swap(int z[])
{
    int t;
    t = z[0];
    z[0] = z[1];
    z[1] = t;
}
 
int main(int argc, char *argv[])
{
    int x=0;
    int y=0;
    int z[2];
 
    scanf("%d\n%d", &x, &y);
    z[0] = x;
    z[1] = y;
    swap(z);
 
    printf("%d\n", z[0]);
    printf("%d\n", z[1]);
 
    return 0;
}