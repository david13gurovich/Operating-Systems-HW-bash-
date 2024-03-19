#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int     glob = 6;                               /* global variable */
char    buf[] = "a write to stdout";

int main(void)
{
        int     var;                            /* automatic variable on the stack */
        pid_t   pid;

        var = 88;
        if(puts(buf)==EOF)
                printf("error in writing to stdout");
        printf("before fork\n");

        if ( (pid = fork()) < 0)
                printf("fork error");
        else{
                if (pid == 0)
                {       /* child */
                        glob++;         /* modify variables */
                        var++;
                }else
                { /* parent */
                        sleep(2);
                }
                printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
                exit(0);
        }
}

