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
void InSert(List *L,int position,int n)//˳������
{
    for(int j=L->length; j>=position; j--)
    {
        L->data[j]=L->data[j-1];//Ԫ����������һλ
    }
    L->data[position-1]=n;
    L->length++;
}
void DeLete(List *L,int position)//˳���ɾ��
{
    for(int j=position-1; j<L->length; j++)
    {
        L->data[j]=L->data[j+1];//Ԫ�������ǰ��һλ
    }
    L->length--;
}
int main()
{
    List *L;
    L=MakeList();
    int x=0,n=0,position;
    printf("ԭʼ����Ϊ��");
    for(n; n<=4; n++)//ѭ����ֵ���ó�����
    {
        x++;
        L->data[n]=x;
        L->length++;
        printf("%d   ",L->data[n]);
    }
    printf("\n���鳤��Ϊ��%d\n",L->length);
    InSert(L,3,8);//������ĵ�����λ�ò�������8
    printf("���������飺");
    for(n=0; n<L->length; n++)
    {
        printf("%d   ",L->data[n]);//ѭ����ӡ
    }
    DeLete(L,4);//ɾ��������ĸ�λ�õ�Ԫ��
    printf("\nɾ��������飺");
    for(n=0; n<L->length; n++)
    {
        printf("%d   ",L->data[n]);
    }
    return 0;
}
