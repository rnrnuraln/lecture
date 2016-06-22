#include <stdio.h>
 
int main(int argc, char *argv[])
{
    char s1[100] = "";
    char s2[100] = "";
      scanf("%99s\n%99s", s1, s2);
    int i;
    int j = 0;
      for (i = 0; i < 100; i++){
        if (s1[i] != s2[i])
          j++;}
    if (j == 0)
       puts("OK");
    else
       puts("NG");
 
    return 0;
}