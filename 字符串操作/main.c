#include <stdio.h>
#include <stdlib.h>

int myStrleng(char *str)//字符串长度计算
{
    int length=0;
    while(*str!='\0')
    {
        length++;
        str++;
    }
    return length;
}
char *myStrcat(char *str1,char *str2)//连接两个字符串
{
    char *tmp = str1;
    while(*str1)
        str1++;
    while(*str1++ = *str2++);
    return tmp;
}
int myStrcmp(char *str1,char *str2)//比较两个字符串
{
    while(*str1&& *str2 &&(*str1==*str2))
    {
        str1++;
        str2++;
    }
    return *str1-*str2;
}
int myStringSearch(char *str1,char *str3)//在字符串中查找另一字符串出现的次数
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
    printf("字符串1的长度为：%d\n字符串2的长度为：%d\n",myStrleng(str1),myStrleng(str2));
    printf("字符串1为：%s\n字符串2为：%s\n",str1,str2);
    if(myStrcmp(str1,str2)<0)
        printf("字符串1小于字符串2！");
    else if(myStrcmp(str1,str2)>0)
        printf("字符串1大于字符串2！");
    else if(myStrcmp(str1,str2)==0)
        printf("字符串1等于字符串2！");
    printf("\n拼接后的字符串为：%s\n",myStrcat(str1,str2));
    printf("字符串3在拼接字符串中出现的次数为：%d",myStringSearch(str1,str3));
    return 0;
}
