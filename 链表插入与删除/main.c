#include <stdio.h>
#include <stdlib.h>
typedef struct node
{
    int data;
    struct node *next;
}Lnode;
Lnode *Creat(int n)//创建一个原始链表
{
    Lnode *head,*pnew;
    head=(Lnode *)malloc(sizeof(Lnode));//创建头节点
    head->next=NULL;
    for(int i=0;i<n;i++)
    {
        pnew=(Lnode *)malloc(sizeof(Lnode));//创建新节点
        pnew->data=n-i;//为节点赋值
        pnew->next=head->next;
        head->next=pnew;//使头节点指向新的节点
    }
    Lnode *p=head->next;//p指向第一个元素结点
    printf("原链表为：");
	while(p!=NULL)
	{
	    printf("%d    ",p->data);  //输出一结点的信息
		p=p->next;
	}
	return head;
}
int length(Lnode *head)//计算链表长度
{
    Lnode* p=head->next;
    int i=0;
    while(p!=NULL)
    {
        i++;
        p=p->next;
    }
    return i;
}
Lnode *InSert(Lnode *head,int n,int x)//链表指定位置插入
{
    Lnode *p=head,*Pre,*Next;
    int j=0;
    Next=(Lnode*)malloc(sizeof(Lnode *));
    while(p->next!=NULL&&j<n-1)//循环到要插入位置的前驱节点
    {
        p=p->next;
        j++;
    }
    Pre=p;
    Next->data=x;
    Next->next=Pre->next;
    Pre->next=Next;
    p=head->next;
    printf("插入后的链表为：");
    while(p!=NULL)
	{
	    printf("%d    ",p->data);  //输出一结点的信息
		p=p->next;
	}
	return head;
}
Lnode *Delete(Lnode *head,int n)//链表指定位置删除
{
    Lnode *p=head,*Pre,*d;
    int j=0;
    while(p->next!=NULL&&j<n-1)//循环到要删除位置的前驱节点
    {
        p=p->next;
        j++;
    }
    Pre=p;
    d=Pre->next;
    Pre->next=d->next;
    free(d);
    p=head->next;
    printf("\n删除后的链表为：");
    while(p!=NULL)
    {
        printf("%d    ",p->data);
		p=p->next;
    }
}
int main()
{
    Lnode *head,*heads;
    head=Creat(5);//使用Creat函数创建一个长度为5的链表
    printf("\n链表长度为：%d \n",length(head));
    heads=InSert(head,3,8);//在链表的第三个位置插入整数8
    Delete(heads,4);//删除链表第四个位置的数据
    return 0;
}
