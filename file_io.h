#ifndef FILE_IO_H_INCLUDED
#define FILE_IO_H_INCLUDED
#include "main.h"

typedef struct File {
    char name[MAX_NAME_LEN+1];
    char path[MAX_PATH_LEN+1];
    FILE *data;
} File;
extern File files[MAX_FILE_NUM];

int file_zipping(const char (*paths)[MAX_PATH_LEN+1],int cnt);
int file_unzipping(const char (*paths)[MAX_PATH_LEN+1],int cnt);

#endif