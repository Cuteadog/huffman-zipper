#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "main.h"

int main(void)
{
    char istr[100]="",ostr[100]="";
    fgets(istr,100,stdin);
    _fullpath(ostr,istr,MAX_PATH_LEN);
    puts(ostr);

    WIN32_FIND_DATA fd;
    char str[100]="";
    puts("输入通配符：");
    scanf("%[^\n]",str);
    HANDLE h=FindFirstFile(str,&fd);
    if(h != INVALID_HANDLE_VALUE)
    {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                printf("Long: %s, Short: %s\n", fd.cFileName, fd.cAlternateFileName);
            }
        } while(FindNextFile(h, &fd));
        FindClose(h);
    }
    return 0;
}