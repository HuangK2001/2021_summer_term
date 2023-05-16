#include <stdio.h>
#include <stdlib.h>
typedef struct treenode
{
    char data;
    struct Node* left;
    struct Node* right;
} Node,*Node;
typedef struct   //结点结构
{
    char data;
    struct QNode *next;
} QNode,*QueuePtr;
typedef struct    //队列的链表结构
{
    QueuePtr front,rear;      //队头、队尾指针
} LinkQueue;

LinkQueue *CreatQueue()//初始化一个空队列Q
{
    LinkQueue *Q=(LinkQueue *)malloc(sizeof(LinkQueue ));
    QNode  *head;
    head=(QNode *)malloc(sizeof(QNode));
    head->next=NULL;
    Q->front=head;
    Q->rear=head;
    return  Q;
}
Node* CreateTree()//创建二叉树
{
    char ch;
    scanf("%c", &ch);
    if(ch == '#')
        return NULL;//#号数量表示线性维度
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
void Qpush(QNode *q,char data)//入队列
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
char Qpop(QNode *Q)//出队列
{
    QNode *p;
    if(Q->front==Q->rear)
        return 0;
    p=Q->front->next;    //将欲删除的队头结点暂存给p
    char e=p->data;    //将欲删除的队头结点的值赋值给e
    Q->front->next=p->next;
    //将原队头结点的后继p->next赋值给头结点后继
    if(Q->rear==p)    //若队头就是队尾，则删除后将rear指向头结点
        Q->rear=Q->front;
    free(p);
    return e;
}
int main()
{
    printf("使用#号法创建一个二叉树:");
    Node *T;
    QNode *Q;
    Q=CreatQueue()
      T=CreateTree();
    TravelByLevel(T,Q);
    return 0;
}
