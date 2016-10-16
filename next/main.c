#include <stdio.h>
#include <stdlib.h>
#include<strings.h>
int Getlenth(char *t);
void GetNext(char *t,int next[]);
int main()
{
   char t[15];
   int len=0;
   scanf("%s",t);
   len=strlen(t);
   int next[len];
   GetNext(t,next);
   return 0;
}
void GetNext(char *t,int next[])
{
  int  j = -1,  i = 0;
  next[0] = -1;
  int len=strlen(t);
  while(i < len)
  {
            if(j == -1 || t[i] == t[j])
           {

                      i++;
                      j++;
                     next[i] = j;
          }
          else
         {
                    j = next[j];
         }
 }
  for(i=1;i<=len;i++)
  {
   printf("%d,",next[i]);
  }
}

