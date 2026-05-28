#ifndef FILE_IO_H_INCLUDED
#define FILE_IO_H_INCLUDED
#include "main.h"

typedef struct File {
    char name[MAX_NAME_LEN+1];
    char path[MAX_PATH_LEN+1];
    FILE *data;
    size_t len1;    // 压缩前长度 (bytes)
    size_t len2;    // 压缩后长度 (bits)
} File;
extern File files[MAX_FILE_NUM];

void get_filedir(const char *path,char *name,char *filedir);
void file_zipping(const char (*paths)[MAX_PATH_LEN+1],int cnt);
void file_unzipping(const char *path);

#endif