#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "main.h"
#include "file_io.h"

int main(int argc,char *argv[])
{
    // 使用说明
    if(argc==1 || strcmp(argv[1],"-help")==0)
    {
        puts("");
        puts("Usage:");
        puts("  zipper <options> [path(s)]");
        puts("");
        puts("Options:");
        puts("  -help    Show help.");
        puts("  -en      Compress several files into one .hzip file.");
        puts("  -de      Unzip several .hzip files separately.");
        puts("");
        puts("Hint:");
        printf("1. A maximum of %d files can be processed at a time.\n",MAX_FILE_NUM);
        puts(  "2. In -en mode, output uses the first valid file's directory.");
        puts(  "3. If the zip file contains only one file, extract it to the same directory; otherwise, extract it to a folder.");
        puts(  "4. Wildcard allowed.");
        puts("");
        return 0;
    }

    // 获取文件完整路径
    char paths[MAX_FILE_NUM][MAX_PATH_LEN+1]={0};
    int cnt=0;
    if(argc-2>MAX_FILE_NUM) puts("Too many files! Only the front files will processed.");
    for(int i=0;i<argc-2&&cnt<MAX_FILE_NUM;i++)
    {
        char *abspath=_fullpath(paths[cnt],argv[i+2],MAX_PATH_LEN);
        if(abspath==NULL) printf("Invalid path: %s\n",argv[i+2]);
        else if(strchr(argv[i+2],'*')!=NULL||strchr(argv[i+2],'?')!=NULL)
        {
            WIN32_FIND_DATA fd;
            HANDLE h=FindFirstFile(argv[i+2],&fd);
            if(h!=INVALID_HANDLE_VALUE)
            {
                char dir[MAX_PATH_LEN]="";
                get_filedir(abspath,NULL,dir);
                do {
                    if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                        strcpy(paths[cnt],dir);
                        strcat(paths[cnt],fd.cFileName);
                        cnt++;
                    }
                } while(cnt<MAX_FILE_NUM&&FindNextFile(h, &fd));
                FindClose(h);
            }
        }
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