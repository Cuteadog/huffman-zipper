#include <stdio.h>
#include <string.h>
#include <limits.h>

#define WIDTH 10
#define STR(x) #x
#define XSTR(x) STR(x)
#define FMT "%" XSTR(WIDTH) "[^\n]"

int main(void)
{
    printf("%d\n", CHAR_BIT);

    char str[100]="";
    scanf(FMT,str);
    puts(str);
    return 0;
}