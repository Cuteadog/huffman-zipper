#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "main.h"

int *static_test(void)
{
    // 只会在第一次调用时清0！
    static int i=0;
    printf("inner: %d\n",i++);
    return &i;
}

struct Node {
    uchar letter;
    size_t freq;
    struct Node *l,*r;
} x,y,z;

int main(void)
{
    size_t a=2593907899;
    size_t b=2596712704;
    printf("a/b=%lf\n",1.*a/b);

    int *p=NULL;
    for(int n=0;n<5;n++)
    {
        p=static_test();
        printf("outside: %d\n",(*p)++);
    }

    x.letter='a';
    x.freq=22;
    x.l=&y;
    x.r=&z;
    memset(&x,0,sizeof(x));
    printf("size of 'Node': %zu\n",sizeof(x));
    printf("letter: %c\n",x.letter);
    printf("lchild: %p\n",x.l);
    printf("rchild: %p\n",x.r);
    return 0;
}