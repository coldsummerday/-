/*
��������Ҫ����С�� �����������У�ÿ��ͨ����Сֵɾ�����ߴ�ӡ��Ȼ���ٵ����Ѷ�Ԫ��
��ʵ��ÿ�ζ�����Сֵ������ֱ����Ԫ��Ϊ�գ����������
*/
#include <stdio.h>
#include <stdlib.h>
int h[101];//����������
int n;//�ѵĴ�С
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
void siftdown(int i)//�ú���Ϊ��������������i��Ԫ��ʹ֮�ѳ���������С��
{
    int t,flag=0;
    while((i*2<=n)&&(flag==0))//���ýڵ㻹�к��ӽڵ㣨��������ڵ㣩
    {
        if(h[i]>h[2*i])//���ڵ�����ڵ�Ƚϣ�˭С �ͼ�˭����
        {
            t=2*i;
        }
        else
        {
            t=i;
        }
        if((2*i+1)<=n)//������Һ��ӽڵ㣬��ȡ������С������
        {
            if(h[t]>h[2*i+1])
                t=2*i+1;
        }
        if(t!=i)//T��=I ������Ҫ�ڵ��ƶ�
        {
            swap(i,t);
            i=t;//�ǵø���Iֵ����ȷ��ÿһ�νڵ��ƶ������ƶ�
        }
        else
        {
            flag=1;//T=I�����ڵ㲻��Ҫ�ƶ����˳�ѭ��
        }
    }
}
void creat()
{
    int i;
    for(i=n/2;i>=1;i--)//����Ҷ�ӽڵ㣬�����ڵ㶼����һ�ε������Ա�֤����Ϊ�ѵ���ʽ��Ҷ�ӽڵ���������ʣ����ڵ������̻ᱻ�ƶ���
    {
        siftdown(i);
    }
}
int deleteroot()//����1��ÿ��ȡ��ֵ������
{
    int t;//ÿ��ȡ���ڵ�ֵ��Ȼ�������ֵ������ڵ��ٽ��е���
    t=h[1];
    h[1]=h[n];
    n--;//�ǵ�ÿ�ΰ�ɾ����ֵ �ų����������ֵ�ŵ�����ͷ�����鳤��-1��
    siftdown(1);
    return t;
}
void heapSort()//�������� ����ʱ�������ѣ�Ȼ��h[1]��h[n]��λ�ã��������ֵ����h[n]�ˣ��ǵ�n--��������һ���ٰ����ֵ
{              //�ŵ������ڶ������ν���
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
        printf("%d ",deleteroot());//����1
    }*/

    n=num;
    heapSort();
    for(i=num;i>=1;i--)//������������һ���õ�ԭ����h[]������
    {
        printf("%d ",h[i]);
    }
    getchar();
    return 0;
}
