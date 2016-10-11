#include <stdio.h>
#include <stdlib.h>
//  本算法主要 利用 异或的思想
//  因为 0^1=1 0^0=0  所以 0^1^0=1 0^1^1=0
//   所以实际上 如果又两个方向指针  prev 跟next   链域 中只需要存  prev^next  然后  保存头指针跟第二个指针，
//   需要用的时候   如果 是要next  即可 prev^prev^next=next   如果是prev ，则可以 next^next^prev
//    难点 ，在于对位运算的 掌握，以及建立环形链表时候把 某一节点的 前驱跟后继 表示出来
//对了，异或时候  long long 的梗 是因为Position为结构体指针，指针的内存大小在32位机上为4个字节即32位，在64位机上是8个字节，所以为了通用性，将其转化为long long而不是int，
//    废话不多说，开撸
struct Node
{
    int data;
    struct Node* prevOrnext;
};
typedef long long LL;
typedef struct Node *Position;
typedef Position RingList;
RingList createRinglist(Position *prevNode,Position *nextNode);
void clockWise(RingList rL,Position prev2);
void AntiClockWise(RingList rl,Position next);
int main()
{
    int choose;
    Position prev,next;
    printf("input the numbers ,EOF Or -1 end the input\n");
    RingList rL=createRinglist(&prev,&next);
    printf("1:顺时针\n2:逆时针\n");
    scanf("%d",&choose);
    switch(choose)
    {
      case 1:clockWise(rL,prev);
             break;
      case 2:AntiClockWise(rL,next);
             break;
    }
    return 0;
}
RingList createRinglist(Position *prevNode,Position *nextNode)
{
    Position p,prev,next,firstP,secondP;
    RingList rL=(struct Node*)malloc(sizeof(struct Node));
    int x;
    p=rL;
    prev=0;
    firstP=NULL;
    secondP=NULL;
    int n=0;
    while(scanf("%d",&x)&&x!=-1)
    {
        n++;
        Position newP=(struct Node*)malloc(sizeof(struct Node));
        if(1==n)
        {
            firstP=newP;
        }
        if(2==n)
        {
            secondP=newP;
        }
        newP->data=x;
        next=newP;//next指向新节点，那就要保存上一个节点的prevOrnext值了
        p->prevOrnext=(Position)((long long)prev^(long long)next);//异或处理时 记录前后位置，注意：P为头指针的时候DATA为空,prev为0.所以rl->prevOrnext  的值是第一个DATA不为空的节点
        prev=p;//p节点的链域保存完毕，把前驱前进
        p=next;
    }
    p->prevOrnext=(Position)((long long)prev^(long long)firstP);//因为p节点为最后一个节点了。所有要前驱跟第一个节点异或处理
    firstP->prevOrnext=(Position)((long long)p^(long long)secondP);
    *prevNode=p;  //因为操作的时候从第一个节点开始，所以返回第一个节点的前驱跟后继
    *nextNode=secondP;
    return rL;
}
void clockWise(RingList rL,Position prev2)
{
    Position firstP=rL->prevOrnext;//因为rL的data值为空，rl->prevOrnext为第一个存数的节点，所 firstp=rl->prevornext，
    Position prev1=firstP;
    Position op=NULL;
    printf("%d",firstP->data);
    op=(Position)((long long)prev1->prevOrnext^(long long)(prev2));
    while(op!=firstP)
    {
        printf(" %d",op->data);
        prev2=prev1;
        prev1=op;
        op=(Position)((long long)prev1->prevOrnext^(long long)(prev2));
    }
    printf("\n");
}
void AntiClockWise(RingList rL,Position next2)
{
    Position firstP=rL->prevOrnext;//因为rL的data值为空，rl->prevOrnext为第一个存数的节点，所 firstp=rl->prevornext，
    Position next1=firstP;
    printf("%d",firstP->data);
    Position op=(Position)((long long)next1->prevOrnext^(long long)(next2));
    while(op!=firstP)
    {
        printf(" %d",op->data);
        next2=next1;
        next1=op;
        op=(Position)((long long)next1->prevOrnext^(long long)(next2));
    }
    printf("\n");
}
