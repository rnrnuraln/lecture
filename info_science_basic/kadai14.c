#include <stdio.h>


int reverse(int a[], int b){

int c;
for (i=0; i < b/2; i++){
 c = a[i];
 a[i] = a[b-i];
 a[b-i] = c;
}
    
return a;
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