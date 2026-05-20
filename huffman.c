#include <stdio.h>
#include <string.h>
#include "main.h"
#include "format_zip.h"
#include "huffman.h"

Heap heap;
Node nodes[2*MAX_CHAR_NUM-1];
uchar codetable[MAX_CHAR_NUM][MAX_CODE_LEN+1];

static int nodes_init(void)
{
    int cnt=0;
    for(int i=0;i<MAX_CHAR_NUM;i++) if(freq_table[i].freq)
    {
        nodes[cnt].letter=(uchar)i;
        nodes[cnt].freq=freq_table[i].freq;
        cnt++;
    }
    return cnt;
}

static void nodes_swap(Node **a,Node **b)
{
    Node *c=*a;
    *a=*b;
    *b= c;
}

static void nodes_add(Node *l,Node *r,int cnt)
{
    Node *new_node = &nodes[cnt];
    new_node->freq = l->freq + r->freq;
    new_node->l = l;
    new_node->r = r;
}

static void heapify_up(int idx)
{
    while(idx>0)
    {
        int parent=(idx-1)/2;
        if(heap.slot[parent]->freq > heap.slot[idx]->freq)
        {
            nodes_swap(&heap.slot[parent],&heap.slot[idx]);
            idx=parent;
        }
        else break;
    }
}

static void heapify_down(int idx)
{
    while(idx<heap.len)
    {
        int l=2*idx+1, r=2*idx+2, min=idx;
        if(l<heap.len && heap.slot[min]->freq>heap.slot[l]->freq) min=l;
        if(r<heap.len && heap.slot[min]->freq>heap.slot[r]->freq) min=r;
        if(min!=idx)
        {
            nodes_swap(&heap.slot[idx],&heap.slot[min]);
            idx=min;
        }
        else break;
    }
}

static void heapify_insert(Node *node)
{
    heap.slot[heap.len]=node;
    heapify_up(heap.len);
    heap.len++;
}

static Node *heapify_extract(void)
{
    Node *min_node=heap.slot[0];
    heap.slot[0]=heap.slot[heap.len-1];
    heap.len--;
    heapify_down(0);
    return min_node;
}

static Node *heapify_build(void)
{
    int cnt=nodes_init();
    for(Node *i=nodes;i-nodes<cnt;i++) heapify_insert(i);
    while(heap.len>1)
    {
        Node *lchild=heapify_extract();
        Node *rchild=heapify_extract();
        nodes_add(lchild,rchild,cnt);
        heapify_insert(&nodes[cnt]);
        cnt++;
    }
    return heapify_extract();
}

static void form_code(Node *root,char *codestr,int depth)
{
    if(root==NULL||depth>=MAX_CODE_LEN) return;
    if(root->l==NULL && root->r==NULL)
    {
        codestr[depth]='\0';
        memcpy(codetable[root->letter],codestr,depth+1);
        return;
    }
    codestr[depth]='0';
    form_code(root->l,codestr,depth+1);
    codestr[depth]='1';
    form_code(root->r,codestr,depth+1);
}

void encode()
{
    Node *root=heapify_build();
    char codestr[MAX_CODE_LEN]="";
    form_code(root,codestr,0);
}