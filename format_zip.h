#ifndef FORMAT_ZIP_H_INCLUDED
#define FORMAT_ZIP_H_INCLUDED
#include "huffman.h"

typedef struct freqTable {
    size_t sum;                 // 总字符频度
    size_t file[MAX_FILE_NUM];  // 各文件的字符频度
} freqTable;
typedef struct codeTable {
    uchar str[MAX_CODE_LEN+1];  // 霍夫曼编码串
    uchar len;                  // 编码长度 (只占用1字节)
    uchar letter;               // 对应的字母 (供解压使用)
} codeTable;
extern freqTable freq_table[MAX_CHAR_NUM];
extern codeTable code_table[MAX_CHAR_NUM];

static const uint MAGICNUM = 0x7a684d59; // YMhz -> huffman zipper by YM
static const ushort VERSION = 0x0001;
void output_zip(FILE *fp,ushort file_cnt);
int decode_zip(FILE *zip,ushort file_cnt,const char *output_dir);

#endif