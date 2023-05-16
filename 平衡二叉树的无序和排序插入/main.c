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

void Insert(Node *tree,char X)//ƽ�����������
{
	if(!tree){
		tree=new node;
		tree->data=X;
		tree->left=tree->right=NULL;
	}else if(X<tree->data){
		tree->left=Insert(tree->left,X);
		if(GetBT(tree)==2){//ƽ�����ӳ���1���Լ���Ϊʲô��==2������>=2
			if(tree->left->data>X){//����
				tree=LL(tree);
			}else{//���ҵ���
				tree=LR(tree);
			}
		}
	}else{
		tree->right=Insert(tree->right,X);
		if(GetBT(tree)==-2){//ƽ�����ӳ���1���Լ���Ϊʲô��==-2������<=-2
			if(tree->right->data>X){//�ҵ���
				tree=RR(tree);
			}else{//������
				tree=RL(tree);
			}
		}
	}
	return tree;
}
int main()
{
    printf("ʹ��#�ŷ�����һ��������:");
    Node *T;
    T=CreateTree();

    return 0;
}
