#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_N 128

int sizeTable[] = { 9, 99, 999, 9999, 99999, 999999, 9999999,
                      99999999, 999999999, 2147483647 };

int CharNum[MAX_N]={0};
//字符数出现次数表，下标为对应字符ASCII
//小于31的为控制字符

typedef struct HuffmanTree
{
    struct HuffmanTree * Left;
    struct HuffmanTree * Parent;
    struct HuffmanTree * Right;
    int value;//权重-字符出现的频次
    char cha;
}Hutr;
Hutr *p[MAX_N];//霍夫曼树的外部节点
Hutr *p2[MAX_N];//副本

char ch;
char flag = 0;//输出操作对象
int count = 0;//计数器
Hutr * p_in;//树的元节点

int stringSize(int x);
//计算一个整数的位数
void ByteAnasis(FILE * in);
//字符数字分析
int minnum(int x, int y);
//求两个整数的最小值
void bucket_sort(void);
//桶排序
void BuildHuffmanTree(void);
//构建霍夫曼树
void insertion_sort(Hutr * arr[][MAX_N], int h,int w);
//插入排序
void insert(Hutr * p_m,int k);
//插入节点表使当前处理的两节点权值始终最小
Hutr * SeekNode(char k);
//找字符对应的节点
void SearchTree(Hutr * p_3,Hutr * p_4,FILE *a);
void putnumber(Hutr * p_3,Hutr * p_4,FILE *a);

//bit级操作函数
void bitin(int bitw,FILE * out);

//树内存的释放
void freeTree(Hutr * p5);

int main(void)
{
    

    FILE * in,* out,* charlist;//输入文件指针，输出文件指针,字符对应表指针
    char *name;//储存文件名
    char *in_file;//储存输入路径


    in_file = (char*)malloc(50);
    name = (char*)malloc(15);

    printf("Please enter the file name(Incude the path):\n");
    scanf("%100s",in_file);
    fflush(stdin);

    printf("Please enter the compressed file name \n");
    scanf("%10s",name);
    strcat(name,".hfip");
    fflush(stdin);

    if ( (in = fopen(in_file,"r"))==NULL )
    {
        printf("No such file");
        exit(EXIT_FAILURE);
    }
    ByteAnasis(in);

    rewind(in);

    BuildHuffmanTree();
    //创建一颗霍夫曼树，返回元节点p_in;

    //创建输出文件
    out = fopen(name, "wb+");

    while ( (ch = getc(in)) !=EOF )
    {
        Hutr *p_ptr = SeekNode(ch);
        SearchTree(p_ptr,NULL,out);
    }
    if (count%8 != 0 )
        fwrite(&flag,1,1,out);
    fclose(in);
    fclose(out);

    //创建输出字符对应表
    charlist = fopen("charlist.txt","w");
    for (int i10 = 0 ; i10 < MAX_N ; i10 ++)
    {
        fprintf(charlist,"%d",p2[i10]->cha);
        putc(' ',charlist);
        putnumber(p2[i10],NULL,charlist);
        putc('\n',charlist);
    }
    fclose(charlist);
    
    //内存释放
    freeTree(p_in);
    free(name);
    free(in_file);

    printf("Complet");

    return 0;
}

void ByteAnasis(FILE * in)
{//字节数统计
    while ( (ch = getc(in)) != EOF )
    {
        int b = ch;
        CharNum[b] ++; 
    }
}
int minnum(int x, int y) 
{
    return x < y ? x : y;
}

void bucket_sort(void) 
{//桶排序
	Hutr * bucket[10][MAX_N]={0};//收集变量的桶
    int T_num[10]={0};//每个桶的变量数
    
    for (int i2 = 0 ; i2 < MAX_N ; i2++)
    {
        int temp;
        temp = stringSize( p[i2]->value )-1;
        bucket[temp][ T_num[temp] ] = p[i2];
        T_num[temp] ++;
    }
    for (int i3 = 0 ; i3 < 10 ; i3++)
    {
        for (int i4 = 0; i4 < T_num[i3] ; i4++)
        {
            insertion_sort(bucket,i3,i4);
        }        
    }
    int i5=0;//记数
    for (int i3 = 0 ; i3 < 10 ; i3++)
    {
        for (int i4 = 0; i4 < T_num[i3] ; i4++)
        {
            p[i5] = bucket[i3][i4];
            i5++;
        }
    } 
}

