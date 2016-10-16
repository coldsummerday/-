#include <stdio.h>
#include <stdlib.h>
//  ���㷨��Ҫ ���� ����˼��
//  ��Ϊ 0^1=1 0^0=0  ���� 0^1^0=1 0^1^1=0
//   ����ʵ���� �������������ָ��  prev ��next   ���� ��ֻ��Ҫ��  prev^next  Ȼ��  ����ͷָ����ڶ���ָ�룬
//   ��Ҫ�õ�ʱ��   ��� ��Ҫnext  ���� prev^prev^next=next   �����prev ������� next^next^prev
//    �ѵ� �����ڶ�λ����� ���գ��Լ�������������ʱ��� ĳһ�ڵ�� ǰ������� ��ʾ����
//���ˣ����ʱ��  long long �Ĺ� ����ΪPositionΪ�ṹ��ָ�룬ָ����ڴ��С��32λ����Ϊ4���ֽڼ�32λ����64λ������8���ֽڣ�����Ϊ��ͨ���ԣ�����ת��Ϊlong long������int��
//    �ϻ�����˵����ߣ
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
    printf("1:˳ʱ��\n2:��ʱ��\n");
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
        next=newP;//nextָ���½ڵ㣬�Ǿ�Ҫ������һ���ڵ��prevOrnextֵ��
        p->prevOrnext=(Position)((long long)prev^(long long)next);//�����ʱ ��¼ǰ��λ�ã�ע�⣺PΪͷָ���ʱ��DATAΪ��,prevΪ0.����rl->prevOrnext  ��ֵ�ǵ�һ��DATA��Ϊ�յĽڵ�
        prev=p;//p�ڵ�����򱣴���ϣ���ǰ��ǰ��
        p=next;
    }
    p->prevOrnext=(Position)((long long)prev^(long long)firstP);//��Ϊp�ڵ�Ϊ���һ���ڵ��ˡ�����Ҫǰ������һ���ڵ������
    firstP->prevOrnext=(Position)((long long)p^(long long)secondP);
    *prevNode=p;  //��Ϊ������ʱ��ӵ�һ���ڵ㿪ʼ�����Է��ص�һ���ڵ��ǰ�������
    *nextNode=secondP;
    return rL;
}
void clockWise(RingList rL,Position prev2)
{
    Position firstP=rL->prevOrnext;//��ΪrL��dataֵΪ�գ�rl->prevOrnextΪ��һ�������Ľڵ㣬�� firstp=rl->prevornext��
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
    Position firstP=rL->prevOrnext;//��ΪrL��dataֵΪ�գ�rl->prevOrnextΪ��һ�������Ľڵ㣬�� firstp=rl->prevornext��
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
