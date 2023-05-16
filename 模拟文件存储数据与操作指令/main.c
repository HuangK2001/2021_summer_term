#include <stdio.h>
#include <stdlib.h>
typedef struct file//文件结构体
{
    char name[10];//文件名
    char time[10];//创建时间
    int size;//文件大小
    char type;//文件类型
} File;
typedef struct filenode//文件体
{
    File *data;//文件体中包含的文件数据
    struct filenode *folder;//子文件夹
    struct filenode *next;//指向下一文件夹或文件的指针
} Filenode;

void fileMake()//创建文件
{
    Filenode* root = (Filenode*)malloc(sizeof(Filenode));
    root->data->name = "ABC";
    root->data->time = "2021.6.25";
    root->data->type = "txt";
    root->folder = fileMake();
    root->next = fileMake();
    return root;
}
int fileAttribute(Filenode *root)//文件属性
{
    return sizeof(root);//计算文件字节大小
}
void fileSearch(Filenode *root,char name)//文件搜索（通过文件名）
{
//    while(root!=NULL)
//    {
//        root=root->next;
//        if(root->data->name==name)printf("查找的目标文件大小为：%d字节\n",fileAttribute(root));
//        if(root->folder->data->name==name)printf("查找的目标文件大小为：%d字节\n",fileAttribute(root));
//        while(root->folder!=NULL)
//        {
//            root->folder=root->folder->next;
//            if(root->folder->data->name==name)printf("查找的目标文件大小为：%d字节\n",fileAttribute(root));
//        }
//
//    }
    if(root->data->name==name)printf("查找的目标文件大小为：%d字节\n",fileAttribute(root));
    fileSearch(root->folder);//先遍历搜索每个节点的文件夹域
    fileSearch(root->next);//再遍历搜索每个节点指向的下一个指针
}
void fileMove(Filenode *root)//文件移动
{

}
void fileDelete(Filenode *root)//文件删除
{
    if(root == NULL)return 0;
    fileDelete(root->folder);
    fileDelete(root->next);
    free(root);
}
void fileSortByName(Filenode *root)//以名称排序
{

}
void fileSortBySize(Filenode *root)//以文件大小排序
{

}
void fileSortByTime(Filenode *root)//以文件创建时间排序
{

}
int main()
{
    Filenode *root;
    char name[10];
    int n;
    printf("\n\n\t|********模拟文件操作系统*********************|\n");//界面
    printf("\t|*********1.新建文件******************************|\n");
    printf("\t|*********2.显示文件属性**************************|\n");
    printf("\t|*********3.查找文件******************************|\n");
    printf("\t|*********4.移动文件******************************|\n");
    printf("\t|*********5.删除文件******************************|\n");
    printf("\t|*********6.排序文件******************************|\n");
    printf("\t|*********0.退出**********************************|\n");
    while(1)
    {
        int s,ss;
        printf("请输入想要操作功能的序号：");
        scanf("%d",&s);
        switch(s)
        {
        case 1:
            root=fileMake()
                 break;
        case 2:
            printf("文件大小为：%d字节",fileAttribute(root));
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
            printf("按名称排序请输入 1 \n按大小排序请输入 2 \n按时间排序请输入 3 \n");
            scanf("%d",&ss);
            if(ss==1)fileSortByName(root);
            if(ss==2)fileSortBySize(root);
            if(ss==3)fileSortByTime(root);
            else printf("输入有误！");
            break;
        case 0:
            system("cls");
            exit(0);
            break;
        }
    }

}

