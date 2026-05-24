#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"

int main(void)
{
    FILE *fp=fopen("D:\\test\\test.exe","w");
    fwrite("hello",1,5,fp);
    fpos_t pos=1;
    fgetpos(fp,&pos);
    printf("pos: %lld\n",pos);

    size_t a=2593907899;
    size_t b=2596712704;
    printf("a/b=%lf\n",1.*a/b);

    return 0;
}