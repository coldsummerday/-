#include <stdio.h>
int a[101],n;

void quicksort(int left,int right)
{
    int i,j,t,temp;
    if(left>right)
        return;
    temp=a[left]; //temp中存的就是基准数（用作比较的数）
    i=left;
    j=right;
    while(i!=j)
    {
         while(a[j]>=temp && i<j)//顺序很重要，要先从右往左找（因为定左边的为基准，所以必须从右开始找，就不会把Flag数重复排进去）
            j--;
        while(a[i]<=temp && i<j)  //再从左往右找
            i++;
        if(i<j)
        {
            t=a[i];
            a[i]=a[j];
            a[j]=t;
        }
    }
    //终将基准数flag归位
    a[left]=a[i];
    a[i]=temp;
    quicksort(left,i-1);//继续处理左边的，这里是一个递归的过程
    quicksort(i+1,right);//继续处理右边的，这里是一个递归的过程
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
