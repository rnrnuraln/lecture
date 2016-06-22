#include <stdio.h>

char *getstr()
{
    static int j = 0;
    static char buf[5][100] = {"","","","",""};
    int x;
 
    x = scanf("%99s", buf[j]);
    j++;
    return (x == EOF) ? NULL : buf[j-1];
}
 
int main(int argc, char *argv[])
{
    char *s[5];
    int i;
 
    for (i = 0; i < 5; i++) {
        s[i] = getstr();
    }
 
    for (i = 4; 0 <= i; i--) {
        if (s[i] != NULL)
            printf("%s\n", s[i]);
    }
 
    return 0;
}