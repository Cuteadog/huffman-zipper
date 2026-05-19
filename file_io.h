#ifndef FILE_IO_H_INCLUDED
#define FILE_IO_H_INCLUDED
#include "main.h"

extern char file_name[MAX_FILE_NUM][MAX_FILE_NAME+1];
extern FILE *file_data[MAX_FILE_NUM];
int file_zipping(const char (*paths)[MAX_PATH_LEN],int cnt);
int file_unzipping(const char (*paths)[MAX_PATH_LEN],int cnt);

#endif