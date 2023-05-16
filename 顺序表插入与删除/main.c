#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 10
typedef struct
{
    int data[MAXSIZE];
    int length;
}List;
List *MakeList()
{
    List *L=(List *)malloc(sizeof(List));
    L->length=0;
    return L;
}
void InSert(List *L,int position,int n)//顺序表插入
{
    for(int j=L->length; j>=position; j--)
    {
        L->data[j]=L->data[j-1];//元素逐个向后移一位
    }
    L->data[position-1]=n;
    L->length++;
}
void DeLete(List *L,int position)//顺序表删除
{
    for(int j=position-1; j<L->length; j++)
    {
        L->data[j]=L->data[j+1];//元素逐个向前移一位
    }
    L->length--;
}
int main()
{
    List *L;
    L=MakeList();
    int x=0,n=0,position;
    printf("原始数组为：");
    for(n; n<=4; n++)//循环赋值并得出长度
    {
        x++;
        L->data[n]=x;
        L->length++;
        printf("%d   ",L->data[n]);
    }
    printf("\n数组长度为：%d\n",L->length);
    InSert(L,3,8);//在数组的第三个位置插入整数8
    printf("插入后的数组：");
    for(n=0; n<L->length; n++)
    {
        printf("%d   ",L->data[n]);//循环打印
    }
    DeLete(L,4);//删除数组第四个位置的元素
    printf("\n删除后的数组：");
    for(n=0; n<L->length; n++)
    {
        printf("%d   ",L->data[n]);
    }
    return 0;
}
