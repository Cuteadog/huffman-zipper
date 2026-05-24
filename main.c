#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "file_io.h"

int main(int argc,char *argv[])
{
    // 使用说明
    if(argc==1 || strcmp(argv[1],"-help")==0)
    {
        puts("");
        puts("Usage:");
        puts("  zipper [options] <path(s)>");
        puts("");
        puts("Options:");
        puts("  -help    Show help.");
        puts("  -en      Compress several files into one .hzip file.");
        puts("  -de      Unzip several .hzip files separately.");
        puts("");
        puts("Hint:");
        printf("1. A maximum of %d files can be processed at a time.\n",MAX_FILE_NUM);
        puts(  "2. In -en mode, output uses the first valid file's directory.");
        puts("");
        return 0;
    }

    // 获取文件完整路径
    char paths[MAX_FILE_NUM][MAX_PATH_LEN+1]={0};
    int cnt=0;
    if(argc-2>MAX_FILE_NUM) puts("Too many files! Only the front files will processed.");
    for(int i=0;i<argc-2&&i<MAX_FILE_NUM;i++)
    {
        char *abspath=_fullpath(paths[cnt],argv[i+2],MAX_PATH_LEN);
        if(abspath==NULL) printf("Invalid path: %s\n",argv[i+2]);
        else cnt++;
    }
    if(cnt==0)
    {
        puts("No files to process.");
        return 0;
    }

    // 分类处理
    if(strcmp(argv[1],"-en")==0)
    {
        puts("Compressing...\n");
        file_zipping(paths,cnt);
    }
    else if(strcmp(argv[1],"-de")==0)
    {
        for(int i=0;i<cnt;i++)
        {
            printf("Unzipping...(%d/%d)\n",i+1,cnt);
            file_unzipping(paths[i]);
        }
    }
    else puts("Parameter error. Use \"-help\" for help.");
    return 0;
}