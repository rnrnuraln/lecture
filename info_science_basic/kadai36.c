#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
char *substr(char *s)
{
    int i = 0;
    char *buf;
 
    buf = malloc(strlen(s) + 1);
    strcpy(buf, s);
    while (buf[i] != '\0') {
        if (buf[i] == 'a')
            break;
        i++;
    }
 
    return buf + i;
}
 
int main()
{
    char buf[100] = "";
    char *s[5];
    int i;
 
    for (i = 0; i < 5; i++) {
        scanf("%99s", buf);
        s[i] = substr(buf);
        buf[0] = '\0';
    }
 
    for (i = 4; 0 <= i; i--) {
        printf("%s\n", s[i]);
        /* 以下の行をコメントアウトするとエラーになる。 */
        /* free(s[i]); */
    }
 
    return 0;
}