#include <stdio.h>
#include <stdlib.h>
typedef struct file//�ļ��ṹ��
{
    char name[10];//�ļ���
    char time[10];//����ʱ��
    int size;//�ļ���С
    char type;//�ļ�����
} File;
typedef struct filenode//�ļ���
{
    File *data;//�ļ����а������ļ�����
    struct filenode *folder;//���ļ���
    struct filenode *next;//ָ����һ�ļ��л��ļ���ָ��
} Filenode;

void fileMake()//�����ļ�
{
    Filenode* root = (Filenode*)malloc(sizeof(Filenode));
    root->data->name = "ABC";
    root->data->time = "2021.6.25";
    root->data->type = "txt";
    root->folder = fileMake();
    root->next = fileMake();
    return root;
}
int fileAttribute(Filenode *root)//�ļ�����
{
    return sizeof(root);//�����ļ��ֽڴ�С
}
void fileSearch(Filenode *root,char name)//�ļ�������ͨ���ļ�����
{
//    while(root!=NULL)
//    {
//        root=root->next;
//        if(root->data->name==name)printf("���ҵ�Ŀ���ļ���СΪ��%d�ֽ�\n",fileAttribute(root));
//        if(root->folder->data->name==name)printf("���ҵ�Ŀ���ļ���СΪ��%d�ֽ�\n",fileAttribute(root));
//        while(root->folder!=NULL)
//        {
//            root->folder=root->folder->next;
//            if(root->folder->data->name==name)printf("���ҵ�Ŀ���ļ���СΪ��%d�ֽ�\n",fileAttribute(root));
//        }
//
//    }
    if(root->data->name==name)printf("���ҵ�Ŀ���ļ���СΪ��%d�ֽ�\n",fileAttribute(root));
    fileSearch(root->folder);//�ȱ�������ÿ���ڵ���ļ�����
    fileSearch(root->next);//�ٱ�������ÿ���ڵ�ָ�����һ��ָ��
}
void fileMove(Filenode *root)//�ļ��ƶ�
{

}
void fileDelete(Filenode *root)//�ļ�ɾ��
{
    if(root == NULL)return 0;
    fileDelete(root->folder);
    fileDelete(root->next);
    free(root);
}
void fileSortByName(Filenode *root)//����������
{

}
void fileSortBySize(Filenode *root)//���ļ���С����
{

}
void fileSortByTime(Filenode *root)//���ļ�����ʱ������
{

}
int main()
{
    Filenode *root;
    char name[10];
    int n;
    printf("\n\n\t|********ģ���ļ�����ϵͳ*********************|\n");//����
    printf("\t|*********1.�½��ļ�******************************|\n");
    printf("\t|*********2.��ʾ�ļ�����**************************|\n");
    printf("\t|*********3.�����ļ�******************************|\n");
    printf("\t|*********4.�ƶ��ļ�******************************|\n");
    printf("\t|*********5.ɾ���ļ�******************************|\n");
    printf("\t|*********6.�����ļ�******************************|\n");
    printf("\t|*********0.�˳�**********************************|\n");
    while(1)
    {
        int s,ss;
        printf("��������Ҫ�������ܵ���ţ�");
        scanf("%d",&s);
        switch(s)
        {
        case 1:
            root=fileMake()
                 break;
        case 2:
            printf("�ļ���СΪ��%d�ֽ�",fileAttribute(root));
            break;
        case 3:
            fileSearch(root,name);
            break;
        case 4:
            fileMove(root);
            break;
        case 5:
            fileDelete(root);
            break;
        case 6:
            printf("���������������� 1 \n����С���������� 2 \n��ʱ������������ 3 \n");
            scanf("%d",&ss);
            if(ss==1)fileSortByName(root);
            if(ss==2)fileSortBySize(root);
            if(ss==3)fileSortByTime(root);
            else printf("��������");
            break;
        case 0:
            system("cls");
            exit(0);
            break;
        }
    }

}

