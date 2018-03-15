#include <stdio.h>
 
int main(int argc, char *argv[])
{
    char s1[100];
    char s2[100];
    scanf("%99s", s1);
    
    int len = 0;
     while(s1[len])
    len++;
   
    int j;

    for (j = 0; j < len; j++)
     s2[j] = s1[(len-j-1)];
    s2[len] = '\0';
    
    
 
    printf("%s\n", s2);
 
    return 0;
}