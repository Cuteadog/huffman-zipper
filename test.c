#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "main.h"

int main(void)
{
    WIN32_FIND_DATA fd;
    HANDLE h=FindFirstFile("\\*",&fd);
    if(h != INVALID_HANDLE_VALUE)
    {
        do {
            printf("%s\n", fd.cFileName);
        } while(FindNextFile(h, &fd));
        FindClose(h);
    }
    return 0;
}