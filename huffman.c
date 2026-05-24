#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "main.h"
#include "format_zip.h"
#include "huffman.h"

static Heap heap;
static Node nodes[2*MAX_CHAR_NUM-1];

static int nodes_init(void)
{
    // 根据freq_table, 以所有出现过的字符作为初始节点
    int cnt=0;
    for(int i=0;i<MAX_CHAR_NUM;i++) if(freq_table[i].sum)
    {
        nodes[cnt].letter=(uchar)i;
        nodes[cnt].freq=freq_table[i].sum;
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
    // 添加两个最小节点的父节点 (分支节点)
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
    // 取出最小节点
    Node *min_node=heap.slot[0];
    heap.slot[0]=heap.slot[heap.len-1];
    heap.len--;
    heapify_down(0);
    return min_node;
}

static Node *heapify_build(void)
{
    // 先初始化最小堆, 再逐步更新节点, 最后返回根节点
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

static void shrink_code(codeTable *letter)
{
    // 将编码串通过位运算进一步缩短, len不变
    // e.g. before: '0' '1'
    //      after : str[0]= 01000000
    uchar str[MAX_CODE_LEN+1]="";
    uchar *p=str;
    for(int i=0;i<letter->len;i++)
    {
        if(i && i%CHAR_BIT==0) p++;
        uchar bit = letter->str[i] - '0';
        *p |= (bit<<(CHAR_BIT-1-i%CHAR_BIT));
    }
    memcpy(letter->str,str,letter->len);
}

static void form_code(Node *root,char *codestr,int depth)
{
    // 从根节点开始, 递归获取霍夫曼编码串
    if(root==NULL||depth>=MAX_CODE_LEN) return;
    if(root->l==NULL && root->r==NULL)
    {
        codestr[depth]='\0';
        codeTable *letter=&code_table[root->letter];
        memcpy(letter->str,codestr,depth+1);
        letter->len=depth;
        shrink_code(letter);
        return;
    }
    codestr[depth]='0';
    form_code(root->l,codestr,depth+1);
    codestr[depth]='1';
    form_code(root->r,codestr,depth+1);
}

void encode(void)
{
    // 返回code_table, 但其作为全局变量使用, 故不传参
    Node *root=heapify_build();
    // 如果只有一种字符, 则需单独处理
    if(root->l==NULL && root->r==NULL)
    {
        // 隐式设置编码串为'0'
        code_table[root->letter].len=1;
        return;
    }
    char codestr[MAX_CODE_LEN+1]="";
    form_code(root,codestr,0);
}

static void revert_code(int idx,uchar *ostr)
{
    const codeTable *code=&code_table[idx];
    const uchar *istr=code->str;
    for(int i=0;i<code->len;i++)
    {
        int shift=i%CHAR_BIT;
        if(i && shift==0) istr++;
        ostr[i]=((*istr)>>(CHAR_BIT-1-shift))&1;
        ostr[i]+='0';
    }
    /*printf("'%c': %s\n",code->letter,ostr);*/
}

const Node *decode(const int letter_cnt)
{
    // 初始化霍夫曼树 (以0为根节点)
    memset(&nodes,0,sizeof(Node)*(2*MAX_CHAR_NUM-1));
    Node *node_p=&nodes[0];
    int node_cnt=1;
    // 逐个解码, 保证字符位于叶节点
    for(int i=0;i<letter_cnt;i++)
    {
        // 获取编码串
        uchar str[MAX_CODE_LEN+1]="";
        revert_code(i,str);
        // 定位节点位置 (0左1右, 无子节点则新建)
        for(uchar *p=str;(*p)!='\0';p++)
        {
            if(*p=='0')
            {
                if(node_p->l==NULL) node_p->l=&nodes[node_cnt++];
                node_p=node_p->l;
            }
            else if(*p=='1')
            {
                if(node_p->r==NULL) node_p->r=&nodes[node_cnt++];
                node_p=node_p->r;
            }
            else continue;
        }
        // 标记叶节点, 回到根节点
        node_p->letter=code_table[i].letter;
        node_p=&nodes[0];
    }
    return nodes;
}