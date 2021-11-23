#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 128
int team[128]={0};//队列
int * p_last=&team[0];//指向队列尾部的指针
long long int count=0;//计数器：判断是否读取八bits

void check_char(char * charlist1[MAX_N+1],char charmap1[MAX_N],FILE * out);
void push_team(char ch);

int main(void)
{
    FILE * charlist,* in,* out;
    char * charlist_path,* in_path;
    charlist_path = malloc(75);
    in_path = malloc(75);
    char * charlist_ptr[MAX_N+1],* charlist_ptr2[MAX_N+1];//保存编码
    char charmap[MAX_N];//保存编码对应的字符
    printf("Please enter the charlist(include the path)");
    scanf("%s",charlist_path);
    fflush(stdin);
    printf("Please enter the file that need to be unpressed"
    "(include the path)");
    scanf("%s",in_path);
    fflush(stdin);

    //建立字符编码对应表
    if ( (charlist = fopen(charlist_path,"r")) == NULL )
    {
        printf("No such file\n");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    charlist_ptr2[0] = malloc(132);
    while ( fgets(charlist_ptr2[i],132,charlist) !=NULL )
    {
        char *m;
        char *l;
        m = strtok(charlist_ptr2[i]," ");
        l = strtok(NULL," ");
        charlist_ptr[i] = l;
        charmap[i] = atoi(m);
        charlist_ptr2[i+1] = malloc(132);
        i++;
    }//切割字符串，保存霍夫曼编码及其对应字符
    fclose(charlist);


    char ch1;//保存输入字符 
    if ( (in = fopen(in_path,"rb+")) == NULL )
    {
        printf("No such file\n");
        exit(EXIT_FAILURE);
    }
    out = fopen("uncompress.txt","w");
    while(fread(&ch1,1,1,in)!=0) 
    {
        for (int i1 = 0 ; i1 < 8 ; i1++)
        {
            push_team(ch1);
            count++;
            check_char(charlist_ptr,charmap,out);
        }
    }
    fclose(in);
    fclose(out);

    //释放分配的内存
    for (int i5 = 0 ; i<=MAX_N ; i++)
        free(charlist_ptr2[i5]);
    free(charlist_path);
    free(in_path);
    
    printf("Complet");
    return 0;
}

void push_team(char ch)
{//将读取的bits存入队列
    char m = (1<<7>>(count%8));
    m = ch&m;
    if (m)
    {
        *p_last = 1;
        p_last ++;
    }
    else
    {
        *p_last = 0;
        p_last++;
    }
}

void check_char(char * charlist_ptr1[MAX_N+1],char charmap1[MAX_N],FILE * out)
{//找到字符对应的霍夫曼节点
    for (int i3 = 0 ; i3 < MAX_N ; i3++)
    {
        int i2=0;
        for ( ; i2<(p_last-team) ; i2++)
        {
            if ((*(charlist_ptr1[i3]+i2)-48) !=  team[i2])
                break;
        }
        if (i2 == (p_last-team) && i2 == strlen(charlist_ptr1[i3])-1)
        {
            putc(charmap1[i3],out);
            p_last = &team[0];
        }
        if (i2 == (p_last-team) && i2 == strlen(charlist_ptr1[i3])-1)
            break;
    }
}