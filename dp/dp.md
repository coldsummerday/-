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



###最长公共子序列问题:


####优化解结构:
设X=(x1, ..., xm)、Y=(y1, ..., yn)
是两个序列，Z=(z1, ..., zk)是X与Y的LCS，我们有：

* 如果xm=yn, 则Zk=Xm=Yn, Zk-1是Xm-1和Yn-1的LCS:LCSXY = LCSXm-1Yn-1 + Xm=Yn
* 如果xmyn，且zkxm，则Z是Xm-1和Y的LCS，即LCSXY= LCSXm-1Y
* 如果xmyn,且zkyn,则Z是X与Yn-1的LCS，即LCSXY= LCSXYn-1



图解为:
![](http://orh99zlhi.bkt.clouddn.com/2017-12-21,16:10:12.jpg)

子问题重复性:
![](http://orh99zlhi.bkt.clouddn.com/2017-12-21,16:12:05.jpg)


解决办法:
 C[i, j] = Xi与Yj 的LCS的长度:
 LCS的递归方程为:

* C[i,j]=0,if i=o or j=0
* C[i,j] = C[i-1,j-1] + 1,if i,j>0 and xi=yj
* C[i,j] = max(C[i-1,j],C[i,j-1]),if i,j>0,and xi不等于yj

![](http://orh99zlhi.bkt.clouddn.com/2017-12-21,16:18:45.jpg)


计算过程:
    ![](http://orh99zlhi.bkt.clouddn.com/2017-12-21,16:19:26.jpg)


伪代码:

```
      LCS-length(X, Y)
      m=length(X)；n=length(Y)；
      For   i1   To   m   Do   C[i,0]0;
      For   j1   To   n    Do   C[0,j]0;
      For   i1   To   m   Do
           For   j1   To   n   Do
               If  xi = yj 
               Then C[i,j]C[i-1,j-1]+1；B[i,j]“↖”; 
               Else If C[i-1,j]C[i,j-1]
                       Then C[i,j]C[i-1,j]; B[i,j]“↑”;
                       Else C[i,j]C[i,j-1]; B[i,j]“←”; 
   Return C and B. 
```

```
Print-LCS(B, X, i, j)
IF  i=0  or  j=0  THEN  Return;
IF  B[i, j]=“↖”  
THEN  Print-LCS(B, X, i-1, j-1);  Print xi; 
ELSE   If   B[i, j]=“↑”  
             THEN   Print-LCS(B, X, i-1, j);
             ELSE   Print-LCS(B, X, i, j-1).

Print-LCS(B, X, length(X), length(Y))
可打印出X与Y的LCS。 
```

```

#include <iostream>
#include <string>
using namespace std;
#define  MAX_LENGTH  100
int c[MAX_LENGTH][MAX_LENGTH]={0};
int order[MAX_LENGTH][MAX_LENGTH]={0} ;
void  lcs_length(string a,string b){

    int m = a.size();
    int n= b.size();
    for(int i=0;i<=m;i++)
    {
        c[i][0]=0;
    }
    for(int i=1;i<=n;i++)
    {
        c[0][i]=0;
    }
    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(a[i]==b[j])
            {
                c[i][j] = c[i-1][j-1] +1;
                order[i][j]=1;

            } else if (c[i-1][j]>c[i][j-1])
            {
                c[i][j] = c[i-1][j];
                order[i][j]=2;
            } else{
                    c[i][j] = c[i][j-1];
                    order[i][j]=3;
                }

        }
    }

}
void print_lcs(string,int,int);
int main()
{

    string a;
    string b;
    cout<<"请输入字符串A"<<endl;
  cin>>a;
     cout<<"请输入字符串B"<<endl;
    cin>>b;
    for(int i=0;i<MAX_LENGTH;i++)
    {
        for(int j=0;j<MAX_LENGTH;j++)
        {
            c[i][j]=0;
            order[i][j]=0;
        }
    }
    int m = a.size();
    int n = b.size();

    lcs_length(a,b);

    print_lcs(a,m,n);
}

void print_lcs(string a,int i,int j)
{

    if (i==0 || j ==0)
    {
        return;
    } else{
        if (order[i][j]==1)
        {
            print_lcs(a,i-1,j-1);
            cout<<a[i];
        } else{
            if (order[i][j]==2)
            {
                print_lcs(a,i-1,j);
            } else{
                print_lcs(a,i,j-1);
            }
        }
    }
}


```



算法复杂度:

时间O(n的平方)


空间O(n的平方)

###0/1背包问题:
问题描述:

给定n种物品和一个背包，物品i的重量是wi，价值vi,  背包容量为C,  问如何选择装入背包的物品，使装入背包中的物品的总价值最大？


限制:

对于每种物品只能选择完全装入或不装入，一个物品至多装入一次。


输入:$C>0, wi>0, vi>0, 1 <=i<=n$
输出:$$(x1, x2, …, xn), xi属于{0, 1}, 满足∑1<=i<=n Wi*xi<= C,   ∑1<=i<=n,vi*xi 最大$$

![](http://orh99zlhi.bkt.clouddn.com/2017-12-22,11:29:30.jpg)
                 
递归方程:
![](http://orh99zlhi.bkt.clouddn.com/2017-12-22,11:30:11.jpg)

第一条:当背包容量小于第I个物品重量的时候,肯定不选第i个物品
第二条:是否选择第I个物品加入背包
第三条:小于的物品重量的时候,不选为0
第四条:只选一个物品的时候


填写规则:
从下往上,从左往右:
![](http://orh99zlhi.bkt.clouddn.com/2017-12-22,11:36:23.jpg)


算法:
![](http://orh99zlhi.bkt.clouddn.com/2017-12-22,11:37:02.jpg)

例子:

当重量分别是:2,2,6,5,4
价值分别是:6,3,5,4,6

![](http://orh99zlhi.bkt.clouddn.com/2017-12-22,22:46:13.jpg)

```
#include <iostream>
#include <string>
using namespace std;
int weights[] = {0,2,2,6,5,4};
int values[] ={0,6,3,5,4,6};
int W = 10;
int m[6][10]={0};

void Package0_1(int n);
void print_select(int index,int count);
int min(int a,int b){
    if(a>b)
    {
        return b;
    } else{
        return a;
    }
}
int max(int a,int b)
{
    if(a>b)
    {
        return a;
    } else
    {
        return b;
    }
}
int main()
{
     Package0_1(5);
    print_select(1,10);
}

void Package0_1(int n)
{

        for(int j=1;j<=min(weights[n]-1,W);j++)
        {
            m[n][j]=0;
        }
        for(int j=weights[n];j<=W;j++)
        {
            m[n][j]=values[n];

        }

    cout<<endl;
    for(int i=n-1;i>=1;i--)
    {
        for(int j=0;j<=min(weights[i]-1,W);j++)
        {
            m[i][j] = m[i+1][j];
        }
        for(int j = weights[i];j<=W;j++)
        {
            m[i][j] = max(m[i+1][j],m[i+1][j-weights[i]]+values[i]);
        }

    }






}
void print_select(int index,int count)
{
    if (index > 5)
    {
        return;
    }
    if(m[index][count]==m[index+1][count])
    {
        cout<<index<<"not  selct"<<endl;
        print_select(index+1,count);
    }
    else
    {
        cout<<index<<"selct"<<endl;
        print_select(index+1,count-weights[index]);

    }
}
```


  


