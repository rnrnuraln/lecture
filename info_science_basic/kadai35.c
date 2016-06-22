#include <stdio.h>
#include <stdlib.h>
 
int main()
{
    void *p;
 
    p = malloc(1);
    if (p == NULL) exit(1);
    printf("%p\n", (char *) p + 1);

    printf("%p\n", (char *) p);
    printf("%p\n", (int *) p);
    printf("%p\n", (int *) p + 1);
    printf("%p\n", (int (*)[4]) p + 1);
    free(p);
 
    return 0;
}