#include <stdio.h>
#include <string.h>
#include "main.h"
#include "file_io.h"
#include "format_zip.h"

char file_name[MAX_FILE_NUM][MAX_FILE_NAME+1];
FILE *file_data[MAX_FILE_NUM];

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

static const char *get_filename(const char *path)
{
    if(path==NULL||*path=='\0') return "";
    const char *last_sep=strrchr(path,'\\');
    if(last_sep==NULL) return "";
    return last_sep+1;
}

static char *get_targetdir(const char *refer_dir,int refer)
{
    // 获取输出目录
    static char output_dir[MAX_PATH_LEN+1]="";
    int dirlen=strlen(refer_dir);
    int filelen=strlen(file_name[refer]);
    memcpy(output_dir,refer_dir,dirlen-filelen);
    output_dir[dirlen-filelen]='\0';
    // 获取输出文件名
    char output_name[MAX_FILE_NAME+1]="";
    memcpy(output_name,file_name[refer],filelen+1);
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
int file_zipping(const char (*paths)[MAX_PATH_LEN],int cnt)
{
    int file_cnt=0,refer=0;
    for(int i=0;i<cnt;i++)
    {
        // 检查文件内容
        file_data[i]=fopen(paths[i],"rb");
        int flag=check_file(file_data[i]);
        if(flag==0||flag==1)
        {
            const char *log=flag?"Already a zip file":"Fail to read the file";
            printf("%s: %s\n",log,paths[i]);
            fclose(file_data[i]);
            file_data[i]=NULL;
            continue;
        }
        else file_cnt++;
        // 获取文件名
        const char *name=get_filename(paths[i]);
        if(*name=='\0')
        {
            puts("Fail to get file name, use \"default\" instead.");
            name="default";
        }
        int namelen=strlen(name);
        if(namelen>MAX_FILE_NAME)
        {
            printf("File name is too long: %s",namelen);
            printf("Only the first %d characters will be used.",MAX_FILE_NAME);
            namelen=MAX_FILE_NAME;
        }
        memcpy(file_name[i],name,namelen);
        file_name[i][namelen]='\0';
        // 标记第一个有效文件
        if(file_cnt==1) refer=i;
    }
    if(file_cnt==0)
    {
        puts("No files to process.");
        return 0;
    }
    // 输出压缩文件
    char *output_path=get_targetdir(paths[refer],refer);
    FILE *fp=fopen(output_path,"wb");
    int flag=output_zip(fp,cnt);
    if(flag) puts("Error");
    fclose(fp);
    return 0;
}

int file_unzipping(const char (*paths)[MAX_PATH_LEN],int cnt)
{
    // 解压缩: 分别解压各个压缩包文件
    // const char *log=flag?"Not a zip file":"Fail to read the file";
}