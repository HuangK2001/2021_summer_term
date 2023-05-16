#include <stdio.h>
#include <stdlib.h>
typedef struct treenode
{
    char data;
    struct Node* left;
    struct Node* right;
}Node;
Node* CreateTree()//����������
{
    char ch;
    scanf("%c", &ch);
    if(ch == '#')return NULL;//#��������ʾ����ά��
    Node* root = (Node*)malloc(sizeof(Node));
    root->data = ch;
    root->left = CreateTree();
    root->right = CreateTree();
    return root;
}
int Height(Node *t)//�ݹ鷽����������߶�
{
    if(t==NULL)return 0;
    int hl=0,hr=0;
    hl=Height(t->left);
    hr=Height(t->right);
    if(hl>=hr)return hl+1;
    else return hr+1;
}
void DestroyTree(Node* t)//�����Ժ�������ķ�ʽ�������٣�������ٸ��ڵ�
{
    if(t == NULL)return 0;

    DestroyTree(t->left);
    DestroyTree(t->right);
    printf("��ǰ�������ٵĽ��ֵΪ��%c\n", t->data);
    free(t);
}
int main()
{
    printf("ʹ��#�ŷ�����һ��������:");
    Node *T;
    T=CreateTree();
    printf("�ö������߶�Ϊ��%d\n",Height(T));
    DestroyTree(T);
    return 0;
}
