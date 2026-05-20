#ifndef HUFFMAN_H_INCLUDED
#define FORMAT_ZIP_H_INCLUDED

#define MAX_CHAR_NUM 256
#define MAX_CODE_LEN 256

typedef struct Node {
    uchar letter;
    size_t freq;
    struct Node *l,*r;
} Node;
typedef struct Heap {
    int len;
    Node *slot[2*MAX_CHAR_NUM-1];
} Heap;

void encode(void);

#endif