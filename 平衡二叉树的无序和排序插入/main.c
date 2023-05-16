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

void Insert(Node *tree,char X)//平衡二叉树插入
{
	if(!tree){
		tree=new node;
		tree->data=X;
		tree->left=tree->right=NULL;
	}else if(X<tree->data){
		tree->left=Insert(tree->left,X);
		if(GetBT(tree)==2){//平衡因子超过1，自己想为什么是==2而不是>=2
			if(tree->left->data>X){//左单旋
				tree=LL(tree);
			}else{//左右单旋
				tree=LR(tree);
			}
		}
	}else{
		tree->right=Insert(tree->right,X);
		if(GetBT(tree)==-2){//平衡因子超过1，自己想为什么是==-2而不是<=-2
			if(tree->right->data>X){//右单旋
				tree=RR(tree);
			}else{//右左单旋
				tree=RL(tree);
			}
		}
	}
	return tree;
}
int main()
{
    printf("使用#号法创建一个二叉树:");
    Node *T;
    T=CreateTree();

    return 0;
}
