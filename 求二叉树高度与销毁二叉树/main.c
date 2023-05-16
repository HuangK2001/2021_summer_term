#include <stdio.h>
#include <stdlib.h>
typedef struct treenode
{
    char data;
    struct Node* left;
    struct Node* right;
}Node;
Node* CreateTree()//创建二叉树
{
    char ch;
    scanf("%c", &ch);
    if(ch == '#')return NULL;//#号数量表示线性维度
    Node* root = (Node*)malloc(sizeof(Node));
    root->data = ch;
    root->left = CreateTree();
    root->right = CreateTree();
    return root;
}
int Height(Node *t)//递归方法求二叉树高度
{
    if(t==NULL)return 0;
    int hl=0,hr=0;
    hl=Height(t->left);
    hr=Height(t->right);
    if(hl>=hr)return hl+1;
    else return hr+1;
}
void DestroyTree(Node* t)//必须以后序遍历的方式进行销毁，最后销毁根节点
{
    if(t == NULL)return 0;

    DestroyTree(t->left);
    DestroyTree(t->right);
    printf("当前正在销毁的结点值为：%c\n", t->data);
    free(t);
}
int main()
{
    printf("使用#号法创建一个二叉树:");
    Node *T;
    T=CreateTree();
    printf("该二叉树高度为：%d\n",Height(T));
    DestroyTree(T);
    return 0;
}
