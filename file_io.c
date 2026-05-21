#include <stdio.h>
#include <string.h>
#include "main.h"
#include "file_io.h"
#include "format_zip.h"

File files[MAX_FILE_NUM];

static int check_file(FILE *fp)
{
    // 0为读取失败
    if(fp==NULL) return 0;
    unsigned int data=0;
    fread(&data,1,4,fp);
    rewind(fp);
    // 1为压缩包, 2为普通文件
    return (data==MAGICNUM)?1:2;
}

static void get_filename(const char *path,int idx)
{
    if(path==NULL||*path=='\0') return;
    const char *last_sep=strrchr(path,'\\');
    if(last_sep==NULL) return;

    const char *name=last_sep+1;
    int namelen=strlen(name);
    if(namelen==0)
    {
        puts("Fail to get file name, use \"default\" instead.");
        name="default";
        namelen=7;
    }
    if(namelen>MAX_NAME_LEN)
    {
        printf("File name is too long: %s",namelen);
        printf("Only the first %d characters will be used.",MAX_NAME_LEN);
        namelen=MAX_NAME_LEN;
    }
    strcpy(files[idx].name,name);
    /*puts(files[idx].name);*/
}

static const char *get_targetpath(const File *refer_file)
{
    // 获取输出目录
    static char output_dir[MAX_PATH_LEN+1]="";
    int pathlen=strlen(refer_file->path);
    int filelen=strlen(refer_file->name);
    memcpy(output_dir,refer_file->path,pathlen-filelen);
    output_dir[pathlen-filelen]='\0';
    // 获取输出文件名
    char output_name[MAX_NAME_LEN+1]="";
    memcpy(output_name,refer_file->name,filelen+1);
    strtok(output_name,".");
    printf("Input zip name (\"%s\" if empty): ",output_name);
    scanf("%100[^\n]",output_name); // 此处不便用宏常量,只好硬编码
    // 拼接最终路径
    strcat(output_name,".hzip");
    strcat(output_dir,output_name);
    /*puts(output_dir);*/
    return output_dir;
}

// 压缩: 将所有文件打包为一个zip
int file_zipping(const char (*paths)[MAX_PATH_LEN+1],int cnt)
{
    ushort file_cnt=0;
    for(int i=0;i<cnt;i++)
    {
        // 检查文件内容
        files[i].data=fopen(paths[i],"rb");
        int flag=check_file(files[i].data);
        if(flag==0||flag==1)
        {
            const char *log=flag?"Already a zip file":"Fail to read the file";
            printf("%s: %s\n",log,paths[i]);
            if(flag) fclose(files[i].data);
            files[i].data=NULL;
            continue;
        }
        // 通过复制路径筛除无效路径 (也便于解压时有一个数组专门存放路径)
        strcpy(files[file_cnt].path,paths[i]);
        // 获取文件名
        get_filename(paths[i],file_cnt++);
    }
    if(file_cnt==0)
    {
        puts("No files to process.");
        return 0;
    }
    // 以第一个有效文件路径为准, 输出压缩文件
    const char *output_path=get_targetpath(&files[0]);
    FILE *fp=fopen(output_path,"wb");
    int flag=output_zip(fp,file_cnt);
    if(flag) puts("Error");
    fclose(fp);
    return 0;
}

int file_unzipping(const char (*paths)[MAX_PATH_LEN+1],int cnt)
{
    // 解压缩: 分别解压各个压缩包文件
    // const char *log=flag?"Not a zip file":"Fail to read the file";
}