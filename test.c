#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int check_file(FILE *fp)
{
    if(fp==NULL)
    {
        puts("Fail to read the file.");
        return 0;
    }
    int data=0;
    fread(&data,1,4,fp);rewind(fp);
    if(data==0x04034b50) {puts("Oh,zip!");return 1;}
    return 2;
}

int input_file(const char *path)
{
    FILE *fp=fopen(path,"rb");
    char flag=check_file(fp);
    if(flag==0) return 0;

    char data[1024]="";
    size_t datalen;
    while((datalen=fread(data,1,sizeof(data),fp))>0)
    {
        puts(data);
    }
    fclose(fp);
    return 1;
}

const char *get_filename(const char *path)
{
    if(path==NULL||*path==0) return "";
    const char *last_sep=strrchr(path,'\\');
    if(last_sep==NULL) return NULL;
    return last_sep+1;
}

void output(const char *input_path)
{
    char path[MAX_PATH_LEN]="";
    memcpy(path,input_path,sizeof(input_path)+1);
    char *output_dir=strtok(path,"\\");
    FILE *fp=fopen(strcat(output_dir,"test.txt"),"ab");
    fwrite("hello",1,6,fp);
    fclose(fp);
}

int main(void)
{
    //output("D:\\note.txt");
    puts(_fullpath(NULL,"hey.txt",0));
    puts(get_filename("D:\\hey.txt"));
    //printf("MAX_FILE_NUM: %d\n",MAX_FILE_NUM);
    //for(int i=0;i<128;i++)printf("%x:%c\n",i,i);
    //if(input_file("..\\note.txt"))printf("True");
    return 0;
}