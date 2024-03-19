#include<stdio.h>
#include<unistd.h>
int main()
{
        execl("/bin/ls","/bin/ls","-l", NULL);
        printf("can only get here on error\n");
}

