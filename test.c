#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "main.h"

#define CHUNK 8192
#define MAX_CHAR_NUM 256
#define MAX_CODE_LEN 256
typedef struct codeTable {
    uchar str[MAX_CODE_LEN+1];  // 霍夫曼编码串
    uchar len;                  // 编码长度 (只占用1字节)
} codeTable;
codeTable code_table[MAX_CHAR_NUM];

static void write_encoded_str(FILE *op,FILE *fp)
{
    clock_t start = clock();

    uchar istr[CHUNK]="",ostr[CHUNK]="";
    size_t ilen=0,olen=0;   // olen表示二进制串长
    ilen=fread(istr,1,10,fp);
    {
        // 逐字符编码
        for(size_t i=0;i<ilen;i++)
        {
            const codeTable *code=&code_table[istr[i]];
            for(int j=(code->len+olen%CHAR_BIT)/CHAR_BIT;j>0;j--)
            {
                ostr[olen/CHAR_BIT+j] = code->str[j] >> (olen%CHAR_BIT);
                ostr[olen/CHAR_BIT+j] |= code->str[j-1] << (CHAR_BIT-olen%CHAR_BIT);
            }
            ostr[olen/CHAR_BIT] |= code->str[0] >> (olen%CHAR_BIT);
            olen += code->len;
        }
        // 写入编码串
        fwrite(ostr,1,olen/CHAR_BIT,op);
        // 重置编码串 (如果最后一个字节没填满, 则接着下一个循环填)
        if(olen%CHAR_BIT)
        {
            ostr[0]=ostr[olen/CHAR_BIT];
            memset(ostr+1,0,CHUNK-1);
        }
        else memset(ostr,0,CHUNK);
        olen%=CHAR_BIT;
    }
    if(olen%CHAR_BIT) fwrite(&ostr[0],1,1,op);

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("CPU 用时 %.4f 秒\n", elapsed);
}

int main(void)
{
    size_t a=2593907899;
    size_t b=2596712704;
    printf("a/b=%lf\n",1.*a/b);

    printf("%d\n", CHAR_BIT);
    code_table['a'].str[0]= 1 << 7; // 1
    code_table['a'].len = 1;
    code_table['b'].str[0]= 1 << 6; // 01
    code_table['b'].len = 2;
    code_table['c'].str[0]= 1 << 5; // 001
    code_table['c'].len = 3;

    FILE *ip=stdin;
    FILE *op=fopen("..\\note.hzip","wb");
    write_encoded_str(op,ip);   // 输入aaabbbccca
    return 0;
}