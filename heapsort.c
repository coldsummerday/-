/*
堆排序，主要用最小堆 进行升序排列，每次通过最小值删除或者打印，然后再调整堆顶元素
以实现每次都是最小值弹出，直到堆元素为空，即排列完成
*/
#include <stdio.h>
#include <stdlib.h>
int h[101];//用数组做堆
int n;//堆的大小
void creat();
void swap(int x,int y);
void siftdown(int i);
int deleteroot();
void heapSort();
void swap(int x,int y)
{
    int temp;
    temp=h[x];
    h[x]=h[y];
    h[y]=temp;
}
void siftdown(int i)//该函数为调整函数，调整i的元素使之堆成最大或者最小堆
{
    int t,flag=0;
    while((i*2<=n)&&(flag==0))//当该节点还有孩子节点（起码有左节点）
    {
        if(h[i]>h[2*i])//父节点与左节点比较，谁小 就记谁坐标
        {
            t=2*i;
        }
        else
        {
            t=i;
        }
        if((2*i+1)<=n)//如果有右孩子节点，则取三者最小的坐标
        {
            if(h[t]>h[2*i+1])
                t=2*i+1;
        }
        if(t!=i)//T！=I 代表需要节点移动
        {
            swap(i,t);
            i=t;//记得更新I值，以确保每一次节点移动后还能移动
        }
        else
        {
            flag=1;//T=I代表父节点不需要移动，退出循环
        }
    }
}
void creat()
{
    int i;
    for(i=n/2;i>=1;i--)//除了叶子节点，其他节点都进行一次调整，以保证数组为堆的形式（叶子节点如果不合适，会在调整过程会被移动）
    {
        siftdown(i);
    }
}
int deleteroot()//方法1：每次取根值并更新
{
    int t;//每次取跟节点值，然后把最后的值放入根节点再进行调整
    t=h[1];
    h[1]=h[n];
    n--;//记得每次把删除的值 放出，数组最后值放到数组头，数组长度-1，
    siftdown(1);
    return t;
}
void heapSort()//方法二： 升序时候建立最大堆，然后h[1]跟h[n]换位置，这样最大值就在h[n]了，记得n--；。调整一遍再把最大值
{              //放到倒数第二，依次进行
    while(n>1)
    {
        swap(1,n);
        n--;
        siftdown(1);
    }
}
int main()
{
    int num,i;
    scanf("%d",&num);
    n=num;
    for(i=1;i<=num;i++)
    {
        scanf("%d",&h[i]);
    }
    creat();
    /*for(i=1;i<=num;i++)
    {
        printf("%d ",deleteroot());//方法1
    }*/

    n=num;
    heapSort();
    for(i=num;i>=1;i--)//方法二，不能一起用的原因是h[]数组会变
    {
        printf("%d ",h[i]);
    }
    getchar();
    return 0;
}
