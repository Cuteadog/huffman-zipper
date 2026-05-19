#include <stdio.h>

typedef struct Node {
    char letter;
    int freq;
    struct Node *l,*r;
} Node;