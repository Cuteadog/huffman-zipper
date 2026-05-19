#ifndef FORMAT_ZIP_H_INCLUDED
#define FORMAT_ZIP_H_INCLUDED

static uint MAGICNUM = 0x5a486d79; // ymHZ -> Huffman Zipper by ym
static ushort VERSION = 0x0001;
int output_zip(FILE *fp,int cnt,ushort file_cnt);

#endif