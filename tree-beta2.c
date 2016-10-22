#include <stdio.h>
#include <stdlib.h>
/*
原谅我菜单没打中文（编码没统一，所以很容易乱码干脆就用英文表示了）
菜单：
    1 建立二叉树
    （1）直接用满二叉树前序顺序输入，空节点用#表示（老师PPT）
    （2） 交互式输入二叉树，空节点照样用#
    2：前序输出二叉树
    3：中序输出二叉树
    4：后序输出二叉树
    5：层序输出二叉树（其思想类似广度搜索，用队列，每一个节点不为空的话就放队尾）
    6:高度
    7：宽度
    注意：我的链表 DATA值 为了方便存了字符类型
*/
struct TreeNode
{
  char data;
  struct TreeNode *lchild;
  struct TreeNode *rchild;
};
char inputs[50];//存储先序输入的字符串
int op=-1;//一次性输入时候字符串的下标
int menu();
struct TreeNode *InorderCreateTree();
struct TreeNode *InorderCreateTree2();
struct TreeNode *SetupTree(struct TreeNode *bt,int n);
void PreOrderVisit(struct TreeNode *bt);
void MidOrderVisit(struct TreeNode *bt);
void PostOrderVisit(struct TreeNode *bt);
void FloorOrderVisit(struct TreeNode *bt);
void PreOrderVisitByStack(struct TreeNode *bt);
void MidOrderVisitByStack(struct TreeNode *bt);
void PostOrderVisitByStack(struct TreeNode *bt);
int  HeightOfTree(struct TreeNode *bt);
int WidthOfTree(struct TreeNode *bt);
int main()
{
  struct TreeNode *root=NULL;
  int choose;
  int Width;
  int i;
  int height;
  while (1) {
    choose=menu();
    switch (choose) {
      case 1:printf("1:PreOrder input one by one, empty node input the'#'\n");
            printf("2:interactive input,empty node input the'#'\n");
            printf("3:PreOrder input , empty node input the'#'\n");
             scanf("%d",&i);
             switch (i)
             {
               case 1:root=InorderCreateTree();
                      break;
               case 2:root=SetupTree(root,0);
                      break;
               case 3:scanf("%s",&inputs);
                      root=InorderCreateTree2();
                      break;
             }
        break;
      case 2:printf("1: by recursion\n");
             printf("2: non-recursion\n");
             scanf("%d",&i);
             switch(i)
             {
                 case 1:PreOrderVisit(root);
                  break;
                 case 2:PreOrderVisitByStack(root);
                  break;
             }
      break;
      case 3:printf("1: by recursion\n");
             printf("2: non-recursion\n");
             scanf("%d",&i);
             switch(i)
             {
                 case 1:MidOrderVisit(root);
                 break;
                 case 2:MidOrderVisitByStack(root);
                 break;
             }
      break;
      case 4:printf("1: by recursion\n");
             printf("2: non-recursion\n");
             scanf("%d",&i);
             switch(i)
             {
                 case 1:PostOrderVisit(root);
                 break;
                 case 2:PostOrderVisitByStack(root);
                 break;
             }
      break;
      case 5:FloorOrderVisit(root);
      break;
      case 6:
             height=HeightOfTree(root);
             printf("%d\n",height);
             break;
      case 7:
             Width=WidthOfTree(root);
             printf("%d\n",Width);
             break;
      case 8:exit(0);
    }
  }
};
int menu()
{
  int choose;
  printf("-----------------------\n");
  printf("1:setup the tree\n");
  printf("2:preOrder printf the tree\n");
  printf("3:Middle order printf the tree\n");
  printf("4:PostOrder printf the tree\n");
  printf("5:floorOrder printf the tree\n");
  printf("6:the tree height is:\n");
  printf("7:the tree width is:\n");
  printf("8:exit\n");
  printf("-----------------------\n");
  scanf("%d",&choose);
  return choose;
}
struct TreeNode *SetupTree(struct TreeNode *bt,int n)
{
  char ch;
  if(n==0)
  printf("root\n");
  fflush(stdin);
  scanf("%ch",&ch);
  fflush(stdin);
  if(ch!='#')
  {
    n=1;
    bt=(struct TreeNode*)malloc(sizeof(struct TreeNode));
    bt->data=ch;
    bt->lchild=NULL;
    bt->rchild=NULL;
    printf("%c lchild is:",bt->data);
    bt->lchild=SetupTree(bt->lchild,n);
    printf("%c rchild is:",bt->data);
    bt->rchild=SetupTree(bt->rchild,n);
  }
  return(bt);
}
struct TreeNode *InorderCreateTree()
{
  struct TreeNode *bt;
  char ch;
  fflush(stdin);
  scanf("%c",&ch);
  if(ch=='#')
  bt=NULL;
  else
  {
    bt=(struct TreeNode*)malloc(sizeof(struct TreeNode));
    bt->data=ch;
    bt->lchild=InorderCreateTree();
    bt->rchild=InorderCreateTree();
  }
  return bt;
}
struct TreeNode *InorderCreateTree2()
{
  struct TreeNode *bt;
  op++;
  if(inputs[op]=='#')//为什么不用考虑数组越界问题，因为最后的叶子肯定是以两个“##”结束的，下标到最后的同时已经递归结束了，所以省略了判断
     {               //除非输入错误，但是可以先判断
         bt=NULL;
         return bt;
     }
  else
  {
    bt=(struct TreeNode*)malloc(sizeof(struct TreeNode));
    bt->data=inputs[op];
    bt->lchild=InorderCreateTree2();
    bt->rchild=InorderCreateTree2();
  }
  return bt;
}
void PreOrderVisit(struct TreeNode *bt)
{
    if(bt!=NULL)
    {
        printf("%c\t",bt->data);
        PreOrderVisit(bt->lchild);
        PreOrderVisit(bt->rchild);
    }
    else return;
};
void MidOrderVisit(struct TreeNode *bt)
{
    if(bt!=NULL)
    {
        MidOrderVisit(bt->lchild);
        printf("%c\t",bt->data);
        MidOrderVisit(bt->rchild);
    }
    else return;
}
void PostOrderVisit(struct TreeNode *bt)
{
    if(bt!=NULL)
    {
        PostOrderVisit(bt->lchild);
        PostOrderVisit(bt->rchild);
        printf("%c\t",bt->data);
    }
    else return;
}
void FloorOrderVisit(struct TreeNode *bt)
{
    struct TreeNode *que[100];
    int head=1;
    int tail=1;
    que[tail]=bt;
    tail++;
    while(head<tail)
    {
        if(que[head]->lchild!=NULL)
        {
            que[tail++]=que[head]->lchild;
        }
        if(que[head]->rchild!=NULL)
        {
            que[tail++]=que[head]->rchild;
        }
        printf("%c\t",que[head]->data);
        head++;
    }
}
void PreOrderVisitByStack(struct TreeNode *bt)
{
  struct TreeNode *Stack[100];
  int top=0;
  do{
    while(bt!=NULL)
    {
        printf("%c\t",bt->data);
        top++;
        if(top<100)
        {
            Stack[top]=bt;
            bt=bt->lchild;
        }
        else{
            printf("the stack is  full~\n");
        }
    }
    if(top!=0)
    {
        bt=Stack[top];
        top--;
        bt=bt->rchild;
    }
  }while((top!=0)||(bt!=NULL));
  return;
}
void MidOrderVisitByStack(struct TreeNode *bt)
{
    struct TreeNode *S[100];
    int top=0;
    do{
        while(bt!=NULL)
        {
            top++;
            if(top<100)
            {
                S[top]=bt;
                bt=bt->lchild;
            }
            else
            {
             printf("the stack is  full~\n");
            }
        }
        if(top!=0)
        {
            bt=S[top];
            top--;
            printf("%c\t",bt->data);
            bt=bt->rchild;
        }
    }while((top!=0)||(bt!=NULL));
  return;
}
void PostOrderVisitByStack(struct TreeNode *bt)
{
    struct TreeNode *stack[100];
    int flag;
    struct TreeNode *p;//用来表示该节点是否已被访问
    int top=0;
    do{
        while(bt!=NULL)
        {
            top++;
            if(top>=100)
                printf("Stack is full\n");
            else
            {
                stack[top]=bt;
                bt=bt->lchild;
            }
        }
        p=NULL;
        flag=1;
        while(top!=0&&flag)
        {
            bt=stack[top];
            if(bt->rchild==p)
            {
                printf("%c\t",bt->data);
                top--;
                p=bt;
            }
            else{
                bt=bt->rchild;
                flag=0;
            }
        }
    }while(top!=0);
return;
}
int HeightOfTree(struct TreeNode *bt)
{
    int ldepth,rdepth;
    if(bt==NULL)
    {
        return 0;
    }
    else
    {
        ldepth=HeightOfTree(bt->lchild);
        rdepth=HeightOfTree(bt->rchild);
        if(ldepth>rdepth)
        {
            return(ldepth+1);//加一是为了加上根节点
        }
        else{
            return(rdepth+1);
        }
    }
}
int WidthOfTree(struct TreeNode *bt)//算法思路，用广搜层序遍历的方法把所有点以及他所在层放入队列
{                                   //然后用桶排序思路在I层就数组【I】++， 然后找最大
    int i;
    int front=0,tail=0,max=0;
    int maxlev[10]={0};
    struct WI
    {
        int nodelev;
        struct TreeNode *node;
    }que[50];
    que[front].node=bt;
    que[front].nodelev=1;
    while(front<=tail)
    {
        if(que[front].node->lchild)
        {
            que[++tail].node=que[front].node->lchild;
            que[tail].nodelev=que[front].nodelev+1;
        }
        if(que[front].node->rchild)
        {
            que[++tail].node=que[front].node->rchild;
            que[tail].nodelev=que[front].nodelev+1;
        }
        front++;
    }
    for(i=0;i<tail;i++)
    {
        maxlev[que[i].nodelev]++;
    }
    for(i=0;i<10;i++)
    {
        if(max<maxlev[i])
            max=maxlev[i];
    }
    return max;
}
