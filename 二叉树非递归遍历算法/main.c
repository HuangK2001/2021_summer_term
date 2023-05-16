#include <stdio.h>
#include <stdlib.h>
typedef struct treenode
{
    char data;
    struct Node* left;
    struct Node* right;
} Node,*Node;
typedef struct   //���ṹ
{
    char data;
    struct QNode *next;
} QNode,*QueuePtr;
typedef struct    //���е�����ṹ
{
    QueuePtr front,rear;      //��ͷ����βָ��
} LinkQueue;

LinkQueue *CreatQueue()//��ʼ��һ���ն���Q
{
    LinkQueue *Q=(LinkQueue *)malloc(sizeof(LinkQueue ));
    QNode  *head;
    head=(QNode *)malloc(sizeof(QNode));
    head->next=NULL;
    Q->front=head;
    Q->rear=head;
    return  Q;
}
Node* CreateTree()//����������
{
    char ch;
    scanf("%c", &ch);
    if(ch == '#')
        return NULL;//#��������ʾ����ά��
    Node* root = (Node*)malloc(sizeof(Node));
    root->data = ch;
    root->left = CreateTree();
    root->right = CreateTree();
    return root;
}

void TravelByLevel(Node *root,QNode *Q)
{

    if(root!=NULL)
    {
        Qpush(Q,root->data);
    }
    while(Q!=NULL)
    {
        printf("%c ",root->data);
        if(root->left!=NULL)
        {
           Qpush(Q,root->left->data);
        }
        if(root->right!=NULL)
        {
           Qpush(Q,root->right->data);
        }
        Qpop(Q);
    }
}
void Qpush(QNode *q,char data)//�����
{
    QNode *temp=(QNode *)malloc(sizeof(QNode));
    while(q->next != NULL)
    {
        q = q->next;
    }
    temp->data = data;
    temp->next = q->next;
    q->next = temp;
}
char Qpop(QNode *Q)//������
{
    QNode *p;
    if(Q->front==Q->rear)
        return 0;
    p=Q->front->next;    //����ɾ���Ķ�ͷ����ݴ��p
    char e=p->data;    //����ɾ���Ķ�ͷ����ֵ��ֵ��e
    Q->front->next=p->next;
    //��ԭ��ͷ���ĺ��p->next��ֵ��ͷ�����
    if(Q->rear==p)    //����ͷ���Ƕ�β����ɾ����rearָ��ͷ���
        Q->rear=Q->front;
    free(p);
    return e;
}
int main()
{
    printf("ʹ��#�ŷ�����һ��������:");
    Node *T;
    QNode *Q;
    Q=CreatQueue()
      T=CreateTree();
    TravelByLevel(T,Q);
    return 0;
}
