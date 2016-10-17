/*
迷宫问题  0 代表能走，1不能走， 输出最短路长度
 深度搜索算法
  思路在某一个点把该点打下一次所有可能走的点都记录，再从下一次开始。

  不同与 深搜，深搜是 该步能走的话直接在这步打基础上走下一步

*/
//思路 ，用队列存该点与下一次能走的所有点
#include <stdio.h>
struct note
{
  int x;
  int y;
  int f;
  int s;
};
int main()
{
  struct note que[2501];//迷宫50*50 万一每一步都能走.. 队列开大点
  int a[51][51]={0};//迷宫
  int book[51][51]={0};//记录该点是否被走过
  int next[4][2]={{0,1},//右
                  {1,0},//下
                  {0,-1},//左
                  {-1,0}};//上
  int tail,head;
  int i,j,k,n,m,startx,starty,p,q,tx,ty,flag;//flag代表走到啦，tx,ty为下一次出发点
  scanf("%d %d",&n,&m);
  for(i=1;i<=n;i++)
  {
    for(j=1;j<=m;j++)
    {
      scanf("%d",&a[i][j]);
    }
  }
  scanf("%d %d %d %d",&startx,&starty,&p,&q);//设要找的在P，Q
  head=1;
  tail=1;
  que[tail].x=startx;
  que[tail].y=starty;
  que[tail].f=0;//记录该点的上一个点在队列的哪个位置，然后找路径时候可以通过f找
  que[tail].s=0;//记录步数
  tail++;
  book[startx][starty]=1;
  flag=0;
  while (head<tail)//当队列不为空证明没找到目标点且最后一个点没到达
   {
    for(k=0;k<3;k++)
    {
      tx=que[head].x+next[k][0];
      ty=que[head].y+next[k][1];
      if(tx<1||tx>n||ty<1||ty>m)//判断是否出边界了
      {
        continue;
      }
      if((a[tx][ty]==0)&&(book[tx][ty]==0))//该点可走且没走过的话
      {
        book[tx][ty]=1;
        que[tail].x=tx;
        que[tail].y=ty;
        que[tail].f=head;//记住前一个点的位置，方便找最短路
        que[tail].s=que[head].s+1;//步数加一
        tail++;
      }
      if((tx==p) && (ty==q))//找到了，需要跳两层循环 所以用flag
      {
        flag=1;
        break;
      }
    }

      if(flag==1)
        break;
        head++;
    }
  printf("%d",que[tail-1].s);//因为tail是先用后加1 ，所以队尾前一个才是最后的点
  getchar();
  return 0;
}
