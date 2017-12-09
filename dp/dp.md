## 动态规划
 
Dynamic programming 动态规划:

通过组合子问题的解来求解原问题的解.
这里的programming是指表格法
其特点:

* 把原始问题划分为一系列的子问题
* 求解每个子问题仅一次,并将其结果保存在一个表中,以后用到时候直接存取,不重复计算,节省计算时间
* 自底向上地计算


使用动态规划的条件:

* 优化子结构
    * 当一个问题的优化解包含了子问题的优化解时,我们就说这个问题具有优化子结构
    * 缩小子问题集合,只需那些优化问题中包含的子问题,减低实现复杂性
* 重叠子问题
    * 在问题的求解过程中,很多子问题的解将被多次使用


我们通常按照如下4个步骤来设计一个动态规划算法:

1. 刻画一个最优解的结构特征
2. 递归地定义最优解的值
3. 计算最优解的值,通常采用自低向上的办法
4. 利用计算出的信息,构造一个最优解.


### 矩阵链乘问题

输入: <A1,A2,....,An>,Ai是矩阵
输出: 计算A1 * A2 * .....An的最小代价

矩阵乘法的代价/复杂性:  乘法的次数

若A是p\*q的矩阵,B是q\*r的矩阵,则A\*B的代价是O(pqr)


##### 题目背景
矩阵链乘法的代价与计算顺序的关系:
设A1=10100矩阵, A2=1005矩阵, A3=550矩阵
T((A1A2)A3)=101005+10550=7500
   
   T(A1(A2A3))=100550+1010050=750000 

不同的计算顺序有不同的代价


###切入点:(最后一次乘法将整个矩阵链分成两个小的矩阵链)

* 两个记号
Ai-j=AiAi+1....Aj
cost(Ai-j )=计算Ai-j的代价

优化子结构:
若计算A1n的优化顺序在k处断开矩阵链, 即A1~n=A1-kAk+1~n，则在A1~n的优化顺序中，对应于子问题A1k的解必须是A1~k的优化解，对应于子问题Ak+1~n的解必须是Ak+1~n的优化解 

子问题的重复性:
![](http://orh99zlhi.bkt.clouddn.com/2017-12-09,21:03:14.jpg)


假设:
* m[i, j] = 计算Ai~j的最小乘法数
* m[1,n] = 计算A1~n的最小乘法数

代价方程:
$$m[i, i] = 计算Ai~i 的最小乘法数= 0$$
$$m[i, j] = m[i, k] + m[k+1, j] + pi~1pkpj$$


其中$pi~1pkpj$是计算Ai~k * Ak+1~j 所需的乘法数,Ai k和Ak+1  j分别是pi-1\*pk和pk\*pj矩阵

当所有的K时候,优化解的代价方程为:
$$m[i,j]=0, if i=j$$
$$m[i,j]=min{m[i,k]+m[k+1,j]+Pi~1PkPj},  if i<j,i<=k<j$$


假如M是个二维矩阵,则:
![](http://orh99zlhi.bkt.clouddn.com/2017-12-09,21:28:07.jpg)


求1-m的代价,需要知道M矩阵的右上角的值,每次计算一个格子的值,需要知道其左,下的值,所以计算顺序,先填对角线,然后斜线式填写.

![](http://orh99zlhi.bkt.clouddn.com/2017-12-09,21:29:31.jpg)

伪代码:

```
Matrix-Chain-Order(p)
n=length(p)-1；
FOR  i=1  TO  n  DO
       m[i, i]=0;
FOR  l=2  TO  n  DO  /* 计算地l对角线 */
       FOR  i=1  TO  n-l+1  DO
               j=i+l-1;
              m[i, j]=∞; 
              FOR  k=i  To  j-1  DO /* 计算m[i,j] */
                     q=m[i, k]+m[k+1, j]+pi-1pkpj
                     IF  q<m[i, j]  THEN  m[i,j]=q; s[i,j]=k;//S[i,j]记录AiAi+1…Aj的
最优划分处在Ak与Ak+1
之间
Return m. 
```

cpp代码:

```
#include <iostream>
#include <string>
using  namespace std;
int m[7][7] ={0};
int s[7][7] = {0};

void Print_Optimal_Parens(int s[][7],int i,int j)  //构造最优解
{
    if ( i ==j)
    {
        printf("A%d",i);
    }
    else
    {
        printf("(");
        Print_Optimal_Parens(s,i,s[i][j]);
        Print_Optimal_Parens(s,s[i][j]+1,j);
        printf(")");
    }
}
void Matrix_Chain(int p[],int n)
{
    int i,j,L,k,q;
    for (i=1;i<=n;i++)      //先对单个矩阵的链，求解，即所有m[i][i] =0;
    {
        m[i][i]=0;
    }
    for(L=2;L<=n;L++)     //从两个矩阵链开始，逐次增加矩阵链的长度
        for(i=1;i<=n-L+1;i++)  //在给定p[]中的矩阵链中，对所有种长度为L的情况计算
        {
            j = i+L-1;
            m[i][j] = -1;
            for(k=i;k<=j-1;k++)   //遍历所有可能的划分点k，计算出最优的划分方案，
            {
                q = m[i][k]+m[k+1][j]+p[i-1]*p[k]*p[j];
                if ( q < m[i][j] || m[i][j] == -1)
                {
                    m[i][j] = q;     //最优的代价q保存在m[i][j]中
                    s[i][j] = k;     //最优的划分位置k保存在s[i][j]中
                }
            }
        }

}
int main() {

    int p[] = {30,35,15,5,10,20,25};
    int length = sizeof(p)/ sizeof(p[0])-1;//数组长度
    int i,j;
    Matrix_Chain(p,length);
    for(i =1;i<=6;i++)
    {
        for (j=1;j<=6;j++)
        {
            printf("%8d",m[i][j]);
        }
        printf("\n");
    }
    Print_Optimal_Parens(s,1,6);
    printf("\n");
}
```


####复杂度分析:
时间复杂度:
(l, i, k)三层循环, 每层至多n-1步  O(n的三次方)

空间复杂度:
o(n的二次方)




