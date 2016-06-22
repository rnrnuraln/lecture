#include <stdio.h>


void reverse(int a[], int b){

int c;
int i;
for (i=0; i < b/2; i++){
 c = a[i];
 a[i] = a[b-i-1];
 a[b-i-1] = c;
}
    
}

int main(int argc, char *argv[])
{
    int a[5] = {0, 0, 0, 0, 0};
    int i;
 
    for (i = 0; i < 5; i++)
        scanf("%d", &a[i]);
 
    reverse(a, 5);
 
    for (i = 0; i < 5; i++)
        printf("%d\n", a[i]);
 
    return 0;
}