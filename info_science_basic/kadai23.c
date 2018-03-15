#include <stdio.h>
 
int main(int argc, char *argv[])
{
    char s[100] = "";
 
    scanf("%99s", s);
 
    if (s[0] == 'y' && s[1] == 'e' && s[2] == 's' && s[3] == '\0')
        puts("OK");
    else
        puts("NG");
 
    return 0;
}