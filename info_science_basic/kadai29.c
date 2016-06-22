#include <stdio.h>

void copy(char *a, char *b){
  int i = 0;
  while (*(b+i)){
    *(a + i) = *(b + i);
    i = i + 1;  
}
  a[i] = '\0';
    }

void reverse(char a[][10], int k)
{
    int i;
    char tmp[10] = "123456789";

    for (i = 0; i < (k - 1) / 2; i++) {
      copy(tmp, a[i]);
      copy(a[i], a[k-i-1]);
      copy(a[k-i-1], tmp);
      
    }
}

int main(int argc, char *argv[])
{
    char a[5][10] = {"", "", "", "", ""};
    int i;

    for (i = 0; i < 5; i++)
        scanf("%9s", a[i]);

    reverse(a, 5);

    for (i = 0; i < 5; i++)
        printf("%s\n", a[i]);

    return 0;
}