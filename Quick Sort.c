#include <stdio.h>
int a[101],n;

void quicksort(int left,int right)
{
    int i,j,t,temp;
    if(left>right)
        return;
    temp=a[left]; //temp�д�ľ��ǻ�׼���������Ƚϵ�����
    i=left;
    j=right;
    while(i!=j)
    {
         while(a[j]>=temp && i<j)//˳�����Ҫ��Ҫ�ȴ��������ң���Ϊ����ߵ�Ϊ��׼�����Ա�����ҿ�ʼ�ң��Ͳ����Flag���ظ��Ž�ȥ��
            j--;
        while(a[i]<=temp && i<j)  //�ٴ���������
            i++;
        if(i<j)
        {
            t=a[i];
            a[i]=a[j];
            a[j]=t;
        }
    }
    //�ս���׼��flag��λ
    a[left]=a[i];
    a[i]=temp;
    quicksort(left,i-1);//����������ߵģ�������һ���ݹ�Ĺ���
    quicksort(i+1,right);//���������ұߵģ�������һ���ݹ�Ĺ���
}

int main()

{
    int i;
    printf("input the nunbers of num\n");
    scanf("%d",&n);
    for(i=1; i<=n; i++)
    {
        scanf("%d",&a[i]);
    }
    quicksort(1,n);
    for(i=1; i<=n; i++)
    {
        printf("%d ",a[i]);
    }
    getchar();
    return 0;
}
