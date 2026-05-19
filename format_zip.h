#ifndef FORMAT_ZIP_H_INCLUDED
#define FORMAT_ZIP_H_INCLUDED
#include "huffman.h"

struct letter_count {
    size_t contentlen;
    size_t table[MAX_CHAR_NUM];
    double freq[MAX_CHAR_NUM];
} ;
extern struct letter_count file_letter_count[MAX_FILE_NUM];
extern double letter_average_freq[MAX_CHAR_NUM];

static uint MAGICNUM = 0x5a486d79; // ymHZ -> Huffman Zipper by ym
static ushort VERSION = 0x0001;
int output_zip(FILE *fp,int cnt,ushort file_cnt);

#endif