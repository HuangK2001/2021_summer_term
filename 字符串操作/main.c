#include <stdio.h>
#include <stdlib.h>

int myStrleng(char *str)//�ַ������ȼ���
{
    int length=0;
    while(*str!='\0')
    {
        length++;
        str++;
    }
    return length;
}
char *myStrcat(char *str1,char *str2)//���������ַ���
{
    char *tmp = str1;
    while(*str1)
        str1++;
    while(*str1++ = *str2++);
    return tmp;
}
int myStrcmp(char *str1,char *str2)//�Ƚ������ַ���
{
    while(*str1&& *str2 &&(*str1==*str2))
    {
        str1++;
        str2++;
    }
    return *str1-*str2;
}
int myStringSearch(char *str1,char *str3)//���ַ����в�����һ�ַ������ֵĴ���
{
    int times=0;
    int leng1=myStrleng(str1);
    int leng3=myStrleng(str3);
    for(int i=0;i<leng1;i++)
    {
        if(str3[0]==str1[i])
        {
            if(str3[1]==str1[i+1])
            {
                times++;
            }
        }
    }
    return times;
}

int main()
{
    char str1[50]= {"ABCDEFAB"};
    char str2[50]= {"ABCDEFAB"};
    char str3[50]= {"AB"};
    printf("�ַ���1�ĳ���Ϊ��%d\n�ַ���2�ĳ���Ϊ��%d\n",myStrleng(str1),myStrleng(str2));
    printf("�ַ���1Ϊ��%s\n�ַ���2Ϊ��%s\n",str1,str2);
    if(myStrcmp(str1,str2)<0)
        printf("�ַ���1С���ַ���2��");
    else if(myStrcmp(str1,str2)>0)
        printf("�ַ���1�����ַ���2��");
    else if(myStrcmp(str1,str2)==0)
        printf("�ַ���1�����ַ���2��");
    printf("\nƴ�Ӻ���ַ���Ϊ��%s\n",myStrcat(str1,str2));
    printf("�ַ���3��ƴ���ַ����г��ֵĴ���Ϊ��%d",myStringSearch(str1,str3));
    return 0;
}
