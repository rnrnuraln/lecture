#include <stdio.h>
#include <stdlib.h>
 
struct student{
    char *name;
    int hp;
    int mp;
};
 
int main()
{
    struct student *p;
    p = (struct student *)malloc(sizeof(struct student));
    p->name = (char *)malloc(sizeof(10));
    scanf("%s", p->name);

    scanf("%d%d", &p->hp, &p->mp);
 
    if (p->mp >= 10) {
        p->hp += 100;
        p->mp -= 10;
    }
 
    printf("%s\n%d\n%d\n", p->name, p->hp, p->mp);
 
    free(p->name);
    free(p);
 
    return 0;
}