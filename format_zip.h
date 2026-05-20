#ifndef FORMAT_ZIP_H_INCLUDED
#define FORMAT_ZIP_H_INCLUDED
#include "huffman.h"

struct freqTable {
    size_t freq;
    double average;
} ;
extern size_t content_len[MAX_FILE_NUM];
extern struct freqTable freq_table[MAX_CHAR_NUM];

static const uint MAGICNUM = 0x5a486d79; // ymHZ -> Huffman Zipper by ym
static const ushort VERSION = 0x0001;
int output_zip(FILE *fp,int cnt,ushort file_cnt);

#endif