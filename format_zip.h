#ifndef FORMAT_ZIP_H_INCLUDED
#define FORMAT_ZIP_H_INCLUDED
#include "huffman.h"

typedef struct contentLen {
    size_t before;  // 压缩前
    size_t after;   // 压缩后
} contentLen;
typedef struct freqTable {
    size_t sum;                 // 总字符频度
    size_t file[MAX_FILE_NUM];  // 各文件的字符频度
} freqTable;
typedef struct codeTable {
    uchar str[MAX_CODE_LEN+1];  // 霍夫曼编码串
    uchar len;                  // 编码长度 (只占用1字节)
} codeTable;
extern contentLen file_size[MAX_FILE_NUM];
extern freqTable freq_table[MAX_CHAR_NUM];
extern codeTable code_table[MAX_CHAR_NUM];

static const uint MAGICNUM = 0x5a486d79; // ymHZ -> Huffman Zipper by ym
static const ushort VERSION = 0x0001;
int output_zip(FILE *fp,int cnt,ushort file_cnt);

#endif