#include <stdio.h>
#include <stdlib.h>
typedef struct node
{
    int data;
    struct node *next;
}Lnode;
Lnode *Creat(int n)//����һ��ԭʼ����
{
    Lnode *head,*pnew;
    head=(Lnode *)malloc(sizeof(Lnode));//����ͷ�ڵ�
    head->next=NULL;
    for(int i=0;i<n;i++)
    {
        pnew=(Lnode *)malloc(sizeof(Lnode));//�����½ڵ�
        pnew->data=n-i;//Ϊ�ڵ㸳ֵ
        pnew->next=head->next;
        head->next=pnew;//ʹͷ�ڵ�ָ���µĽڵ�
    }
    Lnode *p=head->next;//pָ���һ��Ԫ�ؽ��
    printf("ԭ����Ϊ��");
	while(p!=NULL)
	{
	    printf("%d    ",p->data);  //���һ������Ϣ
		p=p->next;
	}
	return head;
}
int length(Lnode *head)//����������
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
Lnode *InSert(Lnode *head,int n,int x)//����ָ��λ�ò���
{
    Lnode *p=head,*Pre,*Next;
    int j=0;
    Next=(Lnode*)malloc(sizeof(Lnode *));
    while(p->next!=NULL&&j<n-1)//ѭ����Ҫ����λ�õ�ǰ���ڵ�
    {
        p=p->next;
        j++;
    }
    Pre=p;
    Next->data=x;
    Next->next=Pre->next;
    Pre->next=Next;
    p=head->next;
    printf("����������Ϊ��");
    while(p!=NULL)
	{
	    printf("%d    ",p->data);  //���һ������Ϣ
		p=p->next;
	}
	return head;
}
Lnode *Delete(Lnode *head,int n)//����ָ��λ��ɾ��
{
    Lnode *p=head,*Pre,*d;
    int j=0;
    while(p->next!=NULL&&j<n-1)//ѭ����Ҫɾ��λ�õ�ǰ���ڵ�
    {
        p=p->next;
        j++;
    }
    Pre=p;
    d=Pre->next;
    Pre->next=d->next;
    free(d);
    p=head->next;
    printf("\nɾ���������Ϊ��");
    while(p!=NULL)
    {
        printf("%d    ",p->data);
		p=p->next;
    }
}
int main()
{
    Lnode *head,*heads;
    head=Creat(5);//ʹ��Creat��������һ������Ϊ5������
    printf("\n������Ϊ��%d \n",length(head));
    heads=InSert(head,3,8);//������ĵ�����λ�ò�������8
    Delete(heads,4);//ɾ��������ĸ�λ�õ�����
    return 0;
}
