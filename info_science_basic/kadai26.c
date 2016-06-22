#include<stdio.h>

void sort3(int *p0, int *p1, int *p2)
{
int a;
if (*p1 <= *p0){
 a = *p0;
 *p0 = *p1;
 *p1 = a;}
if (*p2 <= *p1){
 a = *p1;
 *p1 = *p2;
 *p2 = a;}
if (*p1 <= *p0){
 a = *p0;
 *p0 = *p1;
 *p1 = a;}

}

int main(int argc, char *argv[])
{
    int x = 0, y = 0, z = 0;
 
    scanf("%d", &x);
    scanf("%d", &y);
    scanf("%d", &z);
 
    sort3(&x, &y, &z);
 
    printf("%d\n%d\n%d\n", x, y, z);
 
    return 0;
}