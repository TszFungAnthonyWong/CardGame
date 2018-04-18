#include<unistd.h>
#include<stdio.h>

int main()
{
   printf("Before exec()ing ls!\n");
   execlp("ls","ls",NULL);
   printf("After execing ls!\n");
   
   return 0;
}