void insertion_sort(Hutr * arr[][MAX_N], int h,int w)
{//插入排序1
        int i,j,key;
        Hutr * key_ptr;
        for (i=1;i<w;i++)
        {
            key_ptr = arr[h][i];
            key = arr[h][i]->value;
            j=i-1;
            while((j>=0) && (arr[h][j]->value > key)) 
            {
                arr[h][j+1] = arr[h][j];
                j--;
            }
            arr[h][j+1] = key_ptr;
        }
}

void insert(Hutr * p_m,int k)
{

    for (int i = k ; i<MAX_N && p_m->value >= p[i]->value ; i++)
    {
        p[i-1] = p[i];
        p[i] = p_m;
    }
}

void BuildHuffmanTree(void)
{//构建霍夫曼树
    for (int i1=0 ; i1 < MAX_N ; i1++)
    {
        p[i1] = malloc(sizeof(Hutr));
        p[i1]->cha = i1;
        p[i1]->value = CharNum[i1];
        p[i1]->Left = NULL;
        p[i1]->Right = NULL;
        p[i1]->Parent = NULL;
    }
    bucket_sort();//将节点数组按权值排序
    memcpy(p2,p,sizeof(Hutr *)*MAX_N);
    
    p_in = malloc(sizeof(Hutr));
    p_in->Left = p[0];
    p_in->Right = p[1];
    p_in->value = p[0]->value + p[1]->value;
    p_in->Parent = NULL;
    p_in->cha = 0;

    p[0]->Parent = p[1]->Parent = p_in;

    for (int i7=1 ; i7 < MAX_N-1 ;i7 ++)
    {
        insert(p_in,i7);
        p_in = malloc(sizeof(Hutr));
        p_in->Left = p[i7];
        p_in->Right = p[i7+1];
        p_in->Parent = NULL;
        p_in->value = p[i7]->value + p[i7+1]->value;
        p_in->cha = 0;
        p[i7]->Parent = p[i7+1]->Parent = p_in;
    }
}

int stringSize(int x)
{//计算一个整数的位数
    for (int i=0; ; i++)
        if (x <= sizeTable[i])
            return i+1;
}

//bit级操作函数
void bitin(int bitw,FILE * out)
{//参数：要写入的0或1
    char m = 1;
    if (bitw)
    {
        flag = (flag | (m << 7 >> (count%8)));
    }
    count ++;
    if (count%8 == 0)
    {
        fwrite(&flag,1,1,out);
        flag = 0;
    }
}

Hutr * SeekNode(char k)
{//找到在字符对应的外部节点
    for (int i = 0 ; i<MAX_N ; i++)
    {
        if (p2[i]->cha == k)
            return p2[i];
    }
}

void SearchTree(Hutr * p_3,Hutr * p_4,FILE *a)
{
    if (p_3->Parent != NULL)
        SearchTree(p_3->Parent,p_3,a);
    
    if (p_3->Left == p_4 && p_4 !=NULL)
        bitin(0,a);
    else if (p_4 != NULL)
        bitin(1,a);
}

void putnumber(Hutr * p_3,Hutr * p_4,FILE *a)
{//字符对应表的输出
    if (p_3->Parent != NULL)
        putnumber(p_3->Parent,p_3,a);
    
    if (p_3->Left == p_4 && p_4 !=NULL)
        putc('0',a);
    else if (p_4 != NULL)
        putc('1',a);
}

void freeTree(Hutr * p5)
{
    if (p5->Left == NULL && p5->Right == NULL)
    {
        free(p5);
    }
    else if (p5->Right == NULL)
    {
        freeTree(p5->Left);
    }
    else if (p5->Right == NULL)
    {
        freeTree(p5->Right);
    }
    else
    {
        freeTree(p5->Right);
        freeTree(p5->Left);
        free(p5);
    }
}